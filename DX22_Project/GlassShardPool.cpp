#include "GlassShardPool.h"
#include "GameObject.h"
#include "Prefab.h"
#include "PrefabSet.h"
#include "GlassShard.h"
#include <cassert>
#include "DebugLog.h"
#include "Camera.h"
#include "Transform.h"
#include "GlassShardRenderer.h"
//#include <cmath>          
//#include <DirectXMath.h> 

using namespace DirectX;    // XMConvertToDegrees を楽に使うなら

GlassShardPool* GlassShardPool::s_instance = nullptr;

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
}

void GlassShardPool::OnDestroy()
{
	s_instance = nullptr;
}

void GlassShardPool::Update()
{
	GlassShardRenderer::Instance()->BeginFrame();
}

GameObject* GlassShardPool::Instantiate(const Vector3& pos, GlassGrade grade)
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
		shard->SetGlassGrade(grade);
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
