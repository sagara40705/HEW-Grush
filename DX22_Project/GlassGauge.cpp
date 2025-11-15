#include "GlassGauge.h"
#include "GameObject.h"
#include "HummerScript.h"
#include "CanvasUI.h"

void GlassGauge::Start()
{
	gauge = Parent->TryGetComponent<CanvasUI>();

}

void GlassGauge::OnDestroy()
{
	hs = nullptr;
	gauge = nullptr;
}

void GlassGauge::Update()
{
	if (!hs || !gauge) return;

	float r = hs->GetGaugeRatio();
	
	gauge->SetPosY(200 * (1 - r));
	gauge->SetSize({ 1 , r });
	gauge->SetUVPos({ 0 , 1 - r });
	gauge->SetUVScale({ 1 , r });

    HummerGrade g = hs->grade;    
    DirectX::XMFLOAT4 col;

    switch (g)
    {
    case HummerGrade::Grade_1:
        col = { 1.0f, 1.0f, 1.0f, 1.0f };  
        break;
    case HummerGrade::Grade_2:
        col = { 1.0f, 0.2f, 0.6f, 1.0f };   
        break;
    case HummerGrade::Grade_3:
        col = { 1.0f, 0.85f, 0.0f, 1.0f }; 
        break;
    }

    gauge->SetColor(col);

}
