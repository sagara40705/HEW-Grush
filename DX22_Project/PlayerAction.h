// === File: PlayerAction.h ===
#pragma once
#include "Component.h"
#include "Vector3.h"

// 前方宣言
class Transform;class Rigidbody;class HummerScript;class PlayerMove;

class PlayerAction final : public MonoBehaviour
{
public:
	// ===== 調整値 =====
	// ダッシュ：地面＆移動中（停止中は不可）
	float dashDuration = 1.0f; // 秒
	float dashStartSpeed = 20.0f; // 初速
	float dashEndSpeed = 1.0f; // 終端
	float dashCooldown = 3.0f; // クールタイム（秒）
	float minDashMoveEps = 0.05f; // “移動中”判定の水平速度閾値

	// 回転攻撃（R）
	float spinCooldown = 3.0f;  // 秒
	float spinRotSpeed = 1440.0f;// deg/s
	float spinRotate = 720.0f;

	// 通常攻撃（E）
	float normalMotionTime = 0.5f; // クールタイム = モーション時間


public:
	void Start() override;
	void Update() override;
	void OnDestroy() override;

	// 状態参照
	bool  IsDashing()  const { return isDash_; }
	bool  IsSpinning() const { return spinning_; }


private:
	// 依存
	Transform* t_ = nullptr;
	Rigidbody* rb_ = nullptr;
	PlayerMove* move_ = nullptr;
	HummerScript* hummer_ = nullptr;

	// ダッシュ
	bool    isDash_ = false;
	Vector3 dashDir_ = Vector3::zero();
	float   dashTime_ = 0.0f;
	float   dashCooldownTimer_ = 0.0f;

	// 回転攻撃
	bool  spinning_ = false;
	float spinAccumDeg_ = 0.0f;
	float spinCooldownTimer_ = 0.0f;

	// 通常攻撃
	float normalCooldownTimer_ = 0.0f;
public:
	float GetDashCtRatio() { return (dashCooldownTimer_ / dashCooldown); }
	float GetSpinCtRatio() { return (spinCooldownTimer_ / spinCooldown); }

private:
	void DashTryStart();
	void DashUpdate(float dt);
	void SpinningUpdate(float dt);
	void NormalAttackTryStart();
	void UpdateHummerMask();
	void AcquireHummer();
};