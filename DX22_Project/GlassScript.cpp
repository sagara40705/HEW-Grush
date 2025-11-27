#include "GlassScript.h"
#include "ScoreManager.h"
#include "Game.h"
#include "GameObject.h"
#include "Transform.h"
#include "MeshFilter.h"
#include "MeshRender.h"
#include "Rigidbody.h"
#include "GlassShard.h"

constexpr int   shardCount = 5;      // 何個飛ばす？
constexpr float power = 10.0f;   // 飛び散りの強さ
constexpr float upwardBias = 1.3f;    // 少し上方向に力を加える

constexpr int kShardCount[ToInt(GlassGrade::Count)] = { 6, 8, 10 };
constexpr float kShardPower[ToInt(GlassGrade::Count)] = { 6.f, 8.f, 10.f };
constexpr float kShardScale[ToInt(GlassGrade::Count)] = { 0.4f, 0.7f, 1.f };

void GlassScript::Start()
{

}


void GlassScript::OnDestroy()
{
	ScoreManager::Instance().AddDestroyedKg(kWeightKg[ToInt(grade)]);
    SpawnShards();
}

void GlassScript::SpawnShards()
{
    World& w = Game::GetWorld();
    if (w.isClearing) return;
    
    Transform& myTf = gameObject->GetComponent<Transform>();
    Vector3 center = myTf.GetPosition();
    Vector3 size = Vector3::one() * kShardScale[ToInt(grade)];

    for (int i = 0; i < kShardCount[ToInt(grade)]; ++i)
    {
        // --- 均等に角度を割り振る（偏りなし） ---
        float t = static_cast<float>(i) / shardCount;
        float angle = t *  3.14f * 2.0f;

        Vector3 dir{
            std::cos(angle),
            upwardBias,
            std::sin(angle)
        };
        dir.Normalize();

       
        // --- 欠片生成 ---
        GameObject& g = w.CreateGameObject();
        
        // Transform
        auto& tf = g.AddComponent<Transform>(center, Vector3::zero(), size);
        //tf.SetPosition(center);
     

        auto& mf = g.AddComponent<MeshFilter>(MeshFilter::Type::Cube);
        auto& mr = g.AddComponent<MeshRender>();

        // Rigidbody
        auto& rb = g.AddComponent<Rigidbody>();

        // GlassShard（寿命管理）
        //auto& shard = g.AddComponent<GlassShard>();
        g.Destroy(1.f);

        // --- 初速を与える（Impulse) ---
        rb.AddForce(dir * kShardPower[ToInt(grade)] * (1 + (i % 3) * 0.3));
        
    }
}
