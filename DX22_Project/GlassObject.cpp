#include "GlassObject.h"
#include "GameObject.h"
#include "Transform.h"
#include "GlassGaugeManager.h"
#include "ScoreManager.h"
#include "DebugLog.h"
#include "Prefab.h"
#include "PrefabSet.h"
#include "Rigidbody.h"


void GlassObject::Start()
{
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

	GameObject& shard = Prefab::Instantiate(W, PrefabId::GlassShard, tf_->GetPosition());
	Rigidbody* rb_ = shard.GetComponent<Rigidbody>();
	if (!rb_) return;

	constexpr Vector3 Impulse = { 0.f, 10.f, 0.f };
	rb_->AddForce(Impulse);
	shard.Destroy(3.0f);

	DebugLog::OutputValue("SpawnShards, ID", shard.id);
}
