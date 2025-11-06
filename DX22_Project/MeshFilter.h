#pragma once
#include "Component.h"
#include "MeshBuffer.h"

struct Vertex
{
    float pos[3];
    float uv[2];
};

//途中変更なし予定
class MeshFilter : public Component
{
public:
    enum Type
    {
        None,
        Cube,
        Sphere,
        Plane,
        Cylinder
    };
private:
    Type PrimitiveType{};
    std::shared_ptr<MeshBuffer> mMeshBuffer{};
public:
    MeshFilter(Type type = None) : PrimitiveType(type) {}
    ~MeshFilter() override = default;
    void Start() override;
    void OnDestroy() override;
    std::shared_ptr<MeshBuffer> GetMeshBuffer() const { return mMeshBuffer; }
    Type GetType() const { return PrimitiveType; }
private:
    void BuildPrimitive(); // 種類ごとの生成ルーチン呼び出し
    void BuildCube();
    void BuildSphere();
    void BuildPlane();
    void BuildCylinder();
};