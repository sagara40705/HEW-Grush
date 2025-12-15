#pragma once
#include "Component.h"
#include "Scene.h"

class PlayerState; class Hammer;

class PlayerAction : public MonoBehaviour
{
private:
	Transform* tf_ = nullptr;
	PlayerState* state_ = nullptr;
	Hammer* hammer_ = nullptr;

	GameObject* Effkseer = nullptr;
	SpriteAnimation* m_hitEffekt = nullptr;

public:
	PlayerAction();  // ←コンストラクタで情報を受け取る形にしない
	void Awake() override;
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