#pragma once
#include "CanvasUI.h"

// ‘O•ûéŒ¾
class PlayerControll; class CanvasUI;

class ActionUIManager : public MonoBehaviour
{
private:
	enum Action
	{
		Spining,
		Dash,
		Count
	};
private:
	CanvasUI* ui[Action::Count]{};
	PlayerControll* pc = nullptr;
public:
	ActionUIManager(CanvasUI* spin, CanvasUI* dash);
	void Start() override;
	void OnDestroy() override;
	void Update() override;
private:
	const Vector2 kNumberPos[Action::Count] = { {900.f, 520.f},{1050.f, 520.f} };	//ˆÊ’u(l‚Â•ª)
	const Vector2 kNumberSize = { 0.25f, 0.25f };									//k¬—¦
	const Vector2 kNumberUvPos = {0.f, 0.f};	
	const Vector2 kNumberUVSize = { 1.f, 1.f };										//UVSize
};