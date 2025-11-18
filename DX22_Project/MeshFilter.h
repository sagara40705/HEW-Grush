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
    explicit MeshFilter(Type t = Cube) : type(t) {}
    ~MeshFilter() override = default;

    void Start() override;
    void OnDestroy() override;

    MeshBuffer* GetMeshBuffer() const;
    Type GetType() const { return type; }

    static void ReleaseAll();
private:
    static void EnsureBuilt(Type t); // 必要なプリミティブだけ生成

    static void BuildCube();
    static void BuildSphere();
    static void BuildPlane();
    static void BuildCylinder();
};