#include "GlassShardPool.h"
#include "GameObject.h"
#include "Prefab.h"
#include "PrefabSet.h"
#include "GlassShard.h"
#include <cassert>
#include "DebugLog.h"
#include "Camera.h"
#include "Transform.h"
#include <random>
//#include <cmath>          
//#include <DirectXMath.h> 

using namespace DirectX;    // XMConvertToDegrees を楽に使うなら

GlassShardPool* GlassShardPool::s_instance = nullptr;

// [min, max] の一様乱数
float RandRange(float min, float max)
{
	static std::mt19937 s_rng{ std::random_device{}() };
	std::uniform_real_distribution<float> dist(min, max);
	return dist(s_rng);
}

void GlassShardPool::Start()
{
	assert(!s_instance && "GlassShardPoolがすでに定義済み");
	s_instance = this;

	World& W = *gameObject->world;

	for (int i = 0; i < MAX; ++i)
	{
		pool_[i] = &Prefab::Instantiate(W, PrefabId::GlassShard);
		pool_[i]->SetActive(false);
	}

	camTf_ = Camera::Main()->gameObject->transform;
	assert(camTf_ && "メインカメラのtransformがnullptr");

	GameObject* player = gameObject->FindWithTag(Tag::Player);
	player_ = player->transform;
}

void GlassShardPool::OnDestroy()
{
	player_ = nullptr;
	camTf_ = nullptr;
	s_instance = nullptr;
}

GameObject* GlassShardPool::Instantiate(const Vector3& pos, GlassGrade grade, float scale)
{
	//ビルボード処理
	Vector3 rotate = CalcBillboard(pos);

	for (int i = 0; i < MAX; ++i)
	{
		if (pool_[i]->IsActiveSelf()) continue;

		pool_[i]->SetActive(true);
		pool_[i]->transform->SetRotation(rotate);	//向きを設定する
		GlassShard* shard = pool_[i]->GetComponent<GlassShard>();
		assert(shard && "GlassShardがNullptrです");
		shard->OnActive(pos);
		Vector3 impulseDir = MakePlayerBiasedDir(pos);
		float impulsePower = MakeImpulsePower();
		shard->AddForce(impulseDir, impulsePower);
		shard->SetGlassGrade(grade);
		shard->SetGlassScale(scale);
		DebugLog::OutputValue("GlassShardPool::Instantiate", i);
		return pool_[i];
	}

	return nullptr;
}

Vector3 GlassShardPool::CalcBillboard(const Vector3& pos)
{
	Vector3 camPos = camTf_->GetWorldPosition();   // カメラのワールド座標
	Vector3 dir = camPos - pos;                    // 破片 → カメラ 方向

	// 水平成分だけ見る
	dir.y = 0.0f;

	if (dir.sqrMagnitude() > 1e-6f)
	{
		dir.Normalize();

		// forward = (0,0,1) から dir へ向けるための yaw を計算
		// LH座標系で「Z 前、X 右」のとき atan2(x, z)
		float yawRad = std::atan2f(dir.x, dir.z);
		float yawDeg = XMConvertToDegrees(yawRad);

		return Vector3(0.0f, yawDeg, 0.0f);
	}

	return Vector3::zero();
}

// プレイヤーから離れる方向を基準に、ある程度バラつかせた方向ベクトルを生成
Vector3 GlassShardPool::MakePlayerBiasedDir(const Vector3& shardPos)
{
	Vector3 playerPos = player_->GetWorldPosition();

	// 1) 軸（プレイヤーから離れる）
	Vector3 axis = shardPos - playerPos;
	if (axis.SqrLength() < 1e-6f) axis = Vector3::up();
	axis.Normalize();

	// 2) 軸に直交する基底
	Vector3 ortho = Vector3::Cross(axis, Vector3::up());
	if (ortho.SqrLength() < 1e-6f) ortho = Vector3::Cross(axis, Vector3::right());
	ortho.Normalize();
	Vector3 ortho2 = Vector3::Cross(axis, ortho);

	// 3) コーン角：狭い/広いを混ぜる（自然に散る）
	// 例：70%は狭い、30%は広い
	float maxAngleRad = (RandRange(0.0f, 1.0f) < 0.70f) ? 0.55f : 1.05f;
	maxAngleRad = 1.05f;

	// コーン内を均一にサンプル（theta を均等角度で取らないのがポイント）
	float u = RandRange(0.0f, 1.0f);
	float v = RandRange(0.0f, 1.0f);

	float cosMax = cosf(maxAngleRad);
	float cosTheta = (1.0f - u) + u * cosMax; // 1 -> cosMax
	float theta = acosf(cosTheta);
	float phi = v * DirectX::XM_2PI;

	Vector3 dir =
		axis * cosf(theta) +
		ortho * (sinf(theta) * cosf(phi)) +
		ortho2 * (sinf(theta) * sinf(phi));

	// 4) 上方向は “少しだけ” （入れ過ぎると同じ軌道になる）
	dir.y += RandRange(0.f, 0.5f);

	return dir.Normalized();
}

float GlassShardPool::MakeImpulsePower()
{
	float r = RandRange(0.0f, 1.0f);
	float impulsePower =
		(r < 0.25f) ? RandRange(3.0f, 6.0f) :
		(r < 0.6f) ? RandRange(7.0f, 13.0f) :
		RandRange(15.0f, 20.0f);

	return impulsePower;
}

