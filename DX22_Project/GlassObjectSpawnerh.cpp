#include "GlassObjectSpawner.h"
#include "Transform.h"
#include "Prefab.h"
#include "PrefabSet.h"
#include "DebugLog.h"
#include "Camera.h"
#include "GameState.h"
#include <cassert>

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
	{ PrefabId::Swing, Vector3(5.0f, 0.0f, -10.0f), Vector3::zero(), Vector3::one()},
	{ PrefabId::Swing, Vector3(-6.0f, 3.0f, 8.0f),  Vector3::zero(), Vector3::one()},
	{ PrefabId::Swing, Vector3(0.0f, 4.0f, -12.0f), Vector3::zero(), Vector3::one()},
};

// Stage2用スポーンデータ
const std::vector<SpwanData> kStage2SpawnData ={
	// Stage2 左
	// Lv1
	{ PrefabId::Swing, Vector3(90.f, -1.1f, 4.f),   Vector3(0.f, 0.f, -45.f), Vector3::one()},
	{ PrefabId::Kuukiseizyouki, Vector3(65.f, -1.1f, -23.f),  Vector3(0.f, 0.f, 45.f), Vector3::one()},
	{ PrefabId::Kuukiseizyouki, Vector3(65.f, -1.2f, -14.f),  Vector3(15.f, 0.f, 15.f), Vector3::one()},
	{ PrefabId::Ningixyou, Vector3(69.f, -1.1f, -15.f),  Vector3(-30.f, 0.f, -30.f), Vector3::one()},
	{ PrefabId::SagaraDoll, Vector3(73.f, -1.2f, 9.f),  Vector3(-15.f, 0.f, -15.f), Vector3::one()},
	{ PrefabId::Kuukiseizyouki, Vector3(75.f, -1.1f, 3.f),  Vector3(-30.f, 0.f, -30.f), Vector3::one()},
	// Lv2
	{ PrefabId::Benti, Vector3(73.f, -0.9f, 9.f),   Vector3(78.f, 12.f, 45.f), Vector3::one()},
	{ PrefabId::Jihanki, Vector3(69.f, -0.5f, 0.f),  Vector3(90.f, 0.f, 45.f), Vector3::one()},
	{ PrefabId::Jihanki, Vector3(76.f, -0.5f, 23.f),  Vector3(45.f, 0.f, 90.f), Vector3::one()},
	{ PrefabId::Benti, Vector3(75.f, -0.9f, 3.f),   Vector3(22.f, 150.f, 110.f), Vector3::one()},
	{ PrefabId::Jihanki, Vector3(76.f, -0.5f, 19.f),  Vector3(45.f, 0.f, 90.f), Vector3::one()},
	{ PrefabId::Benti, Vector3(75.f, -0.9f, -2.f),   Vector3(167.f, 112.f, 34.f), Vector3::one()},
	{ PrefabId::Jihanki, Vector3(68.f, -0.5f, -7.f),  Vector3(45.f, 0.f, 90.f), Vector3::one()},
	{ PrefabId::Benti, Vector3(80.f, -0.9f, 5.f),   Vector3(110.f, 145.f, 78.f), Vector3::one()},
	// Stage2 右
	// Lv1
	{ PrefabId::Kuukiseizyouki, Vector3(24.f, -1.1f, 10.f),  Vector3(-30.f, 0.f, -30.f), Vector3::one()},
	{ PrefabId::Kuukiseizyouki, Vector3(24.f, -1.1f, -6.f),  Vector3(0.f, 0.f, 45.f), Vector3::one()},
	{ PrefabId::Ningixyou, Vector3(-24.f, -1.2f, -20.f),  Vector3(-15.f, 0.f, -15.f), Vector3::one()},
	{ PrefabId::Ningixyou, Vector3(25.f, -0.9f, 4.f),  Vector3(-45.f, 0.f, 0.f), Vector3::one()},
	{ PrefabId::Ningixyou, Vector3(28.f, -1.2f, -24.f),  Vector3(-15.f, 0.f, -15.f), Vector3::one()},
	// Lv2
	{ PrefabId::Benti, Vector3(15.f, -0.5f, -2.f),   Vector3(90.f, 0.f, 0.f), Vector3::one()},
	{ PrefabId::Benti, Vector3(16.f, -0.9f, 12.f),   Vector3(0.f, 0.f, 0.f), Vector3::one()},
	{ PrefabId::Benti, Vector3(14.f, -0.5f, -16.f),  Vector3(45.f, 0.f, 90.f), Vector3::one()},
	{ PrefabId::Aircon, Vector3(29.f, -0.5f, 14.f),  Vector3(45.f, 0.f, 90.f), Vector3::one()},
	{ PrefabId::Aircon, Vector3(29.f, -0.5f, 7.f),  Vector3(90.f, 0.f, 0.f), Vector3::one()},
	{ PrefabId::Jihanki, Vector3(-33.f, -0.5f, 14.f),  Vector3(90.f, 0.f, 0.f), Vector3::one()},
	{ PrefabId::Jihanki, Vector3(-33.f, -0.5f, 20.f),  Vector3(45.f, 0.f, 90.f), Vector3::one()},
	{ PrefabId::Jihanki, Vector3(-33.f, -1.4f, -13.f),  Vector3(0.f, 0.f, 10.f), Vector3::one()},
	{ PrefabId::Jihanki, Vector3(-33.f, -1.4f, -8.f),  Vector3(0.f, 0.f, -10.f), Vector3::one()},
};

// Stage3-1用スポーンデータ
const std::vector<SpwanData> kStage3aSpawnData = {
	// Stage3-1 左
	// Lv1
	{ PrefabId::Akican,		Vector3(0.f, -1.2f, 19.f),	Vector3(15.f, 0.f, -15.f),	Vector3::one()},
	{ PrefabId::Swing,		Vector3(-5.f, -1.1f, -5.f),	Vector3(30.f, 0.f, 30.f),	Vector3::one()},
	{ PrefabId::Gomibukuro,	Vector3(1.f, -1.1f, 1.f),	Vector3(-30.f, 0.f, -30.f),	Vector3::one()},
	{ PrefabId::Akican,		Vector3(-2.f, -1.1f, 5.f),	Vector3(0.f, 0.f, -45.f),	Vector3::one()},
	{ PrefabId::Swing,		Vector3(4.f, -1.1f, -14.f),	Vector3(-30.f, 0.f, -30.f),	Vector3::one()},
	{ PrefabId::Akican,		Vector3(-7.f, -0.9f, -3.f),	Vector3(-45.f, 0.f, 0.f),	Vector3::one()},
	{ PrefabId::Gomibukuro,	Vector3(2.f, -1.1f, -11.f),	Vector3(0.f, 0.f, 45.f),	Vector3::one()},
	{ PrefabId::Akican,		Vector3(12.f, -1.1f, 15.f),	Vector3(15.f, 0.f, -15.f),	Vector3::one()},
	{ PrefabId::Swing,		Vector3(-1.f, -1.1f, -18.f),Vector3(-30.f, 0.f, -30.f),	Vector3::one()},
	// Lv2
	{ PrefabId::Jihanki,	Vector3(-20.f, -0.5f, 14.f),Vector3(45.f, 0.f, 90.f),	Vector3::one()},
	{ PrefabId::Reizouko,	Vector3(13.f, -1.f, -16.f),	Vector3(0.f, 0.f, 10.f),	Vector3::one()},
	{ PrefabId::Jihanki,	Vector3(2.f, -1.4f, 20.f),	Vector3(0.f, 0.f, -10.f),	Vector3::one()},
	{ PrefabId::Reizouko,	Vector3(-4.f, -0.5f, -22.f),Vector3(0.f, 0.f, 90.f),	Vector3::one()},
	{ PrefabId::Benti,		Vector3(1.f, -0.5f, -14.f),	Vector3(90.f, 0.f, 0.f),	Vector3::one()},
	{ PrefabId::Aircon,		Vector3(-34.f, -0.5f, -6.f),Vector3(90.f, 0.f, 0.f),	Vector3::one()},
	{ PrefabId::Jihanki,	Vector3(6.f, -1.4f, 3.f),	Vector3(0.f, 0.f, -10.f),	Vector3::one()},
	{ PrefabId::Aircon,		Vector3(10.f, -0.5f, -17.f),Vector3(90.f, 0.f, 0.f),	Vector3::one()},
	{ PrefabId::Jihanki,	Vector3(-3.f, -0.9f, 9.f),	Vector3(0.f, 0.f, 0.f),		Vector3::one()},
	{ PrefabId::Aircon,		Vector3(7.f, -0.9f, 9.f),	Vector3(0.f, 0.f, 0.f),		Vector3::one()},
	{ PrefabId::Reizouko,	Vector3(-10.f, -0.5f, 16.f),Vector3(90.f, 0.f, 0.f),	Vector3::one()},
};

// Stage3-2用スポーンデータ
const std::vector<SpwanData> kStage3bSpawnData = {
	// Stage3-2 UE
	// Lv1
	{ PrefabId::Gomibukuro,		Vector3(110.f, -0.5f, 17.f),Vector3(90.f, 0.f, 45.f),	Vector3::one()},
	{ PrefabId::Swing,			Vector3(109.f, -1.4f, 0.f),	Vector3(0.f, 0.f, -10.f),	Vector3::one()},
	{ PrefabId::Swing,			Vector3(116.f, -0.5f, 7.f),	Vector3(90.f, 0.f, -10.f),	Vector3::one()},
	{ PrefabId::Kuukiseizyouki, Vector3(90.f, -0.9f, 22.f), Vector3(0.f, 0.f, 0.f),		Vector3::one()},
	{ PrefabId::SagaraDoll,		Vector3(101.f, -0.5f, 17.f),  Vector3(90.f, 0.f, 0.f), Vector3::one()},
	{ PrefabId::SagaraDoll,		Vector3(95.f, -1.4f, 10.f),  Vector3(0.f, 0.f, 10.f), Vector3::one()},
	// Lv2
	{ PrefabId::Aircon,		Vector3(112.f, -0.5f, 14.f),Vector3(0.f, 0.f, 90.f),	Vector3::one()},
	{ PrefabId::Reizouko,	Vector3(117.f, -0.5f, 12.f),Vector3(90.f, 0.f, 45.f),	Vector3::one()},
	{ PrefabId::Jihanki,	Vector3(112.f, -0.5f, -1.f),Vector3(45.f, 0.f, 90.f),	Vector3::one()},
	{ PrefabId::Jihanki,	Vector3(94.f, -0.5f, 15.f),	Vector3(90.f, 0.f, -10.f),	Vector3::one()},
	{ PrefabId::Aircon,		Vector3(117.f, -0.5f, -1.f),Vector3(0.f, 0.f, 90.f),	Vector3::one()},
	// Stage3-2 Left
	// Lv1
	{ PrefabId::Swing,			Vector3(-14.f, -0.9f, -19.f),	Vector3(45.f, 0.f, 0.f),	Vector3::one()},
	{ PrefabId::Swing,			Vector3(-13.f, -1.2f, -10.f),	Vector3(15.f, 0.f, 15.f),	Vector3::one()},
	{ PrefabId::Swing,			Vector3(-18.f, -0.9f, 1.f),		Vector3(45.f, 0.f, 0.f),	Vector3::one()},
	{ PrefabId::Gomibukuro,		Vector3(-15.f, -0.9f, 3.f),		Vector3(-45.f, 0.f,0.f),	Vector3::one()},
	{ PrefabId::Swing,			Vector3(-17.f, -1.1f, -1.f),	Vector3(0.f, 0.f, 45.f),	Vector3::one()},
	{ PrefabId::SagaraDoll,		Vector3(-26.f, -0.9f, -17.f),	Vector3(0.f, 0.f, 0.f),		Vector3::one()},
	{ PrefabId::Kuukiseizyouki, Vector3(-26.f, -0.9f, 16.f),	Vector3(0.f, 0.f, 0.f),		Vector3::one()},
	// Lv2
	{ PrefabId::Jihanki,	Vector3(3.f, -0.9f, 10.f),		Vector3(0.f, 0.f, 1.f),		Vector3::one()},
	{ PrefabId::Aircon,		Vector3(-2.f, -0.5f, 16.f),		Vector3(0.f, 90.f, 1.f),	Vector3::one()},
	{ PrefabId::Reizouko,	Vector3(-1.f, -1.4f, 23.f),		Vector3(0.f, 0.f, 10.f),	Vector3::one()},
	{ PrefabId::Benti,		Vector3(-26.f, -0.9f, 16.f),	Vector3(0.f, 0.f, 1.f),	Vector3::one()},
	{ PrefabId::Benti,		Vector3(-26.f, -0.5f, -17.f),	Vector3(45.f, 0.f,90.f),	Vector3::one()},
};

// Stage3-3用スポーンデータ
const std::vector<SpwanData> kStage3cSpawnData = {
	// Stage3-3 UE
	// Lv1
	{ PrefabId::Swing,		Vector3(99.f, -0.9f, 10.f),	Vector3(45.f, 0.f, 0.f),	Vector3::one()},
	{ PrefabId::Gomibukuro,	Vector3(98.f, -1.1f, 10.f),	Vector3(0.f, 0.f, -45.f),	Vector3::one()},
	{ PrefabId::Swing,		Vector3(99.f, -0.9f, 12.f),	Vector3(-45.f, 0.f, 0.f),	Vector3::one()},
	{ PrefabId::Akican,		Vector3(108.f, -1.2f, -4.f),Vector3(15.f, 0.f, 15.f),	Vector3::one()},
	{ PrefabId::Swing,		Vector3(107.f, -1.2f, -5.f),Vector3(-15.f, 0.f, -15.f),	Vector3::one()},
	{ PrefabId::Akican,		Vector3(90.f, -1.1f, 9.f),	Vector3(0.f, 0.f, 45.f),	Vector3::one()},
	{ PrefabId::Swing,		Vector3(89.f, -1.1f, 5.f),	Vector3(0.f, 0.f, -45.f),	Vector3::one()},
	{ PrefabId::Gomibukuro,	Vector3(90.f, -1.1f, 8.f),	Vector3(0.f, 0.f, 45.f),	Vector3::one()},
	{ PrefabId::Akican,		Vector3(92.f, -1.1f, 8.f),	Vector3(0.f, 0.f, -45.f),	Vector3::one()},
	// Lv2
	{ PrefabId::Jihanki,	Vector3(96.f, -0.9f, 2.f),		Vector3(0.f, 0.f, 0.f),		Vector3::one()},
	{ PrefabId::Reizouko,	Vector3(97.f, -1.0f, -2.f),		Vector3(0.f, 0.f, 10.f),	Vector3::one()},
	{ PrefabId::Jihanki,	Vector3(81.f, -0.5f, 7.f),		Vector3(90.f, 0.f, 0.f),	Vector3::one()},
	{ PrefabId::Aircon,		Vector3(83.f, -0.5f, 7.f),		Vector3(90.f, 0.f, 45.f),	Vector3::one()},
	{ PrefabId::Aircon,		Vector3(98.f, -0.9f, 5.f),		Vector3(0.f, 0.f, 0.f),		Vector3::one()},
	// Stage3-3 Left
	// Lv1
	{ PrefabId::Gomibukuro,	Vector3(-18.f, -1.2f, -12.f),	Vector3(15.f, 0.f, 15.f),	Vector3::one()},
	{ PrefabId::Swing,		Vector3(-19.f, -1.2f, 16.f),	Vector3(-15.f, 0.f, -15.f),	Vector3::one()},
	{ PrefabId::Akican,		Vector3(-13.f, -0.9f, 8.f),		Vector3(45.f, 0.f, 0.f),	Vector3::one()},
	{ PrefabId::Gomibukuro,	Vector3(-19.f, -0.9f, 20.f),	Vector3(45.f, 0.f, 0.f),	Vector3::one()},
	// Lv2
	{ PrefabId::Aircon,		Vector3(-8.f, -0.9f, -16.f),	Vector3(0.f, 0.f, 0.f),		Vector3::one()},
	{ PrefabId::Aircon,		Vector3(-7.f, -0.5f, 0.f),		Vector3(45.f, 0.f, 90.f),	Vector3::one()},
	{ PrefabId::Jihanki,	Vector3(-9.f, -0.5f, 17.f),		Vector3(90.f, 0.f, 0.f),	Vector3::one()},
	{ PrefabId::Jihanki,	Vector3(-4.f, -0.5f, -20.f),	Vector3(45.f, 0.f, 90.f),	Vector3::one()},
	{ PrefabId::Jihanki,	Vector3(-11.f, -1.4f, -7.f),	Vector3(90.f, 0.f, 0.f),	Vector3::one()},
	{ PrefabId::Aircon,		Vector3(-17.f, -0.5f, -6.f),	Vector3(90.f, 0.f, -10.f),	Vector3::one()},
};

const std::vector<SpwanData> GetUseSpawnData();

void GlassObjectSpawner::Start()
{
	spawnTimer = 0.0f;
	cam_ = Camera::Main();

	// 最初のガラスオブジェクトをスポーン
	FirstSpawn();

	DebugLog::Output("[GlassObjectSpawner] Start()");
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

	const std::vector<SpwanData> spawnData = GetUseSpawnData();
	if (spawnData.size() == 0)
	{
		// 生成データがない場合は何もしない
		return;
	}

	// スポーン処理
	for (const ObjectData& data : spawnObjData_)
	{
		// 既存のオブジェクトが破壊されているかIDでチェック
		if (world->Find(data.id))
		{
			// まだ存在しているならスキップ
			//DebugLog::OutputValue("GlassObjectSpawner 生存中", id.id);
			continue;
		}
		// Camera::IsVisibleSphereで画面外にいるかチェックしても良いかも
		if (cam_->IsVisibleSphere(spawnData[data.index].position, 0.7f))
		{
			//DebugLog::OutputValue("GlassObjectSpawner 画面内", spawnObjectId[i]);
			continue;
		}

		// 破壊されているなら新たにスポーン
		const SpwanData& newData = spawnData[data.index];
		GameObject& obj = Prefab::Instantiate(*world, newData.id, newData.position, newData.rotation, newData.scale);
		spawnObjData_[data.index].id = obj.id;
		DebugLog::OutputValue("[GlassObjectSpawner] , スポーンします", obj.id);

	}

	/*
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
		//if (cam_->IsVisibleSphere(g_spawnData[i].position, 0.7f))
		//{
		//	//DebugLog::OutputValue("GlassObjectSpawner 画面内", spawnObjectId[i]);
		//	continue;
		//}

		
		// 破壊されているなら新たにスポーン	
		const SpwanData& data = g_spawnData[i];

		GameObject& obj = Prefab::Instantiate(*world, data.id, data.position, data.rotation, data.scale);
		spawnObjectId[i] = obj.id;
		DebugLog::OutputValue("[GlassObjectSpawner] , スポーンします", spawnObjectId[i]);
	}
	*/
}

void GlassObjectSpawner::OnDestroy()
{
}

void GlassObjectSpawner::FirstSpawn()
{
	StageId currentStage = GetCurrentStage();
	std::vector<SpwanData> spawnData = GetUseSpawnData();

	// 生成データがない場合は何もしない
	if (spawnData.size() == 0)
	{
		return;
	}

	// 初期化
	World* world = gameObject->world;
	spawnObjData_.clear();
	int index = 0;

	for (const SpwanData& data : spawnData)
	{
		GameObject& obj = Prefab::Instantiate(*world, data.id, data.position, data.rotation, data.scale);

		//スポーン可能オブジェクトリストに追加
		ObjectData data = { index, obj.id };
		spawnObjData_.push_back(data);

		++index; //インデックス更新
	}
}

const std::vector<SpwanData> GetUseSpawnData()
{
	StageId currentStage = GetCurrentStage();

	switch (currentStage)
	{
	case StageId::Stage1:
		break;
	case StageId::Stage2:
		return kStage2SpawnData;
		break;
	case StageId::Stage3a:
		return kStage3aSpawnData;
		break;
	case StageId::Stage3b:
		return kStage3bSpawnData;
		break;
	case StageId::Stage3c:
		return kStage3cSpawnData;
		break;
	default:
		assert(false && "GlassObjectSpawner::GetUseSpawnData()-未対応のステージです");
		break;
	}

	return std::vector<SpwanData>();
}

