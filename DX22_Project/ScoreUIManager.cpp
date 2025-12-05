#include "ScoreUIManager.h"
#include "ScoreManager.h"
#include "GameObject.h"

ScoreUIManager::ScoreUIManager(CanvasUI* th, CanvasUI* h, CanvasUI* t, CanvasUI* o, CanvasUI* kg)
{
    if (!th || !h || !t || !o || !kg) return;

    uiNumbers[ScoreDigit::Thousands] = th;
    uiNumbers[ScoreDigit::Hundreds] = h;
    uiNumbers[ScoreDigit::Tens] = t;
    uiNumbers[ScoreDigit::Ones] = o;
    uiKg = kg;
}

void ScoreUIManager::Start()
{
    // 初期値 0 にしておく
    currentValue = 500;

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
    // 位置・サイズ設定
    uiKg->SetOffset({ 940.0f, 130.0f });
    uiKg->SetSize({ 0.6, 0.6 });
    uiKg->SetUVPos({ 1, 1 });
    uiKg->SetUVScale({ 1, 1 });
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
    // currentValue (0〜9999) を各桁に分解
    int value = ScoreManager::Instance().GetRemainingKg();

    int digits[ScoreDigit::Count];
    digits[Thousands] = (value / 1000) % 10;
    digits[Hundreds] = (value / 100) % 10;
    digits[Tens] = (value / 10) % 10;
    digits[Ones] = value % 10;

    // 各 CanvasUI の UV を更新
    for (int i = 0; i < ScoreDigit::Count; ++i)
    {
        CanvasUI* ui = uiNumbers[i];
        if (!ui) continue;

        const int d = digits[i];         // 0〜9
        ui->SetUVPos(GetNumberUVPos(d)); // 「d の数字」の左上に UV を移動
        // UV サイズは Start() で設定済み
    }
}
