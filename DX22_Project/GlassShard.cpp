#include "GlassShard.h"
#include "GameObject.h"
#include "ShaderList.h"
#include "Rigidbody.h"
#include "DirectX.h"
#include "GlassShardRenderer.h"
#include <random>
#include <cmath>
#include <cassert>

namespace
{
    // [min, max] の一様乱数
    float RandRange(float min, float max)
    {
        static std::mt19937 s_rng{ std::random_device{}() };
        std::uniform_real_distribution<float> dist(min, max);
        return dist(s_rng);
    }

    // 上向き半球内（0 <= y <= 1）のランダムな単位ベクトル
    // 「ガラスが基本的には上に飛ぶ」想定
    Vector3 RandomUpHemisphere()
    {
        // y を 0.3〜1.0 くらいにして、あまり水平すぎないようにする
        float y = RandRange(0.3f, 1.0f);

        // 残りを水平成分に割り当てる
        float horizontal = std::sqrt(1.0f - y * y);

        // yaw を 0〜2π でランダム
        float yaw = RandRange(0.0f, DirectX::XM_2PI);
        float x = horizontal * std::cos(yaw);
        float z = horizontal * std::sin(yaw);

        return Vector3(x, y, z); // 長さ ≒1
    }

    DirectX::XMFLOAT4 MakeShardRandParam()
    {
        static thread_local std::mt19937 rng{ std::random_device{}() };
        static thread_local std::uniform_real_distribution<float> dist(-3.0f, 2.0f);

        return DirectX::XMFLOAT4{
            dist(rng),
            dist(rng),
            dist(rng),
            dist(rng)
        };
    }
}

void GlassShard::Start()
{
	rb_ = gameObject->GetComponent<Rigidbody>();
    assert(rb_ && "GlassShardのRigidbodyがnullptr");

    randParam_ = MakeShardRandParam();
}

void GlassShard::Update()
{
    constexpr float LIFE = 1.f;
    constexpr float DT = 1.f / 60.f;

    lifeTimer += DT;

    if (lifeTimer >= LIFE)
    {
        gameObject->SetActive(false);
        lifeTimer = 0.f;
    }
}

void GlassShard::OnDestroy()
{
}

void GlassShard::LateUpdate()
{
    GlassShardRenderer* r = GlassShardRenderer::Instance();
    if (!r) return;

    GlassShardRenderer::ShardDrawData d{};
    d.world = DirectX::XMLoadFloat4x4(&gameObject->transform->GetMatrix());
    d.randParam = randParam_;
    d.grade = grade_;

    GlassShardRenderer::Push(d);
}

void GlassShard::OnActive(const Vector3& pos)
{
    assert(rb_ && "GlassShardのRigidbodyがnullptr");
    if (!rb_) return;

    gameObject->transform->SetPosition(pos);

    constexpr float kImpulsePower = 8.0f; 
    Vector3 dir = RandomUpHemisphere();
    dir_ = RandomUpHemisphere();

    rb_->AddForce(dir * kImpulsePower);
}
