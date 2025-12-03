#pragma once
#include "Component.h"

class PlayerState;

class PlayerAction : public MonoBehaviour
{
private:
	PlayerState* state_ = nullptr;
public:
	void Start() override;
	void Update() override;
	void OnDestroy() override;
private:
	void InputWantAction();
	void CheckCanAction();
	void UpdateAction();
	void UpdateActionCT();
private:
	void UpdateNormalAction();
	void UpdateSpinAction();
	void UpdateDashAction();
};