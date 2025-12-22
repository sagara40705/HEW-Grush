// === File: HummerScript.h ===
#pragma once
#include "CollisionListener.h"
#include "GameObject.h"
#include "Vector3.h"
#include <vector>
#include <string>

enum class HummerGrade : int
{
	Grade_1 = 0,
	Grade_2 = 1,
	Grade_3 = 2,
	Count = 3
};
constexpr int ToInt(HummerGrade g) { return static_cast<int>(g); }

class Transform; class Collider; // 前方宣言

// グレードごとのパラメータ構造体
struct HummerGradeParam
{
	float maxGauge;      // ゲージ最大値
	float scaleRatio;    // 大きさ倍率
	float gaugeUpValue;  // このグレードのガラスを割った時のゲージ上昇量
	float vibPower;      // 振動の強さ
	float vibDuration;   // 振動の時間
};

class  HummerScript :public MonoBehaviour, public TriggerListener
{
public:
	// ===== チューニング（通常攻撃） =====
	Vector3 attackStartLocal = Vector3(-1, 0, 1.5); // 「攻撃開始地点」
	Vector3 attackEndLocal = Vector3(1, 0, 1.5);    // 「攻撃終了地点」
	Vector3 idleLocal = Vector3(0, 0, 1.5);         // 待機位置
	float motionTime = 0.3f;                        // 「モーション時間」

private:
	Transform* t = nullptr;
	Transform* owner = nullptr;
	Collider* col = nullptr;
	PlayerAction* pa = nullptr;

	// CSVから読み込んだデータを保持するリスト
	std::vector<HummerGradeParam> gradeParams;

	// グローバル設定(CSVから読み込む)
	float loadedGaugeDecayValue = 0.01f;     // 減衰値
	float loadedGradeUpRatio = 0.2f;         // ランクアップ時の引継ぎ割合
	float loadedGaugeDecayDelay = 5.0f;      // 減衰開始までの遅延時間

public:
	HummerGrade grade = HummerGrade::Grade_1;
	float gauge = 0.f;

private:
	bool  isGaugeDecaying = false;
	float timerGaugeDecayDelay = 0.f;

	// === 通常攻撃のステート ===
	bool  normalAttacking = false;
	float normalTimer = 0.f;

	// === 当たり判定の有効化管理 ===
	bool internalHitEnable = false; // 自分（通常攻撃）でのON
	bool externalHitEnable = false; // プレイヤーの他アクションでのON

public:
	HummerScript(Transform* _o) : owner(_o) {}

	void Start() override;
	void Update() override;
	void OnDestroy() override;
	void OnTriggerStay(const TriggerInfo& c) override;

public:
	// === 追加API ===
	void StartNormalAttack();
	bool IsNormalAttacking() const { return normalAttacking; }

	// プレイヤーの“別アクション”中の当たり判定ON/OFF
	void SetActionHitEnable(bool on) { externalHitEnable = on; }
	// 実効当たり判定ONか
	bool IsHitActive() const { return (internalHitEnable || externalHitEnable); }

private:
	bool CheckGrade(GameObject* glass);
	void GaugeUp(GameObject* glass);
	void GradeUp();
	void GaugeDecay();
	void GradeDown();
	void ScaleByGrade();

	void SetInternalHitEnable(bool on) { internalHitEnable = on; }
	void TickNormalAttack(float dt);
	void DebugCommand();

	void LoadParamsFromCSV(const std::string& filename);

public:
	float GetGaugeRatio();
};