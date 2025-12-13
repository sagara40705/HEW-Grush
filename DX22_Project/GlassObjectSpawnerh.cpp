#include "GlassObjectSpawner.h"
#include "Transform.h"
#include "Prefab.h"
#include "PrefabSet.h"
#include "DebugLog.h"
#include "Camera.h"

//生成データ構造体
struct SpwanData
{
	PrefabId id = PrefabId::Swing;
	Vector3 position = Vector3::zero();
	Vector3 rotation = Vector3::zero();
	Vector3 scale = Vector3::one();
};

//生成データ配列
const SpwanData g_spawnData[kSpawnDataCount] =
{
	{ PrefabId::Swing, Vector3(0.0f, 0.0f, 0.0f),   Vector3::zero(), Vector3::one() },
	{ PrefabId::Swing, Vector3(3.0f, 0.0f, -2.0f),  Vector3::zero(), Vector3::one() },
	{ PrefabId::Swing, Vector3(-3.0f, 0.0f, 3.0f),  Vector3::zero(), Vector3::one() },
	{ PrefabId::Swing, Vector3(1.5f, 0.0f, 2.0f),   Vector3::zero(), Vector3::one() },
	{ PrefabId::Swing, Vector3(-3.5f, 0.0f, -3.0f), Vector3::zero(), Vector3::one() },
};

void GlassObjectSpawner::Start()
{
	spawnTimer = 0.0f;
	cam_ = Camera::Main();

	// 最初のガラスオブジェクトをスポーン
	World* world = gameObject->world;

	for (int i = 0; i < kSpawnDataCount; ++i)
	{
		const SpwanData& data = g_spawnData[i];

		GameObject& obj = Prefab::Instantiate(*world, data.id, data.position, data.rotation, data.scale);
		spawnObjectId[i] = obj.id;
	}
	DebugLog::Output("GlassObjectSpawner::Start()");
}

void GlassObjectSpawner::Update()
{
	//仮の経過時間
	constexpr float DT = 1.f / 60.f;

	// タイマー更新
	spawnTimer += DT;

	// スポーン間隔を超えたらガラスオブジェクトをスポーン
	if (spawnTimer < spawnInterval) return;

	spawnTimer = 0.0f; // タイマーリセット
	World* world = gameObject->world;
	DebugLog::Output("GlassObjectSpawner::Update(), SpawnTime");

	for (int i = 0; i < kSpawnDataCount; ++i)
	{
		// 既存のオブジェクトが破壊されているかIDでチェック
		if (world->Find(spawnObjectId[i]))
		{
			// まだ存在しているならスキップ
			//DebugLog::OutputValue("GlassObjectSpawner 生存中", spawnObjectId[i]);
			continue;
		}

		// Camera::IsVisibleSphereで画面外にいるかチェックしても良いかも
		if (cam_->IsVisibleSphere(g_spawnData[i].position, 0.7f))
		{
			//DebugLog::OutputValue("GlassObjectSpawner 画面内", spawnObjectId[i]);
			continue;
		}

		
		// 破壊されているなら新たにスポーン	
		const SpwanData& data = g_spawnData[i];

		GameObject& obj = Prefab::Instantiate(*world, data.id, data.position, data.rotation, data.scale);
		spawnObjectId[i] = obj.id;
		DebugLog::OutputValue("GlassObjectSpawner::Update(), スポーンします", spawnObjectId[i]);
	}


	//{
	//	spawnTimer = 0.0f; // タイマーリセット

	//	World* world = gameObject->world;

	//	for (int i = 0; i < kSpawnDataCount; ++i)
	//	{
	//		const SpwanData& data = g_spawnData[i];

	//		GameObject& obj = Prefab::Instantiate(*world, data.id, data.position, data.rotation, data.scale);
	//		spawnObjectId[i] = obj.id;
	//	}
	//}

}

void GlassObjectSpawner::OnDestroy()
{
}
