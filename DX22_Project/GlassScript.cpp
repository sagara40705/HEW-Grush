#include "GlassScript.h"
#include "ScoreManager.h"
#include "Game.h"
#include "GameObject.h"
#include "Transform.h"
#include "MeshFilter.h"
#include "MeshRender.h"
#include "Rigidbody.h"
#include "GlassShard.h"
#include "ModelComponent.h"
#include "ModelRenderer.h"
#include "ModelManager.h"
#include "ShaderList.h"
#include "Collider.h"
#include <algorithm>
#include "DebugLog.h"

constexpr int   shardCount = 5;      // âΩå¬îÚÇŒÇ∑ÅH
constexpr float power = 10.0f;   // îÚÇ—éUÇËÇÃã≠Ç≥
constexpr float upwardBias = 1.3f;    // è≠Çµè„ï˚å¸Ç…óÕÇâ¡Ç¶ÇÈ

constexpr int kShardCount[ToInt(GlassGrade::Count)] = { 6, 8, 10 };
constexpr float kShardPower[ToInt(GlassGrade::Count)] = { 6.f, 8.f, 10.f };
constexpr float kShardScale[ToInt(GlassGrade::Count)] = { 0.4f, 0.7f, 1.f };

void GlassScript::Start()
{

}

void GlassScript::Update()
{
    constexpr float DT = 1.f / 60.f;

    if (!isBreaking) return;

    breakTimer += DT;
    float t = std::clamp(breakTimer / breakDuration, 0.0f, 1.0f);

    // ShaderListÇ÷îöéUÉpÉâÉÅÅ[É^ÇëóÇÈ
    ShaderList::SetExplodeParam(t, 2.f);

    if (breakTimer >= breakDuration)
    {
        // îöéUèIóπ Å® ÉKÉâÉXîjâÛäÆóπ
        gameObject->Destroy();
    }
}


void GlassScript::OnDestroy()
{
	ScoreManager::Instance().AddDestroyedKg(kWeightKg[ToInt(grade)]);
    //SpawnShards();
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
        // --- ãœìôÇ…äpìxÇäÑÇËêUÇÈÅiïŒÇËÇ»ÇµÅj ---
        float t = static_cast<float>(i) / shardCount;
        float angle = t *  3.14f * 2.0f;

        Vector3 dir{
            std::cos(angle),
            upwardBias,
            std::sin(angle)
        };
        dir.Normalize();

       
        // --- åáï–ê∂ê¨ ---
        GameObject& g = w.CreateGameObject();
        
        // Transform
        auto& tf = g.AddComponent<Transform>(center, Vector3::zero(), size);
        //tf.SetPosition(center);
     

        //auto& mf = g.AddComponent<MeshFilter>(MeshFilter::Type::Cube);
        //auto& mr = g.AddComponent<MeshRender>();

        auto& mc = g.AddComponent<ModelComponent>(ModelManager::Get(ModelTag::ProtPlay));
        auto& mr = g.AddComponent<ModelRenderer>();

        // Rigidbody
        auto& rb = g.AddComponent<Rigidbody>();

        // GlassShardÅiéıñΩä«óùÅj
        auto& gs = g.AddComponent<GlassScript>(grade);
        g.Destroy(1.f);

        // --- èâë¨Çó^Ç¶ÇÈÅiImpulse) ---
        rb.AddForce(dir * kShardPower[ToInt(grade)] * (1 + (i % 3) * 0.3));
        
    }
}

void GlassScript::StartBrake()
{
    if (isBreaking) return;
    isBreaking = true;

    // ShaderÇîöéUópÇ…ïœçX
    ModelRenderer* mr = gameObject->TryGetComponent<ModelRenderer>();
    //Collider* c = gameObject->TryGetComponent<Collider>();

    if(mr)  Debug::Log("StartBrake() - ModelRenderer");
    //if(c)  Debug::Log("StartBrake() - Collider");

    if (!mr ) return;

    breakTimer = 0.0f;

    //c->isTrigger = true; c->SetEnabled(false);
    mr->vsKind = ShaderList::VS_EXPLODE;

}
