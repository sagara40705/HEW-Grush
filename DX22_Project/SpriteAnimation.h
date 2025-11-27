#pragma once
#include "Component.h"
#include "Vector2.h"

class SpriteAnimation : public MonoBehaviour
{
private:
	CanvasUI* ui_ = nullptr;
public:
	int rows_ = 1;
	int columns_ = 1;
	int loopFrame_ = 1;
	float speed_ = 1.0f;
public:
	SpriteAnimation(CanvasUI* ui, int r, int c, int f, float s) : ui_(ui), rows_(r), columns_(c), loopFrame_(f), speed_(s) {}
	void Start() override;
	void OnDestroy() override;
	void Update() override;
private:
	int currentFrame_ = 0;
	float timer_ = 0.0f;
	void SetSpriteUV();
	float dx;
	float dy;
};