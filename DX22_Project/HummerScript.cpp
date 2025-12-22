// === File: HummerScript.cpp ===
#include "HummerScript.h"
#include "Transform.h"
#include "Collider.h" 
#include "GameObject.h"
#include "Input.h"
#include <algorithm>
#include <cmath>
#include "DebugLog.h"
#include "GlassScript.h"
#include "CameraShake.h"
#include "PlayerAction.h"
#include "Game.h"
#include "Vector2.h"
#include <fstream>
#include <sstream>
#include <iostream>


// ここで定数を定義（ヘッダー側からは削除済み）
constexpr Vector3 kDefaultHummerScale = Vector3(0.8f, 0.4f, 0.4f);
constexpr float kDebugGaugeUpValue = 1.0f;

static inline Vector3 Lerp(const Vector3& a, const Vector3& b, float t)
{
	return a + (b - a) * std::clamp(t, 0.0f, 1.0f);
}

void HummerScript::Start()
{
	t = gameObject->TryGetComponent<Transform>();
	col = gameObject->TryGetComponent<Collider>();
	assert(t && "HummerScript requires Transform");

	// CSV読み込み
	LoadParamsFromCSV("HummerParams.csv");

	// === PlayerActionの取得 ===
	for (auto& o : Game::GetWorld().objects)
	{
		if (!o->active) continue;
		if (!o->CompareTag(Tag::Player)) continue;
		pa = o->TryGetComponent<PlayerAction>();
		if (pa) break;
	}

	SetInternalHitEnable(false);

	// 初期サイズ適用
	ScaleByGrade();
}

void HummerScript::LoadParamsFromCSV(const std::string& filename)
{
	// デフォルト値の設定
	gradeParams.resize(ToInt(HummerGrade::Count));
	gradeParams[0] = { 10.0f, 1.0f, 2.0f, 0.25f, 0.6f };
	gradeParams[1] = { 20.0f, 1.5f, 4.0f, 0.35f, 0.8f };
	gradeParams[2] = { 30.0f, 2.25f, 6.0f, 0.5f, 1.0f };

	// グローバルパラメータのデフォルト
	loadedGaugeDecayValue = 0.01f;
	loadedGradeUpRatio = 0.2f;
	loadedGaugeDecayDelay = 5.0f;

	std::ifstream file(filename);
	if (!file.is_open())
	{
		Debug::Log("Error: Failed to open CSV file. Using defaults.");
		return;
	}

	std::string line;
	// ヘッダー行をスキップ
	std::getline(file, line);

	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string token;
		std::vector<std::string> columns;

		while (std::getline(ss, token, ','))
		{
			columns.push_back(token);
		}

		if (columns.empty()) continue;

		if (columns[0] == "Global")
		{
			if (columns.size() >= 5)
			{
				loadedGaugeDecayValue = std::stof(columns[2]);
				loadedGradeUpRatio = std::stof(columns[3]);
				loadedGaugeDecayDelay = std::stof(columns[4]);
			}
		}
		else if (columns[0] == "Data")
		{
			if (columns.size() >= 7)
			{
				int gIndex = std::stoi(columns[1]);

				if (gIndex >= 0 && gIndex < ToInt(HummerGrade::Count))
				{
					HummerGradeParam param;
					param.maxGauge = std::stof(columns[2]);
					param.scaleRatio = std::stof(columns[3]);
					param.gaugeUpValue = std::stof(columns[4]);
					param.vibPower = std::stof(columns[5]);
					param.vibDuration = std::stof(columns[6]);

					gradeParams[gIndex] = param;
				}
			}
		}
	}
	file.close();
}


void HummerScript::Update()
{
	constexpr float DT = 1.0f / 60.0f;

	if (normalAttacking) TickNormalAttack(DT);

	const bool nowHit = IsHitActive();
	if (col) col->isTrigger = true;

	if (isGaugeDecaying)
	{
		GaugeDecay();
	}
	else
	{
		timerGaugeDecayDelay += DT;
		if (timerGaugeDecayDelay >= loadedGaugeDecayDelay)
		{
			isGaugeDecaying = true;
			timerGaugeDecayDelay = 0.f;
			Debug::Log(" HummerScript::Update - StartGaugeDecay");
		}
	}

#ifdef _DEBUG
	DebugCommand();
#endif
}

void HummerScript::OnDestroy()
{
	t = nullptr;
	owner = nullptr;
	col = nullptr;
	pa = nullptr;
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

			// ハンマー自身のグレードによる振動
			int gIdx = ToInt(grade);
			if (gIdx < gradeParams.size())
			{
				pa->PlayHitVibration(gradeParams[gIdx].vibPower, gradeParams[gIdx].vibDuration);
			}

			CameraShake::StartShake(static_cast<CameraShake::Level>(static_cast<int>(grade)));

			GlassScript* gs = c.other->TryGetComponent<GlassScript>();
			gs->SpawnShards();

			// ガラス側のグレードによる振動（必要なら）
			int glassGradeIdx = ToInt(gs->grade);
			if (glassGradeIdx < gradeParams.size())
			{
				pa->PlayHitVibration(gradeParams[glassGradeIdx].vibPower, gradeParams[glassGradeIdx].vibDuration);
			}

			GaugeUp(c.other);

			isGaugeDecaying = false;
			timerGaugeDecayDelay = 0.f;
		}
	}
}

void HummerScript::StartNormalAttack()
{
	if (normalAttacking) return;
	normalAttacking = true;
	normalTimer = 0.f;
	SetInternalHitEnable(true);
	if (t) t->SetPosition(attackStartLocal);
}

bool HummerScript::CheckGrade(GameObject* glass)
{
	GlassScript* gs = glass->TryGetComponent<GlassScript>();
	if (!gs) return false;
	return (ToInt(gs->grade) <= ToInt(grade));
}

void HummerScript::GaugeUp(GameObject* glass)
{
	GlassScript* gs = glass->TryGetComponent<GlassScript>();
	if (!gs) return;

	int glassIdx = ToInt(gs->grade);
	int currentIdx = ToInt(grade);

	if (glassIdx < gradeParams.size())
	{
		gauge += gradeParams[glassIdx].gaugeUpValue;
	}

	Debug::LogValue(" HummerScript::gaugeratio", GetGaugeRatio());

	if (currentIdx < gradeParams.size() && gauge >= gradeParams[currentIdx].maxGauge)
	{
		if (grade == HummerGrade::Grade_3)
		{
			gauge = gradeParams[ToInt(HummerGrade::Grade_3)].maxGauge;
		}
		else
		{
			GradeUp();
		}
	}
}

void HummerScript::GradeUp()
{
	grade = static_cast<HummerGrade>(ToInt(grade) + 1);
	int newIdx = ToInt(grade);

	if (newIdx < gradeParams.size())
	{
		gauge = gradeParams[newIdx].maxGauge * loadedGradeUpRatio;
	}

	Debug::LogValue(" HummerScript::gradelup", ToInt(grade) + 1);
	ScaleByGrade();
}

void HummerScript::GaugeDecay()
{
	gauge -= loadedGaugeDecayValue;

	if (gauge < 0.f)
	{
		if (grade == HummerGrade::Grade_1) gauge = 0.f;
		else GradeDown();
	}
}

void HummerScript::GradeDown()
{
	grade = static_cast<HummerGrade>(ToInt(grade) - 1);
	int newIdx = ToInt(grade);

	if (newIdx < gradeParams.size())
	{
		gauge = gradeParams[newIdx].maxGauge;
	}

	Debug::LogValue(" HummerScript::gradelup", ToInt(grade) + 1);
	ScaleByGrade();
}

void HummerScript::ScaleByGrade()
{
	int idx = ToInt(grade);
	if (idx < gradeParams.size())
	{
		t->SetScale(kDefaultHummerScale * gradeParams[idx].scaleRatio);
	}
}

void HummerScript::TickNormalAttack(float dt)
{
	normalTimer += dt;
	const float T = std::max(motionTime, 1e-6f);
	const float r = std::min(normalTimer / T, 1.0f);
	if (t) t->SetPosition(Lerp(attackStartLocal, attackEndLocal, r));

	if (normalTimer >= T)
	{
		normalAttacking = false;
		normalTimer = 0.f;
		SetInternalHitEnable(false);
		if (t) t->SetPosition(idleLocal);
	}
}

void HummerScript::DebugCommand()
{
	int idx = ToInt(grade);
	if (idx >= gradeParams.size()) return;

	if (IsKeyPress('G') && IsKeyPress('W'))
	{
		gauge += kDebugGaugeUpValue;
		// 修正: kMaxGaugeValue 配列ではなく gradeParams を使用
		if (gauge >= gradeParams[idx].maxGauge)
		{
			if (grade == HummerGrade::Grade_3)
			{
				gauge = gradeParams[ToInt(HummerGrade::Grade_3)].maxGauge;
			}
			else
			{
				GradeUp();
			}
		}
	}
	if (IsKeyPress('G') && IsKeyPress('S'))
	{
		gauge -= kDebugGaugeUpValue;
	}

	if (IsKeyPress('H'))
	{
		CameraShake::StartShake(static_cast<CameraShake::Level>(static_cast<int>(grade)));
	}
}

float HummerScript::GetGaugeRatio()
{
	int idx = ToInt(grade);
	// 修正: idx >= size は範囲外なので、idx < size に修正
	if (idx < gradeParams.size() && gradeParams[idx].maxGauge > 0.0f)
	{
		return (gauge / gradeParams[idx].maxGauge);
	}
	return 0.0f;
}