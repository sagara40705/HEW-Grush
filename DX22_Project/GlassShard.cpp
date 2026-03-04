#include "GlassShard.h"
#include "GameObject.h"
#include "ShaderList.h"
#include "Rigidbody.h"
#include "DirectX.h"
#include "TextureManager.h"
#include "Time/Time.h"

#include <cmath>
#include <cassert>
#include <random>

DirectX::XMFLOAT4 MakeShardRandParam()
{
    static thread_local std::mt19937 rng{ std::random_device{}() };
    static thread_local std::uniform_real_distribution<float> dist(- 3.0f, 3.0f);

    return DirectX::XMFLOAT4{
        dist(rng),
        dist(rng),
        dist(rng),
        dist(rng)
    };
}

void GlassShard::OnEnable()
{

}

void GlassShard::OnDisable()
{

}

void GlassShard::Start()
{
    tf_ = gameObject->transform;
	rb_ = gameObject->GetComponent<Rigidbody>();
    assert(rb_ && "GlassShardのRigidbodyがnullptr");

	// ランダムパラメータを生成
	randParam_ = MakeShardRandParam();

}

void GlassShard::Update()
{
    constexpr float LIFE = 1.f;
    gameObject->transform->AddRotate(-dir_);

    if (timer_.ElapsedSeconds() >= LIFE)
    {
        gameObject->SetActive(false);
		timer_.Stop();
    }
}

void GlassShard::OnDestroy()
{
	tf_ = nullptr;
	rb_ = nullptr;
}

void GlassShard::OnActive(const Vector3& pos)
{
    assert(rb_ && "GlassShardのRigidbodyがnullptr");
    if (!rb_) return;

    gameObject->transform->SetPosition(pos);
    rb_->velocity = Vector3::zero();
    timer_.Start();
	//AddForce(dir_);
}

void GlassShard::AddForce(const Vector3& dir, float impulsePower)
{
	dir_ = dir;
    rb_->AddForce(dir * impulsePower);
}

void GlassShard::SetGlassGrade(GlassGrade g)
{
	grade_ = g;
}

void GlassShard::SetGlassScale(float s)
{
    tf_->SetScale(Vector3(s, s, s));
}

void GlassShard::SetShardParamVS()
{
    // VS_SHARD の b1 に書き込む
    auto vs = ShaderList::GetVS(ShaderList::VS_SHARD);
    if (vs)
    {
        vs->WriteBuffer(1, &randParam_);
    }
}

void GlassShard::SetGlassParamPS()
{
    DirectX::XMFLOAT4 Tint[ToIndex(GlassGrade::Count)] = {
       {0.0f, 0.0f, 1.0f, 1.0f},
       {0.0f, 1.0f, 0.0f, 1.0f},
       {1.0f, 0.0f, 0.0f, 1.0f}
    };

    ShaderList::SetGlassParam(Tint[ToIndex(grade_)], 5.0f, 3.0f, 1.0f, 1.0f);

    Texture* tex = TextureManager::Get(TextureTag::Game_GlassShard);

    auto ps = ShaderList::GetPS(ShaderList::PS_GLASS_SHARD);
    ps->SetTexture(0, tex);

}


