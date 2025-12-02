#pragma once
#include "Component.h"
#include "Raycast.h"

class PlayerState;

class PlayerMove : public MonoBehaviour
{
private:
	Transform* playerTF_ = nullptr;
	Transform* camTF_ = nullptr;
	Rigidbody* rb_ = nullptr;
	PlayerState* state_ = nullptr;
	Animator* animator_ = nullptr;
public:
	void Start() override;
	void Update() override;
	void OnDestroy() override;
private:
	//Ú’n”»’è
	void CheckGround();
	Ray GetDownwordRay();
	//ƒWƒƒƒ“ƒv
	void Jump();
	//ˆÚ“®
	void InputMoveAxis();
	void ApplyMoveAxis();
	void AlignRotationToCamera();
};