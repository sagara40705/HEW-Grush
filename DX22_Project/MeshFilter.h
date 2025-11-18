#pragma once
#include <memory>
#include <array>
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
        Cube,
        Sphere,
        Plane,
        Cylinder,
        Count
    };
private:
    static std::array<std::unique_ptr<MeshBuffer>, Type::Count> s_meshBuffers;
    Type type{};
public:
    MeshFilter(Type t) : type(t) {}
    ~MeshFilter() override = default;
    void Start() override;
    void OnDestroy() override;
    MeshBuffer* GetMeshBuffer() const;
    Type GetType() const { return type; }
private:
    void BuildPrimitive(); // 種類ごとの生成ルーチン呼び出し
    void BuildCube();
    void BuildSphere();
    void BuildPlane();
    void BuildCylinder();
};