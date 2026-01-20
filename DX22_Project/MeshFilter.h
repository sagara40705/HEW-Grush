#pragma once
#include "Component.h"
#include "PrimitiveMeshManager.h"
#include "ModelManager.h"
#include <cstdint>

enum class MeshSource : uint8_t
{
    Primitive,
    Model,
};

class MeshFilter final : public Component
{
public:
    // プリミティブ用コンストラクタ
    MeshFilter(PrimitiveMeshTag tag) : source(MeshSource::Primitive)
    {
        primitive = PrimitiveMeshManager::Get(tag);
        boundsRadius = PrimitiveMeshManager::GetBounds(tag);  // あれば
    }

    // モデル用コンストラクタ
    MeshFilter(ModelTag tag) : source(MeshSource::Model)
    {
        model = ModelManager::Get(tag);
        boundsRadius = ModelManager::GetBounds(tag);          // あれば
    }

    MeshSource GetSource() const { return source; }
    MeshBuffer* GetPrimitive() const { return primitive; }
    void SetModel(ModelTag tag);
    Model* GetModel()     const { return model; }

    float boundsRadius = 5.0f;
    Vector3 boundsOffset = Vector3(0.f, 0.5f, 0.f);

    // color はデフォルト緑 / group は General としておく
    void SetBoundsOffset(Vector3 offset) { boundsOffset = offset; }
    void DebugDrawBounds() const;

private:
    MeshSource  source{};
    MeshBuffer* primitive = nullptr; // プリミティブ用
    Model* model = nullptr; // モデル用
};
