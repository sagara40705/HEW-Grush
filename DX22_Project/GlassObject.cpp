#include "GlassObject.h"
#include "GameObject.h"
#include "Transform.h"
#include "GlassGaugeManager.h"
#include "ScoreManager.h"
#include "DebugLog.h"
#include "Prefab.h"
#include "PrefabSet.h"
#include "Rigidbody.h"
#include "ShaderList.h"
#include "GlassShard.h"
#include "GlassShardPool.h"
#include "UiSpriteRenderer.h"
#include "SpriteAnimation.h"

void GlassObject::Start()
{
	// 子オブジェクトを生成して参照をメンバに保存しておく
	GameObject& Effkseer = gameObject->CreateChild();
	Effkseer.AddComponent<UiSpriteRenderer>(TextureTag::EffectLv1, Vector2(350,300), Vector2(500, 500));
	m_hitEffekt = &Effkseer.AddComponent<SpriteAnimation>(5, 5, 5, 26, false);
	GameObject* gaugeMgr = gameObject->world->FindWithTag(Tag::GaugeManager);
	gaude_ = gaugeMgr->GetComponent<GlassGaugeManager>();

	GameObject* score = gameObject->FindWithTag(Tag::ScoreManager);
	score_ = score->GetComponent<ScoreManager>();

	constexpr float HP[ToIndex(GlassGrade::Count)] = { 1.f, 3.f, 5.f };
	hp_ = HP[ToIndex(grade_)];

	tf_ = gameObject->GetComponent<Transform>();
}

void GlassObject::OnDestroy()
{
	gaude_ = nullptr;
	score_ = nullptr;
	tf_ = nullptr;
}

void GlassObject::TakeDamage(float damage)
{
	if (m_hitEffekt)
	{
		m_hitEffekt->Play();
	}
	hp_ -= damage;
	if (hp_ <= 0.f)
	{
		gameObject->Destroy();
		gaude_->BrakeGlass(grade_);
		score_->UpScore(grade_);
		SpawnShards();
		DebugLog::OutputValue("GlassObject Destory, ID", gameObject->id);
		
	}
}

void GlassObject::SpawnShards()
{

	World& W = *gameObject->world;
	GlassShardPool* pool = GlassShardPool::GetInstanse();
	Vector3 pos = tf_->GetPosition();

	const int kSpawnNum = 8 + ToIndex(grade_) * 4;

	for (int i = 0; i < kSpawnNum; ++i)
	{
		pool->Instantiate(pos, grade_);
	}
}

void GlassObject::SetGlassParamPS()
{
	DirectX::XMFLOAT4 Tint[ToIndex(GlassGrade::Count)] = { {0.4f, 0.4f, 0.8f, 0.7f}, {0.4f, 0.8f, 0.4f, 0.7f}, {1.0f, 0.6f, 0.6f, 0.7f} };

	ShaderList::SetGlassParam(Tint[ToIndex(grade_)], 3.0f, 0.5f, 1.0f);
}
