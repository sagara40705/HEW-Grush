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

class Transform;
class  HummerScript :public MonoBehaviour, public TriggerListener
{
private:
	Transform* t = nullptr;
	Transform* owner = nullptr;
	HummerGrade grade = HummerGrade::Grade_1;
	float gauge = 0.f;

	bool  isGaugeDecaying = false;
	static constexpr float kGaugeDecayDelay = 5.f;
	float timerGaugeDecayDelay = 0.f;
public:
	HummerScript(Transform* _o): owner(_o)  {}

	void Start() override;
	void Update() override;
	void OnDestroy() override;
	void OnTriggerEnter(const TriggerInfo& c) override;

private:
	bool CheckGrade(GameObject* glass);	// グレードを比較
	void GaugeUp(GameObject* glass);	// ゲージをUP
	void GradeUp();						// グレードをUP
	void GaugeDecay();					// 時間経過でゲージを減衰
	void GradeDown();					// グレードをDOWN
	void ScaleByGrade();				// 大きさを更新
public:
	float GetGaugeRatio();			//ゲージの割合を取得
};