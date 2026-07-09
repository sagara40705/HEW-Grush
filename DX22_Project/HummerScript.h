// === File: HummerScript.h ===
#pragma once
#include "CollisionListener.h"
#include "GameObject.h"
#include "Vector3.h"

enum class HummerGrade : int
{
	Grade_1 = 0,
	Grade_2 = 1,
	Grade_3 = 2,
	Count = 3
};
constexpr int ToInt(HummerGrade g) {return static_cast<int>(g);}


// グレードごとのゲージの最大値
constexpr float kMaxGaugeValue[ToInt(HummerGrade::Count)] = {10.f, 20.f, 30.f};

// デフォルトのハンマーの大きさ
constexpr Vector3 kDefaultHummerScale = Vector3(0.8f, 0.4f, 0.4f);
// グレードごとの大きさ(倍率)
constexpr float kHummerScaleRatio[ToInt(HummerGrade::Count)] = { 1.f, 1.5f, 2.25f };

// グレードUp時に貰えるゲージの最低値(全体の割合)
constexpr float KGradeUpGaugeRatio = 0.2f;

class Transform; class Collider; //前方宣言

class  HummerScript :public MonoBehaviour, public TriggerListener
{
public:
	// ===== チューニング（通常攻撃） =====
	Vector3 attackStartLocal = Vector3(-1, 0, 1.5); // 「攻撃開始地点」
	Vector3 attackEndLocal = Vector3(1, 0, 1.5); // 「攻撃終了地点」
	Vector3 idleLocal = Vector3(0, 0, 1.5); // 待機位置（子として前方1）
	float motionTime = 0.3f; // 「モーション時間(0.3)」
	// ===== ゲージ減衰 =====
	const float kGaugeDecayValue = 0.01f;
private:
	Transform* t = nullptr;
	Transform* owner = nullptr;
	Collider* col = nullptr;
public:
	HummerGrade grade = HummerGrade::Grade_1;
	float gauge = 0.f;
private:
	bool  isGaugeDecaying = false;
	static constexpr float kGaugeDecayDelay = 5.f;
	float timerGaugeDecayDelay = 0.f;

	// === 通常攻撃のステート ===
	bool  normalAttacking = false;
	float normalTimer = 0.f;

	// === 当たり判定の有効化管理 ===
	bool internalHitEnable = false; // 自分（通常攻撃）でのON
	bool externalHitEnable = false; // プレイヤーの他アクション（回転/ダッシュ）でのON
public:
	HummerScript(Transform* _o): owner(_o)  {}

	void Start() override;
	void Update() override;
	void OnDestroy() override;
	void OnTriggerStay(const TriggerInfo& c) override;
public:
	// === 追加API（プレイヤー側から使う） ===
	void StartNormalAttack();     // 通常攻撃を開始（クールタイム＝motionTime。多重起動ガードあり）
	bool IsNormalAttacking() const { return normalAttacking; }

	// プレイヤーの“別アクション”（回転攻撃/ダッシュ）中の当たり判定ON/OFF（OR結合）
	void SetActionHitEnable(bool on) { externalHitEnable = on; }
	// 実効（内部攻撃 or 外部アクション）での当たり判定ONか
	bool IsHitActive() const { return (internalHitEnable || externalHitEnable); }

	// 任意で見た目/コライダーON/OFF切り替えしたい場合のフック
	//void SetVisualEnabled(bool on);

private:
	bool CheckGrade(GameObject* glass);	// グレードを比較
	void GaugeUp(GameObject* glass);	// ゲージをUP
	void GradeUp();						// グレードをUP
	void GaugeDecay();					// 時間経過でゲージを減衰
	void GradeDown();					// グレードをDOWN
	void ScaleByGrade();				// 大きさを更新
	// 内部ヘルパ
	void SetInternalHitEnable(bool on) { internalHitEnable = on; }
	void TickNormalAttack(float dt);    // 位置補間
public:
	float GetGaugeRatio();			//ゲージの割合を取得
};