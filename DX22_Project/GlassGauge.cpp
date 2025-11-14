#include "GlassGauge.h"
#include "GameObject.h"
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
	static float ox = 0.0f;
	ox += 0.1f;
	gauge->SetPosX( ox);
}
