#include "GlassShard.h"
#include "GameObject.h"
#include "ShaderList.h"
#include "Rigidbody.h"
#include <random>
#include <cmath>

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
}

void GlassShard::OnEnable()
{

}

void GlassShard::OnDisable()
{

}

void GlassShard::Start()
{
	rb_ = gameObject->GetComponent<Rigidbody>();
	if (!rb_) return;

    constexpr float kImpulsePower = 8.0f; // 元の 10.f をそのまま流用
    Vector3 dir = RandomUpHemisphere();

    rb_->AddForce(dir * kImpulsePower);
}

void GlassShard::Update()
{

}

void GlassShard::OnDestroy()
{

}

void GlassShard::SetGlassParamPS()
{
	DirectX::XMFLOAT4 Tint[ToIndex(GlassGrade::Count)] = { {0.0f, 0.0f, 1.0f, 0.8f}, {0.0f, 1.0f, 0.0f, 0.8f}, {1.0f, 0.0f, 0.0f, 0.8f} };

	ShaderList::SetGlassParam(Tint[ToIndex(grade_)], 3.0f, 0.5f, 1.0f);
}


