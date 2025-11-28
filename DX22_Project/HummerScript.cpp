#include "HummerScript.h"
#include "Transform.h"
#include "Collider.h" 
#include "GameObject.h"
#include "Input.h"
#include <algorithm>
#include <cmath>
#include "DebugLog.h"
#include "GlassScript.h"

static inline Vector3 Lerp(const Vector3& a, const Vector3& b, float t)
{
	return a + (b - a) * std::clamp(t, 0.0f, 1.0f);
}

void HummerScript::Start()
{
	t = Parent->TryGetComponent<Transform>();
	col = Parent->TryGetComponent<Collider>();
	assert(t && "HummerScript requires Transform");

	// 当たり判定は標準OFF（プレイヤーのアクション時のみON）
	SetInternalHitEnable(false);
}

void HummerScript::Update()
{
	constexpr float DT = 1.0f / 60.0f;

	// 通常攻撃の進行
	if (normalAttacking) TickNormalAttack(DT);

	const bool nowHit = IsHitActive();
	if (col) col->isTrigger = true;

	if (isGaugeDecaying)
	{
		GaugeDecay();
	}
	else 
	{
		// タイマー更新
		// ** 仮実装 **
		timerGaugeDecayDelay += DT;

		if (timerGaugeDecayDelay >= kGaugeDecayDelay)
		{
			isGaugeDecaying = true;
			timerGaugeDecayDelay = 0.f;
			Debug::Log(" HummerScript::Update - StartGaugeDecay");
		}
	}
}

void HummerScript::OnDestroy()
{
	t = nullptr;
	owner = nullptr;
	col = nullptr;
}

void HummerScript::OnTriggerStay(const TriggerInfo& c)
{
	if (!IsHitActive()) return;

	if (c.other)
	{
		if (!c.other->CompareTag(Tag::Glass)) return;

		if (CheckGrade(c.other))
		{
			c.other->Destroy();
			//Debug::LogValue(" HummerScript::Destroy id", c.other->id);
			GaugeUp(c.other);
			
			// ゲージ減衰中なら止める & Delayタイマーを初期化
			if (isGaugeDecaying)
			{
				isGaugeDecaying = false;
				timerGaugeDecayDelay = 0.f;
			}
		}
	}
}

void HummerScript::StartNormalAttack()
{
	// 多重起動ガード（モーション時間中は再起動しない）
	if (normalAttacking) return;

	normalAttacking = true;
	normalTimer = 0.f;

	// 当たり判定ON（内部）
	SetInternalHitEnable(true);

	// 開始位置へスナップ（ローカル）
	if (t) t->SetPosition(attackStartLocal);

	// 見た目ONにしたい場合はここで
	//SetVisualEnabled(true);
}

bool HummerScript::CheckGrade(GameObject* glass)
{
	GlassScript* gs = glass->TryGetComponent<GlassScript>();
	if (!gs) return false;

	return (ToInt(gs->grade) <= ToInt(grade));
}

void HummerScript::GaugeUp(GameObject* glass)
{
	// 破壊したガラスオブジェクトのGlassScriptを取得
	GlassScript* gs = glass->TryGetComponent<GlassScript>();
	if (!gs) return;

	// ゲージを上昇
	gauge += kGaugeUpValue[ToInt(gs->grade)];
	Debug::LogValue(" HummerScript::gaugeratio", GetGaugeRatio());

	// ゲージの最大値を超えたら
	if (gauge >= kMaxGaugeValue[ToInt(grade)])
	{
		// 1・2 → グレードUP , 3 → 最大値の正規化
		if (grade == HummerGrade::Grade_3) gauge = kMaxGaugeValue[ToInt(HummerGrade::Grade_3)];
		else GradeUp();
	}
}

void HummerScript::GradeUp()
{
	grade = static_cast<HummerGrade>(ToInt(grade) + 1);
	gauge = 0.f;
	Debug::LogValue(" HummerScript::gradelup", ToInt(grade) + 1);
	ScaleByGrade();
}

void HummerScript::GaugeDecay()
{
	// ゲージをマイナス
	gauge -= kGaugeDecayValue;

	// ゲージのマイナスを超えたら
	if (gauge < 0.f)
	{
		// 1→ 0に正規化 , 3・2  → グレードDOWN
		if (grade == HummerGrade::Grade_1) gauge = 0.f;
		else GradeDown();
	}
}

void HummerScript::GradeDown()
{
	grade = static_cast<HummerGrade>(ToInt(grade) - 1);
	gauge = kMaxGaugeValue[ToInt(grade)];
	Debug::LogValue(" HummerScript::gradelup", ToInt(grade) + 1);
	ScaleByGrade();
}

void HummerScript::ScaleByGrade()
{
	t->SetScale(kDefaultHummerScale * kHummerScaleRatio[ToInt(grade)]);
}

void HummerScript::TickNormalAttack(float dt)
{
	normalTimer += dt;
	const float T = std::max(motionTime, 1e-6f);
	const float r = std::min(normalTimer / T, 1.0f);

	// 線形補間で「開始→終了」へ移動（ローカル）
	if (t) t->SetPosition(Lerp(attackStartLocal, attackEndLocal, r));

	if (normalTimer >= T)
	{
		// 完了
		normalAttacking = false;
		normalTimer = 0.f;

		// 内部の当たり判定OFF（外部からのONは維持）
		SetInternalHitEnable(false);

		// 待機位置へ戻す（仕様に合わせたい場合は任意）
		if (t) t->SetPosition(idleLocal);
	}
}

float HummerScript::GetGaugeRatio()
{
	return (gauge / kMaxGaugeValue[ToInt(grade)]);
}


