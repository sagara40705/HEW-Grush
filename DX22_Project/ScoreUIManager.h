#pragma once
#include "CanvasUI.h"
#include <cmath> // 必要に応じて

class ScoreUIManager : public MonoBehaviour
{
public:
    // 4桁 (最大 9999)
    enum ScoreDigit
    {
        Thousands,  // 千の位
        Hundreds,   // 百の位
        Tens,       // 十の位
        Ones,       // 一の位
        Count
    };

private:
    CanvasUI* uiNumbers[ScoreDigit::Count]{}; // 4桁分
    int currentValue = 0;                     // 現在表示している値（演出で徐々に変化する）
    static constexpr int MAX_VALUE = 9999;

    // --- アニメーション制御用変数 ---
    int lastValue = 0;              // 前回の真の値を保存（増減判定用）
    float shakeTimer = 0.0f;        // 振動・演出の残り時間
    float currentScale = 1.0f;      // 現在の拡大率 (1.0 = 通常, 1.5 = 拡大)

    // 揺れの種類
    enum class ShakeType {
        None,       // なし
        Bouncing,   // ドンキーコング風：跳ねながら拡大（スコア増加時）
        Damage      // ダメージ：激しくランダム振動（スコア減少時）
    };
    ShakeType currentShakeType = ShakeType::None;

    // --- 調整用定数（パラメータ） ---
    const float kBounceDuration = 0.2f; // 跳ねる時間（短くキビキビと）
    const float kDamageDuration = 0.5f; // ダメージ演出の時間
    const float kMaxScale = 1.5f;       // 最大拡大倍率（1.5倍までデカくなる）
    const float kJumpPower = 15.0f;     // 上に跳ねる高さ（ピクセル）
    const float kShakeSpeed = 20.0f;    // 揺れの速さ

public:
    // 4つの CanvasUI を受け取るコンストラクタ
    ScoreUIManager(CanvasUI* th, CanvasUI* h, CanvasUI* t, CanvasUI* o);

    void Start() override;
    void OnDestroy() override;
    void Update() override;

    // 外部から強制的に値をセットする場合（基本はUpdate内でScoreManagerを見る）
    void SetValue(int v);
    int  GetValue() const { return currentValue; }

private:
    // 基本配置座標（適当に調整用）
    const Vector2 kNumberPos[ScoreDigit::Count] = {
        { 1020.f, 80.f },   // 千の位
        { 1070.f, 80.f },   // 百の位
        { 1120.f, 80.f },   // 十の位
        { 1170.f, 80.f },   // 一の位
    };

    // 基本サイズ
    const Vector2 kNumberSize = { 0.5f, 5.0f };

    // UV計算ヘルパー
    Vector2 GetNumberUVPos(int n) { return { n * 0.1f, 1.f }; }
    const Vector2 kNumberUVSize = { 0.1f, 1.f };
};