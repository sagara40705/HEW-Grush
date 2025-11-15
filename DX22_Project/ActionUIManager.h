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
};