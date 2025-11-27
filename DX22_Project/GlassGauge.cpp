#include "GlassGauge.h"
#include "GameObject.h"
#include "Game.h"
#include "GameObject.h"
#include "HummerScript.h"
#include "CanvasUI.h"

GlassGauge::GlassGauge(CanvasUI* frame, CanvasUI* base, CanvasUI* effect)
{
    gUi[Gauge::Frame] = frame;
    gUi[Gauge::Base] = base;
    gUi[Gauge::UpEffect] = effect;
}

void GlassGauge::Start()
{
    // === HummerScriptの取得 ===
    for (auto& o : Game::GetWorld().objects)
    {
        if (!o->active) continue;
        if (!o->CompareTag(Tag::Weapon)) continue;
        hs = o->TryGetComponent<HummerScript>();
        if (hs) break;
    }
    // UI 初期設定
    gUi[Gauge::Frame]->SetOffset({ 0,0 });
    gUi[Gauge::Frame]->SetSize({ 1, 1 });
    gUi[Gauge::Frame]->SetUVPos({ 1, 1 });
    gUi[Gauge::Frame]->SetUVScale({ 1, 1 });

    gUi[Gauge::Base]->SetOffset({ 0, 290  });
    gUi[Gauge::Base]->SetSize({ 1, 0 });
    gUi[Gauge::Base]->SetUVPos({ 0, 1 });
    gUi[Gauge::Base]->SetUVScale({ 1, 0 });

    gUi[Gauge::UpEffect]->SetOffset({ -10, 50 });
    gUi[Gauge::UpEffect]->SetSize({ 1.f / 24.f , 1.f / 12.f });
    gUi[Gauge::UpEffect]->SetUVPos({ 0, 0 });
    gUi[Gauge::UpEffect]->SetUVScale({ 1.f / 8, 1.f / 4 });

   
}

void GlassGauge::OnDestroy()
{
	hs = nullptr;
    for (int i = 0; i < Gauge::Count; ++i)
    {
        gUi[i] = nullptr;
    }
}

void GlassGauge::Update()
{
	if (!hs || !gUi[Gauge::Base]) return;

    //ゲージの割合を取得
	float r = hs->GetGaugeRatio();
	
    //エフェクトの座標の更新

    //ゲージのUIの更新
    gUi[Gauge::Base]->SetPosY( 250 * (1 - r));
    gUi[Gauge::Base]->SetSize({ 1 , r });
    gUi[Gauge::Base]->SetUVPos({ 0 , 1 - r });
    gUi[Gauge::Base]->SetUVScale({ 1 , r });

    if (r > prevGauge + 0.02f)   // 誤差対策で 0.02f 以上なら上昇扱い
    {
        efPlaying = true;
        efFrame = 0;
        gUi[Gauge::UpEffect]->SetColor({ 1,1,1,1 });
    }
    else if (r < prevGauge)
    {
        gUi[Gauge::Frame]->SetColor({ efDenger,0,0,1 });
        efDenger += 0.05;
        if (efDenger >= 1)
            efDenger = 0.0f;
    }
    prevGauge = r;



    //エフェクト再生
    GaugeEffect();


    HummerGrade g = hs->grade;    
    DirectX::XMFLOAT4 col;

    switch (g)
    {
    case HummerGrade::Grade_1:
        col = { 1.0f, 1.0f, 1.0f, 1.0f };  break;
    case HummerGrade::Grade_2:
        col = { 1.0f, 0.2f, 0.6f, 1.0f };  break;
    case HummerGrade::Grade_3:
        col = { 1.0f, 0.85f, 0.0f, 1.0f }; break;
    }

    gUi[Gauge::Base]->SetColor(col);

}

void GlassGauge::GaugeEffect()
{
    if (!efPlaying) return;

    constexpr int Interval = 2;        // 2フレームごとに次の画像へ
    constexpr int frameMax = 32;       // 8×4 の 32枚

    efCounter++;

    if (efCounter >= Interval)
    {
        efCounter = 0;
        efFrame++;

        if (efFrame >= frameMax)
        {
            efPlaying = false;
            return;
        }
    }


    //UV 設定
    int x = efFrame % 8;
    int y = efFrame / 8;

    float u = 1.f / 8.f;
    float v = 1.f / 4.f;

    gUi[Gauge::UpEffect]->SetUVPos({ u * x, v * y });
    gUi[Gauge::UpEffect]->SetUVScale({ u, v });

}
