#include "LoadingGauge.h"
#include "LoadingScene.h"
#include "GameObject.h"
#include "UiSpriteRenderer.h"

LoadingGauge::LoadingGauge(LoadingScene* loading) : loading_(loading)
{
}

void LoadingGauge::Start()
{
	ui_ = gameObject->GetComponent<UiSpriteRenderer>();
}

void LoadingGauge::Update()
{
	float prog = loading_->GetProgress();

	Vector2 size = ui_->GetSize();

	ui_->SetSize(size);
}

void LoadingGauge::OnDestroy()
{

}
