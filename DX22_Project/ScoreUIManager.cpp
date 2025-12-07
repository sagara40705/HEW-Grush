#include "ScoreUIManager.h"
#include "ScoreManager.h"
#include "GameObject.h"
#include <cmath>
#include<algorithm> // std::min, std::max用

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
    // 初期値 0 にしておく
    currentValue = 500;

    // これをしないと開始時に0から500になったと判定されて揺れてしまうため
    int startValue = ScoreManager::Instance().GetRemainingKg();
    lastValue = startValue;

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
    // 本当のスコアを取得
    int targetValue = ScoreManager::Instance().GetRemainingKg();

    // 振動のトリガー判定（本当の値が変動したかどうか）
    if (targetValue != lastValue)
    {
        shakeTimer = kShakeDuration;

        if (targetValue > lastValue)
        {
            currentShakeType = ShakeType::Vertical;  // 増えたら縦揺れ
        }
        else
        {
            currentShakeType = ShakeType::Horizontal; // 増えたら横揺れ
        }
    }

    lastValue = targetValue; // 次回の比較用に「本当の値」を保存

    if (currentValue < targetValue)
    {
        currentValue++; // 目標より小さいなら 1つずつ増やす
        // 【補足】もし差が大きすぎて時間がかかりすぎる場合は、
        // 以下のように差分に応じてスピードを変えると良いです。
        // int diff = targetValue - currentValue;
        // int step = std::max(1, diff / 10); // 差の1/10ずつ進む（最低1）
        // currentValue += step;
    }
    else if(currentValue > targetValue)
    {
        currentValue = targetValue;
    }

    Vector2 shakeOffset = { 0.0f, 0.0f };

    if (shakeTimer > 0.0f)
    {
        shakeTimer -= 1.0f / 60.0f;
        if (shakeTimer > 0.0f)
        {
            float wave = sinf(shakeTimer * kShakeSpeed) * kShakePower;
            if (currentShakeType == ShakeType::Vertical)
                shakeOffset.y = wave;
            else if (currentShakeType == ShakeType::Horizontal)
                shakeOffset.x = wave;
        }
        else
        {
            currentShakeType = ShakeType::None;
        }
    }

    int tempVal = currentValue;

    int digits[ScoreDigit::Count];
    digits[Thousands] = (tempVal / 1000) % 10;
    digits[Hundreds] = (tempVal / 100) % 10;
    digits[Tens] = (tempVal / 10) % 10;
    digits[Ones] = tempVal % 10;

    // 各 CanvasUI の UV を更新
    for (int i = 0; i < ScoreDigit::Count; ++i)
    {
        CanvasUI* ui = uiNumbers[i];
        if (!ui) continue;

        ui->SetUVPos(GetNumberUVPos(digits[i])); // 「d の数字」の左上に UV を移動
        // UV サイズは Start() で設定済み
        ui->SetOffset(kNumberPos[i] + shakeOffset);
    }
}
