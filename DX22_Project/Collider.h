// Collider.h
#pragma once
#include "Component.h"
#include "Layer.h"
#include <cstdint>
#include <DirectXMath.h>

class Transform;

enum class ColliderType : uint8_t { Box , Sphere, Capsule };

struct Contact 
{
    DirectX::XMFLOAT3 normal{ 0,1,0 }; // A←B
    float penetration = 0.0f;
};

class Collider : public Component
{
public:
    struct AABB { DirectX::XMFLOAT3 min, max; };

    bool isStatic = false;             // Rigidbody 無しでも強制静的化したい時に使用
    bool isTrigger = false;
    Layer layer = Layer::Default;      // 自身のレイヤ
    LayerMask mask = MaskAll;          // 当たりたい相手レイヤ集合

    virtual ~Collider() = default;
    virtual ColliderType GetType() const = 0;
    virtual AABB GetWorldAABB() = 0;

    // レイヤーマスクで当たり対象か？
    bool IsCollidableWith(const Collider& other) const noexcept 
    {
        const LayerMask meBit = ToBit(layer);
        const LayerMask otherBit = ToBit(other.layer);
        return ((mask & otherBit) != 0u) && ((other.mask & meBit) != 0u);
    }

    // ユーティリティ
    void SetLayer(Layer l) noexcept { layer = l; }
    void SetMask(LayerMask m) noexcept { mask = m; }
    void SetMask(std::initializer_list<Layer> ls) noexcept { mask = MaskOf(ls); }

protected:
    const Transform* GetTransform() const;
    Transform* GetTransform();
};
