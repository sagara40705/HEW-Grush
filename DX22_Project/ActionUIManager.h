#pragma once
#include "CanvasUI.h"

class PlayerControll;

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
};