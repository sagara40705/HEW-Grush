#pragma once
#include "CollisionListener.h"
#include "GameObject.h"

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

class Transform;
class  HummerScript :public MonoBehaviour, public TriggerListener
{
public:
	Transform* t = nullptr;
	Transform* owner = nullptr;
	float rotate = 0.0f;
	HummerGrade grade = HummerGrade::Grade_1;
	float gauge = 0.f;

	HummerScript(Transform* _o): owner(_o)  {}

	void Start() override;
	//void Update() override;
	void OnDestroy() override;
	void OnTriggerEnter(const TriggerInfo& c) override;

private:
	bool CheckGrade(GameObject* glass);	// グレードを比較
	void GaugeUp(GameObject* glass);	// ゲージをUP
	void GradeUp();						// グレードをUP
public:
	float GetGaugeRatio();			//ゲージの割合を取得
};