// === File: PlayerMove.h ===
#pragma once
#include "Component.h" // MonoBehaviour
#include "Vector3.h"

// 前方宣言
class Transform;class Rigidbody;class Collider;class PlayerAction; 

class PlayerMove final : public MonoBehaviour
{
public:
	// ===== 調整値 =====
	float MOVE_SPEED = 8.0f; // m/s
	float AIR_CONTROL = 0.5f; // 空中制御 0..1（落下中など）
	float GROUND_EPS = 0.10f; // 接地判定の許容
	float SMOOTH_ACC = 20.0f; // 速度追従（大きいほどキビキビ）
	// 入力から“向き”を決めるか（外部からロック可能）
	bool rotateToMove = true;

public:
	PlayerMove();
	~PlayerMove();
	void Start() override;
	void Update() override;
	void OnDestroy() override;

	// === 外部（PlayerAction）から利用する情報/制御 ===
	bool  IsGrounded() const;            // 接地
	float HorizontalSpeed() const;       // 水平速度|v|
	float HorizontalSpeedSq() const;     // |v|^2
	const Vector3& VelocityRef() const;  // 参照（読み取り専用想定）
	void  SetMoveOverride(bool on);      // true中は水平方向加速を行わない（例：ダッシュで上書き）
	void  SetRotateLock(bool on);        // true中は入力方向へ向けない（例：回転攻撃）
	void  AddExternalYaw(float deg);     // Transformを外部回転した分だけ自前yawを同期
	float FacingYawDeg() const { return facingYawDeg_; }


private:
	void ApplyHorizontal(float dt, bool grounded);
	void FaceToWish(const Vector3& wishDir);

private:
	Transform* t_ = nullptr;
	Transform* cam_ = nullptr;
	Rigidbody* rb_ = nullptr;
	Collider* col_ = nullptr;
	PlayerAction* pa_ = nullptr;

	float  facingYawDeg_ = 0.0f;
	bool   moveOverride_ = false; // ダッシュ等で速度を外部制御
	bool   rotateLock_ = false; // 回転攻撃等で“向かない”


};