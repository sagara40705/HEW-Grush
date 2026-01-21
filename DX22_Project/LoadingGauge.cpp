#include "LoadingGauge.h"
#include "LoadingScene.h"
#include "GameObject.h"
#include "UiSpriteRenderer.h"
#include "Transform.h"


LoadingGauge::LoadingGauge(LoadingScene* loading) : loading_(loading)
{
	
}

void LoadingGauge::Start()
{

	ui_ = gameObject->GetComponent<UiSpriteRenderer>();
	if (ui_) fullSize_ = ui_->GetSize();	

	if(gameObject && gameObject->transform)
	{
		baseCenterPos_ = ui_->GetOffset();
	}
}

void LoadingGauge::Update()
{
	if (!loading_ || !ui_ || !gameObject || !gameObject->transform) return;

	// 進捗率取得
	float p = loading_->GetProgress();
	if (p < 0.0f) p = 0.0f;
	if (p > 1.0f) p = 1.0f;

	// 横幅を更新
	Vector2 size = fullSize_;
	size.x = 1280.f * p;
	ui_->SetSize(size);



	// 左端固定にするために中心座標を補正
	// 初期状態の中心X(baseCenterPos_.x)から、縮んだ分の半分だけ左へ寄せる
	// newCenterX = baseCenterX - (fullW - newW) * 0.5
	//Vector2 pos = baseCenterPos_;
	//pos.x = baseCenterPos_.x - (fullSize_.x - size.x) * 0.5f;
	//ui_->SetOffset(pos);
}

void LoadingGauge::OnDestroy()
{

}
