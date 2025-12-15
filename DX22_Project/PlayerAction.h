#pragma once
#include "Component.h"
#include "Scene.h"

class PlayerState; class Hammer;

class PlayerAction : public MonoBehaviour,Scene
{
private:
	Transform* tf_ = nullptr;
	PlayerState* state_ = nullptr;
	Hammer* hammer_ = nullptr;
	GameObject* Effkseer = nullptr;
	SpriteAnimation* m_hitEffekt = nullptr;

public:
	PlayerAction(GameObject* Effkt);
	void Start() override;
	void Update() override;
	void OnDestroy() override;
	
private:
	void InputWantAction();
	void CheckCanAction();
	void UpdateAction();
	void UpdateActionCT();
	// アニメーション状態
	int startFrame = 0;
private:
	void SetHammerActive(bool v);
	void UpdateNormalAttack();
	void UpdateSpinAttack();
	void UpdateDashAttack();
public:
	void StopNormalAttack();
	void SetStartFrame(int start) { startFrame = start; }
};