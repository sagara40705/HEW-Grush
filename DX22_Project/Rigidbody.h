// Rigidbody.h（イメージ：既存に最小追加）
#pragma once
#include "Component.h"
#include "Vector3.h"

class Rigidbody : public MonoBehaviour
{
private:
    Transform* tf = nullptr;
public:
    Vector3 velocity = Vector3::zero();
    bool useGravity = true;
    const float gravity = -9.8f;;
    float mass = 1.0f; // 今は未使用（将来用）
    //bool  isKinematic = false;
    //const float gravity = -9.8f;

    //float mass = 1.0f;         // 有効質量（0か負は非許可）
    //float bounciness = 0.0f;   // 反発係数 e ∈ [0,1]（0=非弾性, 1=完全弾性）

    void Start() override;
    void Update() override; // ここで「重力→積分」のみ
};
