#include "ScoreUIManager.h"
#include "ScoreManager.h"
#include "GameObject.h"
#include <cmath>    // sinf, abs
#include <cstdlib>  // rand

ScoreUIManager::ScoreUIManager(CanvasUI* th, CanvasUI* h, CanvasUI* t, CanvasUI* o)
{
    if (!th || !h || !t || !o) return;

    uiNumbers[ScoreDigit::Thousands] = th;
    uiNumbers[ScoreDigit::Hundreds] = h;
    uiNumbers[ScoreDigit::Tens] = t;
    uiNumbers[ScoreDigit::Ones] = o;
}

void ScoreUIManager::Start()
{
    // 初期値設定（適当な初期値）
    currentValue = 500;

    // 重要：開始時の「本当の値」を取得して同期しておく
    // （これをしないと、開始直後に0→500と判定されて演出が走ってしまうため）
    int startValue = ScoreManager::Instance().GetRemainingKg();
    lastValue = startValue;
    currentValue = startValue; // 最初は一致させておく

    // 各桁の初期化
    for (int i = 0; i < ScoreDigit::Count; ++i)
    {
        if (!uiNumbers[i]) continue;

        // 位置・サイズ設定
        uiNumbers[i]->SetOffset(kNumberPos[i]);
        uiNumbers[i]->SetSize(kNumberSize);

        // 数字 0 の UV にしておく
        uiNumbers[i]->SetUVPos(GetNumberUVPos(0));
        uiNumbers[i]->SetUVScale(kNumberUVSize);
    }
}

void ScoreUIManager::OnDestroy()
{
    for (int i = 0; i < ScoreDigit::Count; ++i)
    {
        uiNumbers[i] = nullptr;
    }
}

void ScoreUIManager::SetValue(int v)
{
    if (v < 0) v = 0;
    if (v > MAX_VALUE) v = MAX_VALUE;
    currentValue = v;
}

void ScoreUIManager::Update()
{
    // 1. 現在の本当のスコアを取得
    int targetValue = ScoreManager::Instance().GetRemainingKg();

    // ---------------------------------------------------------
    //  A. 減少判定（ダメージなど）
    // ---------------------------------------------------------
    if (targetValue != lastValue)
    {
        if (targetValue < lastValue)
        {
            // 減った時は即座に値を反映し、ダメージ演出（激しい振動）を開始
            currentValue = targetValue;
            currentShakeType = ShakeType::Damage;
            shakeTimer = kDamageDuration;
            currentScale = 1.0f; // サイズは通常に戻す
        }
        // 増えた時は下の「カウントアップ」で演出するので、ここでは何もしない
    }
    lastValue = targetValue; // 次回用に値を保存

    // ---------------------------------------------------------
    //  B. 増加カウントアップ（ドンキーコング風演出）
    // ---------------------------------------------------------
    if (currentValue < targetValue)
    {
        // 1ずつ増やす（パラパラと増える）
        currentValue++;

        // ★増えるたびに「デカく」して「跳ねる」モードにリセット
        // これにより、連続して増えている間はずっとボヨヨンと動き続ける
        currentScale = kMaxScale;               // 1.5倍にセット！
        currentShakeType = ShakeType::Bouncing; // バウンスモード
        shakeTimer = kBounceDuration;           // タイマーセット
    }
    else if (currentValue > targetValue)
    {
        // ターゲットを超えていたら補正（減少時の処理漏れ対策）
        currentValue = targetValue;
    }

    // ---------------------------------------------------------
    //  C. アニメーション計算（スケール減衰 ＆ 揺れオフセット）
    // ---------------------------------------------------------

    // サイズを徐々に 1.0 に戻す（弾力感）
    if (currentScale > 1.0f)
    {
        currentScale -= 0.1f; // 減衰スピード
        if (currentScale < 1.0f) currentScale = 1.0f;
    }

    // 揺れの計算
    Vector2 shakeOffset = { 0.0f, 0.0f };

    if (shakeTimer > 0.0f)
    {
        shakeTimer -= 1.0f / 60.0f; // 60FPS想定で減算

        if (shakeTimer > 0.0f)
        {
            if (currentShakeType == ShakeType::Bouncing)
            {
                // 【バウンス】 abs(sin) で「地面から上に跳ねる」動きを作る
                // kJumpPower が正の値なら、Y座標はマイナス方向（画面上）へ動かす
                float bounce = std::abs(sinf(shakeTimer * kShakeSpeed));
                shakeOffset.y = -bounce * kJumpPower;
            }
            else if (currentShakeType == ShakeType::Damage)
            {
                // 【ダメージ】 ランダムにガタガタ震える
                float rx = (static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f;
                float ry = (static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f;
                shakeOffset.x = rx * 5.0f; // 横揺れ幅
                shakeOffset.y = ry * 5.0f; // 縦揺れ幅
            }
        }
        else
        {
            // タイマー終了時に演出終了（カウントアップが終わっていれば）
            if (currentValue == targetValue)
            {
                currentShakeType = ShakeType::None;
            }
        }
    }

    // ---------------------------------------------------------
    //  D. UIへの適用
    // ---------------------------------------------------------
    int tempVal = currentValue;

    // 桁ごとの数値を算出
    int digits[ScoreDigit::Count];
    digits[Thousands] = (tempVal / 1000) % 10;
    digits[Hundreds] = (tempVal / 100) % 10;
    digits[Tens] = (tempVal / 10) % 10;
    digits[Ones] = tempVal % 10;

    for (int i = 0; i < ScoreDigit::Count; ++i)
    {
        CanvasUI* ui = uiNumbers[i];
        if (!ui) continue;

        // UV設定
        ui->SetUVPos(GetNumberUVPos(digits[i]));

        // --- サイズ適用 ---
        Vector2 finalSize = {
            kNumberSize.x * currentScale,
            kNumberSize.y * currentScale
        };
        ui->SetSize(finalSize);

        // --- 中心補正 ---
        // 左上基準で拡大すると位置がズレて見えるため、
        // (元のサイズ - 今のサイズ) / 2 だけずらして中心を合わせる
        Vector2 centerCorrection = {
            (kNumberSize.x - finalSize.x) * 0.5f,
            (kNumberSize.y - finalSize.y) * 0.5f
        };

        // --- 最終座標セット ---
        // 基本位置 + 揺れ + 中心補正
        ui->SetOffset(kNumberPos[i] + shakeOffset + centerCorrection);
    }
}