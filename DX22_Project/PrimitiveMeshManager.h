#pragma once
#include <memory>
#include <array>
#include <cstdint>
#include "MeshBuffer.h"

enum class PrimitiveMeshTag : uint8_t
{
    Cube,
    Sphere,
    Plane,
    Cylinder,
    GlassShard,
    Quad,
    Count
};

class PrimitiveMeshManager
{
public:
    static void Init();      // ‹N“®
    static void Uninit(); // I—¹

    static MeshBuffer* Get(PrimitiveMeshTag tag)
    {
        return primitives[static_cast<size_t>(tag)].get();
    }

    static float GetBounds(PrimitiveMeshTag tag)
    {
        return primitiveBounds[static_cast<size_t>(tag)];
    }
private:
    static std::array<std::unique_ptr<MeshBuffer>, static_cast<size_t>(PrimitiveMeshTag::Count)> primitives;
    static std::array<float, static_cast<size_t>(PrimitiveMeshTag::Count)> primitiveBounds;
private:
    static void CreateCube();
    static void CreateSphere();
    static void CreatePlane();
    static void CreateCylinder();
    static void CreateGlassShard();
};