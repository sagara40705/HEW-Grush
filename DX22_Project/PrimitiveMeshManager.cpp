#include "PrimitiveMeshManager.h"
#include <vector>
#include <DirectXMath.h>
#include <cmath>
using namespace DirectX;

std::array<std::unique_ptr<MeshBuffer>, static_cast<size_t>(PrimitiveMeshTag::Count)> PrimitiveMeshManager::primitives{};
std::array<float, static_cast<size_t>(PrimitiveMeshTag::Count)> PrimitiveMeshManager::primitiveBounds{};

struct Vertex
{
    float pos[3];
    float uv[2];
};

void PrimitiveMeshManager::Init()
{
    CreateCube();
    primitiveBounds[(size_t)PrimitiveMeshTag::Cube] = 0.5f * std::sqrt(3.0f); // 対角線の半径など

    CreateSphere();
    primitiveBounds[(size_t)PrimitiveMeshTag::Sphere] = 0.5f; // radius

    CreatePlane();
    primitiveBounds[(size_t)PrimitiveMeshTag::Plane] = 0.5f * std::sqrt(2.0f);

    CreateCylinder();
    primitiveBounds[(size_t)PrimitiveMeshTag::Cylinder] = std::sqrt(0.5f * 0.5f + 0.5f * 0.5f);

    CreateGlassShard();
    primitiveBounds[(size_t)PrimitiveMeshTag::GlassShard] = 0.5f; // radius
}

void PrimitiveMeshManager::Uninit()
{
    for (auto& t : primitives)
    {
        t.reset();
    }
}

void PrimitiveMeshManager::CreateCube()
{
    //--- 頂点の作成（中心(0,0,0)、サイズ1のキューブ）
    constexpr Vertex vtx[] = {
        // -Z 面
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}},
        // +Z 面
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}},
        // +X 面
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}},
        // -X 面
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}},
        // +Y 面
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
        // -Y 面
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}},
    };

    //--- インデックスの作成
    constexpr unsigned int idx[] = {
        0, 1, 2,  1, 3, 2,   // -Z
        4, 6, 5,  5, 6, 7,   // +Z

        8,  9, 10,  9, 11, 10,   // +X
        13, 12, 14, 13, 14, 15,  // -X

        16, 17, 18, 17, 19, 18,  // +Y
        21, 20, 22, 21, 22, 23,  // -Y
    };

    MeshBuffer::Description desc{};
    desc.pVtx = vtx;
    desc.vtxCount = static_cast<UINT>(std::size(vtx));
    desc.vtxSize = sizeof(Vertex);
    desc.isWrite = false;
    desc.pIdx = idx;
    desc.idxCount = static_cast<UINT>(std::size(idx));
    desc.idxSize = sizeof(unsigned int);
    desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    auto mb = std::make_unique<MeshBuffer>();
    mb->Create(desc);
    primitives[static_cast<size_t>(PrimitiveMeshTag::Cube)] = std::move(mb);
}

void PrimitiveMeshManager::CreateSphere()
{
    constexpr int   slice = 16;    // 経度分割
    constexpr int   stack = 16;    // 緯度分割
    constexpr float radius = 0.5f;

    std::vector<Vertex>        vertices;
    std::vector<unsigned int>  indices;

    // 頂点生成
    for (int i = 0; i <= stack; ++i)
    {
        float phi = XM_PI * i / stack;
        float y = std::cos(phi);
        float r = std::sin(phi);

        for (int j = 0; j <= slice; ++j)
        {
            float theta = XM_2PI * j / slice;
            float x = r * std::cos(theta);
            float z = r * std::sin(theta);

            Vertex v{};
            v.pos[0] = x * radius;
            v.pos[1] = y * radius;
            v.pos[2] = z * radius;
            v.uv[0] = static_cast<float>(j) / slice;
            v.uv[1] = static_cast<float>(i) / stack;
            vertices.push_back(v);
        }
    }

    // インデックス生成
    for (int i = 0; i < stack; ++i)
    {
        for (int j = 0; j < slice; ++j)
        {
            int a = i * (slice + 1) + j;
            int b = a + slice + 1;

            indices.push_back(a);
            indices.push_back(b);
            indices.push_back(a + 1);

            indices.push_back(a + 1);
            indices.push_back(b);
            indices.push_back(b + 1);
        }
    }

    MeshBuffer::Description desc{};
    desc.pVtx = vertices.data();
    desc.vtxCount = static_cast<UINT>(vertices.size());
    desc.vtxSize = sizeof(Vertex);
    desc.isWrite = false;
    desc.pIdx = indices.data();
    desc.idxCount = static_cast<UINT>(indices.size());
    desc.idxSize = sizeof(unsigned int);
    desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    auto mb = std::make_unique<MeshBuffer>();
    mb->Create(desc);
    primitives[static_cast<size_t>(PrimitiveMeshTag::Sphere)] = std::move(mb);
}

void PrimitiveMeshManager::CreatePlane()
{
    // y=0 の XZ 平面（原点中心、1x1）
    constexpr Vertex vtx[4] =
    {
        {{-0.5f, 0.0f, -0.5f}, {0.0f, 1.0f}}, // 左下
        {{-0.5f, 0.0f,  0.5f}, {0.0f, 0.0f}}, // 左上
        {{ 0.5f, 0.0f,  0.5f}, {1.0f, 0.0f}}, // 右上
        {{ 0.5f, 0.0f, -0.5f}, {1.0f, 1.0f}}, // 右下
    };

    constexpr unsigned int idx[6] = { 0, 1, 2,  0, 2, 3 };

    MeshBuffer::Description desc{};
    desc.pVtx = vtx;
    desc.vtxCount = 4;
    desc.vtxSize = sizeof(Vertex);
    desc.isWrite = false;
    desc.pIdx = idx;
    desc.idxCount = 6;
    desc.idxSize = sizeof(unsigned int);
    desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    auto mb = std::make_unique<MeshBuffer>();
    mb->Create(desc);
    primitives[static_cast<size_t>(PrimitiveMeshTag::Plane)] = std::move(mb);
}

void PrimitiveMeshManager::CreateCylinder()
{
    constexpr int   slice = 24;    // 角度分割
    constexpr float radius = 0.5f;  // 半径
    constexpr float halfH = 0.5f;  // 高さ / 2

    std::vector<Vertex>        vertices;
    std::vector<unsigned int>  indices;

    // ===== 側面 =====
    // 下段(y=-halfH) と 上段(y=+halfH) の2列、それぞれ (slice+1) 頂点でクローズ
    for (int yStep = 0; yStep <= 1; ++yStep)
    {
        const float y = (yStep == 0) ? -halfH : +halfH;
        const float v = static_cast<float>(yStep); // 0 or 1

        for (int i = 0; i <= slice; ++i)
        {
            const float t = static_cast<float>(i) / slice; // 0..1
            const float theta = XM_2PI * t;
            const float x = std::cos(theta) * radius;
            const float z = std::sin(theta) * radius;

            Vertex vert{};
            vert.pos[0] = x;
            vert.pos[1] = y;
            vert.pos[2] = z;
            vert.uv[0] = t;   // U=角度
            vert.uv[1] = v;   // V=高さ
            vertices.push_back(vert);
        }
    }
    const unsigned int sideStride = static_cast<unsigned int>(slice + 1);

    // 側面インデックス（外側から見て CW）
    for (int i = 0; i < slice; ++i)
    {
        const unsigned int a = i;                 // 下段
        const unsigned int b = i + sideStride;    // 上段

        indices.push_back(a);
        indices.push_back(b);
        indices.push_back(a + 1);

        indices.push_back(a + 1);
        indices.push_back(b);
        indices.push_back(b + 1);
    }

    // ===== 下キャップ (y = -halfH) =====
    const unsigned int bottomCenter = static_cast<unsigned int>(vertices.size());
    {
        Vertex c{};
        c.pos[0] = 0.0f; c.pos[1] = -halfH; c.pos[2] = 0.0f;
        c.uv[0] = 0.5f; c.uv[1] = 0.5f;
        vertices.push_back(c);

        const unsigned int ringStart = static_cast<unsigned int>(vertices.size());
        for (int i = 0; i <= slice; ++i)
        {
            const float t = static_cast<float>(i) / slice;
            const float theta = XM_2PI * t;
            const float x = std::cos(theta) * radius;
            const float z = std::sin(theta) * radius;

            Vertex v{};
            v.pos[0] = x; v.pos[1] = -halfH; v.pos[2] = z;
            // 中心(0.5,0.5) の平面マッピング
            v.uv[0] = 0.5f + (x / (radius * 2.0f));
            v.uv[1] = 0.5f - (z / (radius * 2.0f));
            vertices.push_back(v);
        }
        for (int i = 0; i < slice; ++i)
        {
            indices.push_back(bottomCenter);
            indices.push_back(ringStart + i);
            indices.push_back(ringStart + i + 1); // 上から見て CCW
        }
    }

    // ===== 上キャップ (y = +halfH) =====
    const unsigned int topCenter = static_cast<unsigned int>(vertices.size());
    {
        Vertex c{};
        c.pos[0] = 0.0f; c.pos[1] = +halfH; c.pos[2] = 0.0f;
        c.uv[0] = 0.5f; c.uv[1] = 0.5f;
        vertices.push_back(c);

        const unsigned int ringStart = static_cast<unsigned int>(vertices.size());
        for (int i = 0; i <= slice; ++i)
        {
            const float t = static_cast<float>(i) / slice;
            const float theta = XM_2PI * t;
            const float x = std::cos(theta) * radius;
            const float z = std::sin(theta) * radius;

            Vertex v{};
            v.pos[0] = x; v.pos[1] = +halfH; v.pos[2] = z;
            v.uv[0] = 0.5f + (x / (radius * 2.0f));
            v.uv[1] = 0.5f - (z / (radius * 2.0f));
            vertices.push_back(v);
        }
        for (int i = 0; i < slice; ++i)
        {
            indices.push_back(topCenter);
            indices.push_back(ringStart + i + 1);
            indices.push_back(ringStart + i);     // 上から見て CW
        }
    }

    MeshBuffer::Description desc{};
    desc.pVtx = vertices.data();
    desc.vtxCount = static_cast<UINT>(vertices.size());
    desc.vtxSize = sizeof(Vertex);
    desc.isWrite = false;
    desc.pIdx = indices.data();
    desc.idxCount = static_cast<UINT>(indices.size());
    desc.idxSize = sizeof(unsigned int);
    desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    auto mb = std::make_unique<MeshBuffer>();
    mb->Create(desc);
    primitives[static_cast<size_t>(PrimitiveMeshTag::Cylinder)] = std::move(mb);
}

void PrimitiveMeshManager::CreateGlassShard()
{
    constexpr Vertex vtx[] = 
    {
    {{-0.5f,  0.0f, 0.0f}, {0.0f, 0.0f}},
    {{ 0.5f,  0.0f, 0.0f}, {1.0f, 0.0f}},
    {{ 0.0f,  1.0f, 0.0f}, {0.5f, 1.0f}},
    };
    constexpr unsigned int idx[] = { 0, 1, 2 };

    MeshBuffer::Description desc{};
    desc.pVtx = vtx;
    desc.vtxCount = static_cast<UINT>(std::size(vtx));
    desc.vtxSize = sizeof(Vertex);
    desc.isWrite = false;
    desc.pIdx = idx;
    desc.idxCount = static_cast<UINT>(std::size(idx));
    desc.idxSize = sizeof(unsigned int);
    desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    auto mb = std::make_unique<MeshBuffer>();
    mb->Create(desc);
    primitives[static_cast<size_t>(PrimitiveMeshTag::GlassShard)] = std::move(mb);
}
