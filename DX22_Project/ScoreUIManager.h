#pragma once
#include "CanvasUI.h"

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
    int currentValue = 0;
    static constexpr int MAX_VALUE = 9999;

public:
    // 4つの CanvasUI を受け取るコンストラクタ
    ScoreUIManager(CanvasUI* th, CanvasUI* h, CanvasUI* t, CanvasUI* o);

    void Start() override;
    void OnDestroy() override;
    void Update() override;

    // 外部から「目標までの残り kg」を設定する
    void SetValue(int v);
    int  GetValue() const { return currentValue; }

private:
    // 配置とスケール（適当に調整用。位置は必要に応じて変更してください）
    const Vector2 kNumberPos[ScoreDigit::Count] = {
        { 1020.f, 80.f },   // 千の位
        { 1070.f, 80.f },   // 百の位
        { 1120.f, 80.f },   // 十の位
        { 1170.f, 80.f },   // 一の位
    };
    const Vector2 kNumberSize = { 0.5f, 0.5f }; // 縮小率

    // Number.png を TimerUI と同じ仕様で使う前提（0〜9 が横並び）
    Vector2 GetNumberUVPos(int n) { return { n * 0.1f, 1.f }; }
    const Vector2 kNumberUVSize = { 0.1f, 1.f };
};
