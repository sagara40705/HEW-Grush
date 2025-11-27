#include "LineManager.h"
#include "Camera.h"
#include "GameObject.h"
#include "BoxCollider.h"
#include "Transform.h"

using namespace DirectX;
static inline XMVECTOR V3(float x, float y, float z) { return XMVectorSet(x, y, z, 1.0f); }

// 静的メンバ定義
std::vector<LineManager::LineVertex> LineManager::s_cpu;
bool          LineManager::s_dirty = false;
MeshBuffer* LineManager::s_vb = nullptr;
VertexShader* LineManager::s_vs = nullptr;
PixelShader* LineManager::s_ps = nullptr;
XMFLOAT4X4    LineManager::s_view = {};
XMFLOAT4X4    LineManager::s_proj = {};

void LineManager::Init() {
    // 行列を単位初期化
    XMStoreFloat4x4(&s_view, XMMatrixIdentity());
    XMStoreFloat4x4(&s_proj, XMMatrixIdentity());
    ensureShaders();
    // 既存資源があれば流用。初回 Draw で VB を構築する。
}

void LineManager::Term() {
    delete s_vb;         s_vb = nullptr;
    delete s_vs;         s_vs = nullptr;
    delete s_ps;         s_ps = nullptr;
    s_cpu.clear();       s_cpu.shrink_to_fit();
    s_dirty = false;
}

void LineManager::SetView(const XMFLOAT4X4& v) { s_view = v; }
void LineManager::SetProjection(const XMFLOAT4X4& p) { s_proj = p; }

void LineManager::AddLine(const XMFLOAT3& a, const XMFLOAT3& b, const XMFLOAT4& c)
{
    s_cpu.push_back({ {a.x,a.y,a.z}, {c.x,c.y,c.z,c.w} });
    s_cpu.push_back({ {b.x,b.y,b.z}, {c.x,c.y,c.z,c.w} });
    s_dirty = true; // 変更あり → 次回 Draw でVB再構築
}

void LineManager::AddBoxCollider(GameObject* go)
{
    if (!go) return;
    auto* bc = go->TryGetComponent<BoxCollider>();
    if (!bc) return;
    AddBoxCollider(bc);
}

void LineManager::AddBoxCollider(const BoxCollider* bc)
{
    if (!bc) return;
    auto* tf = bc->gameObject->TryGetComponent<Transform>();
    if (!tf) return;

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
        V3(c.x - e.x, c.y + e.y, c.z - e.z),
        V3(c.x + e.x, c.y + e.y, c.z - e.z),
        V3(c.x - e.x, c.y - e.y, c.z + e.z),
        V3(c.x + e.x, c.y - e.y, c.z + e.z),
        V3(c.x - e.x, c.y + e.y, c.z + e.z),
        V3(c.x + e.x, c.y + e.y, c.z + e.z),
    };

    // ワールド8頂点
    XMFLOAT3 W[8];
    for (int i = 0; i < 8; ++i) {
        XMStoreFloat3(&W[i], XMVector3Transform(L[i], M));
    }

    // 12辺のインデックス（標準のボックス）
    const int E[12][2] = {
        {0,1},{0,2},{0,4},
        {1,3},{1,5},
        {2,3},{2,6},
        {3,7},
        {4,5},{4,6},
        {5,7},
        {6,7}
    };

    // 12本登録
    for (auto& e12 : E) { AddLine(W[e12[0]], W[e12[1]], kGreen); }
}

void LineManager::AddPolyline(const std::vector<XMFLOAT3>& pts, const XMFLOAT4& color, bool loop)
{
    if (pts.size() < 2) return;
    for (size_t i = 0; i + 1 < pts.size(); ++i)
        AddLine(pts[i], pts[i + 1], color);
    if (loop && pts.size() >= 3)
        AddLine(pts.back(), pts.front(), color);
}

void LineManager::Draw() 
{
    if (!s_vs || !s_ps) ensureShaders();
    if (s_cpu.empty())  return;
    if (s_dirty) buildImmutableVB();

    //SetDepthTest(false);
    SetView(Camera::Main()->GetView());
    SetProjection(Camera::Main()->GetProjection());

    struct Matrices { XMFLOAT4X4 world, view, proj; } m;
    XMStoreFloat4x4(&m.world, XMMatrixTranspose(XMMatrixIdentity()));
    XMStoreFloat4x4(&m.view, XMMatrixTranspose(XMLoadFloat4x4(&s_view)));
    XMStoreFloat4x4(&m.proj, XMMatrixTranspose(XMLoadFloat4x4(&s_proj)));


    s_vs->WriteBuffer(0, &m);
    s_vs->Bind();
    s_ps->Bind();

    // 頂点数は s_cpu.size()
    if (s_vb) s_vb->Draw(static_cast<int>(s_cpu.size()));

    SetDepthTest(true);
}

void LineManager::ClearAll() {
    s_cpu.clear();
    s_dirty = true;
    delete s_vb; s_vb = nullptr; // 次回Drawで再構築される
}

void LineManager::ensureShaders() {
    if (s_vs && s_ps) return;

    const char* VS = R"(
struct VS_IN { float3 pos:POSITION0; float4 color:COLOR0; };
struct VS_OUT{ float4 pos:SV_POSITION; float4 color:COLOR0; };
cbuffer Matrix: register(b0) {
    float4x4 world; float4x4 view; float4x4 proj;
};
VS_OUT main(VS_IN i){
    VS_OUT o;
    float4 p = float4(i.pos,1);
    // worldは使わない（ワールド座標で渡す前提）
    o.pos = mul(mul(p, view), proj);
    o.color = i.color;
    return o;
})";

    const char* PS = R"(
struct PS_IN{ float4 pos:SV_POSITION; float4 color:COLOR0; };
float4 main(PS_IN i):SV_TARGET{ return i.color; })";

    s_vs = new VertexShader(); s_vs->Compile(VS);
    s_ps = new PixelShader();  s_ps->Compile(PS);
}

void LineManager::buildImmutableVB() {
    // 既存を破棄→不変VBで作り直し（以後は更新なしで超軽量）
    delete s_vb; s_vb = nullptr;

    MeshBuffer::Description d = {};
    d.pVtx = s_cpu.data();
    d.vtxCount = static_cast<int>(s_cpu.size());
    d.vtxSize = sizeof(LineVertex);
    d.topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
    d.isWrite = false;           // 不変（更新しない）
    d.pIdx = nullptr;
    d.idxSize = 0;
    d.idxCount = 0;

    s_vb = new MeshBuffer();
    s_vb->Create(d);
    s_dirty = false;
}
