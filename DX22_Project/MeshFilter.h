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
    Model* GetModel()     const { return model; }

    float boundsRadius = 30.0f;

private:
    MeshSource  source{};
    MeshBuffer* primitive = nullptr; // プリミティブ用
    Model* model = nullptr; // モデル用
};
