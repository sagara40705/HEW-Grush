#include "PlayerMove.h"
#include "GameObject.h"
#include "Rigidbody.h"
#include "PlayerState.h"
#include "PlayerAction.h"
#include "PhysicsSystem.h"
#include "InputKeyboard.h"
#include "Animator.h"
#include "ModelManager.h"
#include <cmath>

void PlayerMove::Start()
{
	playerTF_ = gameObject->GetComponent<Transform>();

	GameObject* camPivot = gameObject->FindWithTag(Tag::CamreraPivot);
	camTF_ = camPivot->GetComponent<Transform>();

	rb_ = gameObject->GetComponent<Rigidbody>();
	state_ = gameObject->GetComponent<PlayerState>();
	animator_ = gameObject->GetComponent<Animator>();
}

void PlayerMove::Update()
{
	CheckGround();
	Jump();
	InputMoveAxis();
	ApplyMoveAxis();
}

void PlayerMove::OnDestroy()
{
	animator_ = nullptr;
	state_ = nullptr;
	rb_ = nullptr;
	camTF_ = nullptr;
}

void PlayerMove::CheckGround()
{
	constexpr float kGroundCheckDistance = 0.01f;

	Ray ray = GetDownwordRay();
	RaycastHit hit;
	//実装希望

	state_->isGrounded
		= Physics::Raycast(*(gameObject->world), 
			ray, &hit, kGroundCheckDistance, ToBit(Layer::Ground));
}

Ray PlayerMove::GetDownwordRay()
{
	Vector3 pos = playerTF_->GetPosition() - Vector3(0.f, PlayerState::kRadius, 0.f);
	constexpr Vector3 down = Vector3::down();

	return { pos , down };
}

void PlayerMove::Jump()
{
	constexpr float JumpForce = 5.0f;
	constexpr int JumpKey = VK_SPACE;

	if (!state_->isGrounded) return;

	if (InputKeyboard::GetKeyDown(JumpKey))
	{
		rb_->AddForce(Vector3(0.f, JumpForce, 0.f));
	}
}


//WASDとRステックで前後左右移動
void PlayerMove::InputMoveAxis()
{
	if (state_->usingAction != ActionId::None) return;

	Vector2 axis = Vector2::zero();

	// --- キーボード WASD ---
	if (InputKeyboard::GetKey('W')) axis.y += 1.f;
	if (InputKeyboard::GetKey('S')) axis.y -= 1.f;
	if (InputKeyboard::GetKey('D')) axis.x += 1.f;
	if (InputKeyboard::GetKey('A')) axis.x -= 1.f;

	// 長さ 1 を超える場合は正規化
	if (axis.Length() > 1.f) axis = axis.Normalized();

	state_->moveAxis = axis;
}

void PlayerMove::ApplyMoveAxis()
{
	if (!state_->canMove || !state_->canControl) return;

	AlignRotationToCamera();

	Vector2 axis = state_->moveAxis;
	if (axis.Length() <= 0.01f)
	{
		// 移動なし
		rb_->velocity.x = 0.f;
		rb_->velocity.z = 0.f;
		return;
	}

	// カメラ基準で 3D 方向へ変換
	Vector3 camForward = Vector3(camTF_->Forward().x, 0.f, camTF_->Forward().z);
	camForward.Normalized();

	Vector3 camRight = Vector3(camTF_->Right().x, 0.f, camTF_->Right().z);
	camRight.Normalized();

	Vector3 move = camForward * axis.y + camRight * axis.x;

	move.Normalize();

	constexpr float moveSpeed = 4.0f;

	// Y 速度は維持（ジャンプや重力用）
	float vy = rb_->velocity.y;

	rb_->velocity = Vector3(move.x * moveSpeed, vy, move.z * moveSpeed);
}

//移動時カメラ方向と向きを同期させる
void PlayerMove::AlignRotationToCamera()
{
	if (!playerTF_ || !camTF_) return;

	// 入力がほぼゼロなら向きを変えない
	Vector2 axis = state_->moveAxis;
	if (axis.Length() <= 0.01f)
	{
		return;
	}

	// ApplyMoveAxis() と同じ計算で「移動方向」を求める
	Vector3 camForward = Vector3(camTF_->Forward().x, 0.f, camTF_->Forward().z);
	Vector3 camRight = Vector3(camTF_->Right().x, 0.f, camTF_->Right().z);

	camForward.Normalize();
	camRight.Normalize();

	// WASD（縦＝axis.y, 横＝axis.x）をカメラ基準で 3D ベクトルに
	Vector3 moveDir = camForward * axis.y + camRight * axis.x;

	// 水平ベクトルだけ使う（Y は 0 にしておく）
	moveDir.y = 0.f;

	// 長さがほぼゼロなら何もしない
	if (moveDir.magnitude() <= 1e-6f)
	{
		return;
	}

	moveDir.Normalize();
	moveDir *= -1.0f;

	// 移動ベクトルから yaw（Y 回転）を計算
	// Z+ 方向を 0 度として X+ 方向に行くほどプラス
	float yawRad = std::atan2(moveDir.x, moveDir.z);
	constexpr float Rad2Deg = 180.0f / 3.14159265358979323846f;
	float yawDeg = yawRad * Rad2Deg;

	// プレイヤーの Y 回転だけを移動方向に合わせる
	Vector3 rot = playerTF_->GetRotation(); // (pitch, yaw, roll) = (x,y,z) [度]
	rot.y = yawDeg;
	playerTF_->SetRotation(rot);
}
