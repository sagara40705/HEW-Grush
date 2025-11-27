#include "SpriteAnimation.h"
#include "CanvasUI.h"

void SpriteAnimation::Start()
{
	SetSpriteUV();

	dx = 1 / columns_;
	dy = 1 / rows_;
	Vector2 uvSize = { dx , dy };
	ui_->SetUVScale(uvSize);
}

void SpriteAnimation::OnDestroy()
{
}

void SpriteAnimation::Update()
{
	constexpr float fixedDt = 1.0f / 60.0f;

	timer_ += fixedDt;

	if (timer_ >= speed_)
	{
		timer_ = 0.0f;

		++currentFrame_;
		if (currentFrame_ >= loopFrame_) currentFrame_ = 0;

		SetSpriteUV();
	}
}

void SpriteAnimation::SetSpriteUV()
{
	float ix = currentFrame_ / columns_;
	float iy = currentFrame_ % columns_;
	
	Vector2 uvPos = { ix * dx , iy * dy };

	ui_->SetUVPos(uvPos);
}
