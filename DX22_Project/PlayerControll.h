// === File: PlayerControll.h ===
#pragma once
#include "CollisionListener.h"

class Transform;
class Rigidbody;
class Collider;

class PlayerControll final : public MonoBehaviour, public CollisionListener
{
public:
	// ====== チューニング用（必要に応じて外から書き換え可）======
	float MOVE_SPEED = 5.0f; // m/s
	float AIR_CONTROL = 0.5f; // 空中制御 0..1
	float JUMP_SPEED = 5.0f; // 上向き初速
	float GROUND_EPS = 0.10f; // 接地許容
	float SMOOTH_ACC = 20.0f; // XZ 追従加速

	// --- ダッシュ仕様（変数化） ---
	float dashDuration = 1.0f;   // 「ダッシュ時間」秒（線形減速に使用）
	float dashStartSpeed = 7.0f;   // 初速「5」
	float dashEndSpeed = 1.0f;   // 終了速度「2」
	float dashCooldown = 2.0f;   // クールタイム「５」秒
	float minDashMoveEps = 0.05f;  // “移動中”判定の閾値（水平速度）

	// --- 回転攻撃（例：Rキー） ---
	float spinCooldown = 2.0f;   // クールタイム「3」秒
	float spinRotSpeed = 1440.0f; // deg/s（最小実装）


public:
	void Start() override;
	void Update() override;
	void OnDestroy() override;
	void OnCollisionEnter(const CollisionInfo& c) override;

private:
	Transform* t = nullptr;
	Transform* camera_ = nullptr;
	Rigidbody* rb_ = nullptr;
	Collider* col_ = nullptr;
	Collider* hummer_ = nullptr;

	bool IsGrounded() const;
	void ApplyHorizontal(float dt, bool grounded);
	void ApplyJump(bool grounded);
	void OnScale();
	void Spinning(float dt);   // ← クールタイム対応
	void DashTryStart(bool grounded);
	void DashUpdate(float dt);
	void SetHummerActive(bool activ);

private:
	// --- アクション状態 ---
	bool isAttack = false;

	// ダッシュ中フラグ（仕様要件）
	bool isDash = false;

	// 入力/移動一時値
	Vector3 moveDir = Vector3::zero();

	// --- ダッシュ内部状態 ---
	Vector3 dashDir = Vector3::zero(); // 押下時の水平正規化方向
	float   dashTime = 0.0f;           // 経過時間

	// --- クールタイム管理 ---
	float dashCooldownTimer = 0.0f;
	float spinCooldownTimer = 0.0f;

	// --- 回転攻撃内部 ---
	bool  spinning = false;
	float spinAccumDeg = 0.0f;
};