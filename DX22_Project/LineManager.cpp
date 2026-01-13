#include "LineManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "BoxCollider.h"
#include "DirectX.h" // GetDevice / GetContext

using namespace DirectX;

//=====================================================
// ファイル内限定の動的 VB 管理
//=====================================================
namespace
{
    ID3D11Buffer* g_lineVB = nullptr;
    UINT          g_lineVBVertexCapacity = 0;

    inline XMVECTOR V3(float x, float y, float z)
    {
        return XMVectorSet(x, y, z, 1.0f);
    }

    bool EnsureDynamicVB(UINT requiredVertexCount)
    {
        if (requiredVertexCount == 0)
        {
            return false;
        }

        ID3D11Device* device = GetDevice();
        if (!device) return false;

        // 既に十分なサイズが確保されていれば再利用
        if (g_lineVB && g_lineVBVertexCapacity >= requiredVertexCount)
        {
            return true;
        }

        // 古いバッファを破棄
        if (g_lineVB)
        {
            g_lineVB->Release();
            g_lineVB = nullptr;
            g_lineVBVertexCapacity = 0;
        }

        D3D11_BUFFER_DESC bd = {};
        bd.ByteWidth = sizeof(LineManager::LineVertex) * requiredVertexCount;
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        HRESULT hr = device->CreateBuffer(&bd, nullptr, &g_lineVB);
        if (FAILED(hr))
        {
            return false;
        }

        g_lineVBVertexCapacity = requiredVertexCount;
        return true;
    }
}

//=====================================================
// 静的メンバ定義
//=====================================================

LineManager::LineBucket
LineManager::s_buckets[static_cast<std::size_t>(LineGroup::Count)] = {};

VertexShader* LineManager::s_vs = nullptr;
PixelShader* LineManager::s_ps = nullptr;

XMFLOAT4X4 LineManager::s_view = {};
XMFLOAT4X4 LineManager::s_proj = {};

//=====================================================
// 公開 API
//=====================================================

void LineManager::Init()
{
    EnsureShaders();

    // ビュー / プロジェクションは単位行列で初期化
    XMStoreFloat4x4(&s_view, XMMatrixIdentity());
    XMStoreFloat4x4(&s_proj, XMMatrixIdentity());
}

void LineManager::Uninit()
{
    // CPU 側バッファのクリア
    for (auto& b : s_buckets)
    {
        b.cpu.clear();
        b.dirty = false;
    }

    // 動的 VB を解放
    if (g_lineVB)
    {
        g_lineVB->Release();
        g_lineVB = nullptr;
        g_lineVBVertexCapacity = 0;
    }

    // シェーダ解放
    if (s_vs)
    {
        delete s_vs;
        s_vs = nullptr;
    }
    if (s_ps)
    {
        delete s_ps;
        s_ps = nullptr;
    }
}

void LineManager::SetViewProj(const XMFLOAT4X4& view, const XMFLOAT4X4& proj)
{
    // ここで転置しておく（シェーダ側は column-major 前提）
    {
        XMMATRIX v = XMMatrixTranspose(XMLoadFloat4x4(&view));
        XMStoreFloat4x4(&s_view, v);
    }
    {
        XMMATRIX p = XMMatrixTranspose(XMLoadFloat4x4(&proj));
        XMStoreFloat4x4(&s_proj, p);
    }
}

void LineManager::ClearAll()
{
    for (std::size_t i = 0; i < static_cast<std::size_t>(LineGroup::Count); ++i)
    {
        auto& bucket = s_buckets[i];
        bucket.cpu.clear();
        bucket.dirty = true;
    }
}

void LineManager::Clear(LineGroup group)
{
    if (group == LineGroup::Count)
    {
        ClearAll();
        return;
    }

    auto& bucket = s_buckets[static_cast<std::size_t>(group)];
    bucket.cpu.clear();
    bucket.dirty = true;
}

void LineManager::AddLine(const XMFLOAT3& a, const XMFLOAT3& b,
    const XMFLOAT4& color, LineGroup group)
{
    if (group == LineGroup::Count)
    {
        group = LineGroup::General;
    }
    auto& bucket = s_buckets[static_cast<std::size_t>(group)];
    PushLine(bucket, a, b, color);
}

void LineManager::AddPolyline(const std::vector<XMFLOAT3>& points,
    const XMFLOAT4& color, LineGroup group)
{
    if (points.size() < 2) return;

    for (std::size_t i = 0; i + 1 < points.size(); ++i)
    {
        AddLine(points[i], points[i + 1], color, group);
    }
}

void LineManager::AddBoxCollider(const BoxCollider* bc,
    const XMFLOAT4& color,
    LineGroup group)
{
    if (!bc) return;

    GameObject* go = bc->gameObject;
    if (!go) return;

    Transform* tf = go->transform;
    if (!tf)
    {
        tf = go->GetComponent<Transform>();
        if (!tf) return;
    }

    // ワールド行列（S*R*T 想定）
    const XMFLOAT4X4 M4 = tf->GetMatrix();
    const XMMATRIX   M = XMLoadFloat4x4(&M4);

    // ローカル中心と半サイズ
    const XMFLOAT3 c = { bc->center.x, bc->center.y, bc->center.z };
    const XMFLOAT3 e = { bc->size.x * 0.5f, bc->size.y * 0.5f, bc->size.z * 0.5f };

    // ローカル8頂点
    XMVECTOR L[8] = {
        V3(c.x - e.x, c.y - e.y, c.z - e.z),
        V3(c.x + e.x, c.y - e.y, c.z - e.z),
        V3(c.x - e.x, c.y - e.y, c.z + e.z),
        V3(c.x + e.x, c.y - e.y, c.z + e.z),
        V3(c.x - e.x, c.y + e.y, c.z - e.z),
        V3(c.x + e.x, c.y + e.y, c.z - e.z),
        V3(c.x - e.x, c.y + e.y, c.z + e.z),
        V3(c.x + e.x, c.y + e.y, c.z + e.z),
    };

    // ワールド8頂点
    XMFLOAT3 W[8];
    for (int i = 0; i < 8; ++i)
    {
        XMVECTOR wv = XMVector3Transform(L[i], M);
        XMStoreFloat3(&W[i], wv);
    }

    // 対象バケット
    if (group == LineGroup::Count)
    {
        group = LineGroup::BoxCollider;
    }
    auto& bucket = s_buckets[static_cast<std::size_t>(group)];

    // 12本の辺を追加
    PushLine(bucket, W[0], W[1], color); // 底面
    PushLine(bucket, W[1], W[3], color);
    PushLine(bucket, W[3], W[2], color);
    PushLine(bucket, W[2], W[0], color);

    PushLine(bucket, W[4], W[5], color); // 上面
    PushLine(bucket, W[5], W[7], color);
    PushLine(bucket, W[7], W[6], color);
    PushLine(bucket, W[6], W[4], color);

    PushLine(bucket, W[0], W[4], color); // 側面
    PushLine(bucket, W[1], W[5], color);
    PushLine(bucket, W[2], W[6], color);
    PushLine(bucket, W[3], W[7], color);
}

void LineManager::AddBoxCollider(GameObject* go,
    const XMFLOAT4& color,
    LineGroup group)
{
    if (!go) return;

    auto* bc = go->GetComponent<BoxCollider>();
    if (!bc) return;

    AddBoxCollider(bc, color, group);
}

void LineManager::Draw(LineGroup group)
{
    // 何もラインが無ければ何もしない
    bool hasAny = false;
    if (group == LineGroup::Count)
    {
        for (std::size_t i = 0; i < static_cast<std::size_t>(LineGroup::Count); ++i)
        {
            if (!s_buckets[i].cpu.empty())
            {
                hasAny = true;
                break;
            }
        }
    }
    else
    {
        const auto& bucket = s_buckets[static_cast<std::size_t>(group)];
        hasAny = !bucket.cpu.empty();
    }

    if (!hasAny) return;

    EnsureShaders();
    if (!s_vs || !s_ps) return;

    // シェーダバインド
    s_vs->Bind();
    s_ps->Bind();

    // VP 行列を VS の b0 に書き込む
    Camera* c = Camera::Main();
    SetViewProj(c->GetView(), c->GetProjection());

    struct VP
    {
        XMFLOAT4X4 view;
        XMFLOAT4X4 proj;
    } vp = { s_view, s_proj };
    s_vs->WriteBuffer(0, &vp);

    // 指定されたレイヤーを描画
    auto drawBucket = [](LineBucket& bucket)
        {
            if (bucket.cpu.empty()) return;
            BuildDynamicVB(bucket);
        };

    if (group == LineGroup::Count)
    {
        for (std::size_t i = 0; i < static_cast<std::size_t>(LineGroup::Count); ++i)
        {
            drawBucket(s_buckets[i]);
        }
    }
    else
    {
        drawBucket(s_buckets[static_cast<std::size_t>(group)]);
    }
}

//=====================================================
// 内部ユーティリティ
//=====================================================

void LineManager::EnsureShaders()
{
    if (s_vs && s_ps) return;

    const char* vsCode = R"EOT(
struct VS_IN {
    float3 pos   : POSITION;
    float4 color : COLOR0;
};
struct VS_OUT {
    float4 pos   : SV_POSITION;
    float4 color : COLOR0;
};
cbuffer VP : register(b0)
{
    float4x4 view;
    float4x4 proj;
};
VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    float4 p = float4(vin.pos, 1.0f);
    p = mul(p, view);
    p = mul(p, proj);
    vout.pos = p;
    vout.color = vin.color;
    return vout;
}
)EOT";

    const char* psCode = R"EOT(
struct PS_IN {
    float4 pos   : SV_POSITION;
    float4 color : COLOR0;
};
float4 main(PS_IN pin) : SV_TARGET
{
    return pin.color;
}
)EOT";

    if (!s_vs)
    {
        s_vs = new VertexShader();
        s_vs->Compile(vsCode);
    }
    if (!s_ps)
    {
        s_ps = new PixelShader();
        s_ps->Compile(psCode);
    }
}

void LineManager::BuildDynamicVB(LineBucket& bucket)
{
    if (bucket.cpu.empty())
    {
        bucket.dirty = false;
        return;
    }

    ID3D11DeviceContext* ctx = GetContext();
    if (!ctx)
    {
        bucket.dirty = false;
        return;
    }

    const UINT vertexCount = static_cast<UINT>(bucket.cpu.size());
    if (!EnsureDynamicVB(vertexCount))
    {
        bucket.dirty = false;
        return;
    }

    // VB 書き込み
    D3D11_MAPPED_SUBRESOURCE mapped = {};
    HRESULT hr = ctx->Map(g_lineVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    if (FAILED(hr))
    {
        bucket.dirty = false;
        return;
    }

    const size_t bytes = sizeof(LineVertex) * vertexCount;
    memcpy_s(mapped.pData, bytes, bucket.cpu.data(), bytes);
    ctx->Unmap(g_lineVB, 0);

    // パイプライン設定
    const UINT stride = sizeof(LineVertex);
    const UINT offset = 0;

    ctx->IASetVertexBuffers(0, 1, &g_lineVB, &stride, &offset);
    ctx->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
    ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

    // 描画
    ctx->Draw(vertexCount, 0);

    bucket.dirty = false;
}

void LineManager::PushLine(LineBucket& bucket,
    const XMFLOAT3& a,
    const XMFLOAT3& b,
    const XMFLOAT4& color)
{
    LineVertex va{};
    va.pos = a;
    va.color = color;

    LineVertex vb{};
    vb.pos = b;
    vb.color = color;

    bucket.cpu.push_back(va);
    bucket.cpu.push_back(vb);
    bucket.dirty = true;
}
