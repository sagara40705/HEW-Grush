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
#include "Sprite3DRenderer.h"
#include "SpriteAnimation.h"
#include "SoundManager.h"

void  GlassObject::Awake()
{
	// オブジェクトを生成して参照をメンバに保存しておく
	// *子オブジェクトとして生成すると、親破壊時に一緒に破壊されるので変更

	//破壊エフェクト生成
	//GameObject& Effkseer = gameObject->world->CreateGameObject();
	//Effkseer.GetComponent<Transform>()->SetPosition(gameObject->GetComponent<Transform>()->GetPosition());
	//Effkseer.AddComponent<Sprite3DRenderer>(TextureTag::EffectLv1, Vector2(0, 0), Vector2(5, 5));
	//m_hitEffekt = &Effkseer.AddComponent<SpriteAnimation>(5, 5, 5, 26, false);
}
 
void GlassObject::Start()
{
	// ゲージマネージャーの参照を取得
	GameObject* gaugeMgr = gameObject->world->FindWithTag(Tag::GaugeManager);
	if (!gaugeMgr) return;
	gaude_ = gaugeMgr->GetComponent<GlassGaugeManager>();

	// スコアマネージャーの参照を取得
	GameObject* score = gameObject->FindWithTag(Tag::ScoreManager);
	score_ = score->GetComponent<ScoreManager>();

	constexpr float HP[ToIndex(GlassGrade::Count)] = { 1.f, 5.f, 25.f };
	hp_ = HP[ToIndex(grade_)];

	tf_ = gameObject->GetComponent<Transform>();

	//m_hitEffekt->Start();
}

void GlassObject::OnDestroy()
{
	gaude_ = nullptr;
	score_ = nullptr;
	tf_ = nullptr;
}

void GlassObject::TakeDamage(float damage)
{
	hp_ -= damage;
	if (hp_ <= 0.f)
	{
		gameObject->Destroy();
		gaude_->BrakeGlass(grade_);
		score_->UpScore(grade_);
		SpawnShards();
		DebugLog::OutputValue("GlassObject Destory, ID", gameObject->id);
		switch (grade_)
		{
		case GlassGrade::Grade_1:
			SoundManager::SoundPlay(Level1GlassBreakSE,0.7f);
			break;
		case GlassGrade::Grade_2:
			SoundManager::SoundPlay(Level2GlassBreakSE, 0.7f);
			break;
		case GlassGrade::Grade_3:
		SoundManager::SoundPlay(Level3GlassBreakSE, 0.8f);
			break;
		}
		return;
	}
	else
	{
		SpawnShardMini();
		SoundManager::SoundPlay(GlassCrackSE, 1.0f);
	}
}

void GlassObject::SpawnShards()
{
	//m_hitEffekt->Play();

	World& W = *gameObject->world;
	GlassShardPool* pool = GlassShardPool::GetInstanse();
	Vector3 pos = tf_->GetPosition();

	const size_t kSpawnNum = 6 + ToIndex(grade_) * 6;

	constexpr float kSize[ToIndex(GlassGrade::Count)] = { 0.7f,1.f, 1.3f };

	for (int i = 0; i < kSpawnNum; ++i)
	{
		pool->Instantiate(pos, grade_, kSize[ToIndex(grade_)]);
	}
}

void GlassObject::SpawnShardMini()
{
	World& W = *gameObject->world;
	GlassShardPool* pool = GlassShardPool::GetInstanse();
	Vector3 pos = tf_->GetPosition();

	const size_t kSpawnNum = ToIndex(grade_) * 6;

	constexpr float kSize[ToIndex(GlassGrade::Count)] = { 0.3f, 0.5f, 0.7f };

	for (int i = 0; i < kSpawnNum; ++i)
	{
		pool->Instantiate(pos, grade_, kSize[ToIndex(grade_)]);
	}
}

void GlassObject::SetGlassParamPS()
{
	DirectX::XMFLOAT4 Tint[ToIndex(GlassGrade::Count)] = { {0.4f, 0.4f, 0.8f, 0.8f}, {0.4f, 0.8f, 0.4f, 0.8f}, {0.8f, 0.6f, 0.6f, 0.8f} };

	ShaderList::SetGlassParam(Tint[ToIndex(grade_)], 3.0f, 0.5f, 1.0f);
}
