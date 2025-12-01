#include "CanvasUI.h"
#include "Camera.h"
#include "Defines.h"
using namespace DirectX;

// -------------------------------------------------------
// CanvasUI
// 2D UI (画像・テキストなど) を画面に描画するコンポーネント
// 位置・サイズ・UV・色を CPU 側で保持し、毎フレームシェーダに送る
// -------------------------------------------------------

// 静的メンバ：デフォルトのシェーダを全 UI で共有
std::shared_ptr<VertexShader> CanvasUI::s_defVS;
std::shared_ptr<PixelShader>  CanvasUI::s_defPS;

// -------------------------------------------------------
// コンストラクタ
// texture : UI に使用するテクスチャ
// pos     : UI の表示座標（ピクセル）
// size    : テクスチャに対する比率(1.0 = テクスチャサイズそのまま)
// uvPos   : UV の開始位置
// uvSize  : UV の拡大率
// -------------------------------------------------------
CanvasUI::CanvasUI(Texture* texture, Vector2 pos, Vector2 size, Vector2 uvPos, Vector2 uvSize)
{
    m_data.texture = texture;

    // UI の左上座標設定
    SetOffset(pos);

    // UI の横幅・縦幅をテクスチャ比率で設定
    SetSize(size);

    // UV 座標設定（スプライトの切り出しなどに使う）
    SetUVPos(uvPos);
    SetUVScale(uvSize);

    // UI の色 (RGBA) : 初期値 = 白(透明なし)
    m_data.param[2] = { 1,1,1,1 };
}

// -------------------------------------------------------
// Start(): 初期化処理
//  - メッシュ作成 (2 ポリゴン分の四角形)
//  - デフォルトシェーダをセット
// -------------------------------------------------------
void CanvasUI::Start()
{
    CreateMesh();
    ensureDefaultShaders();

    // デフォルトシェーダ割り当て
    m_data.vs = s_defVS.get();
    m_data.ps = s_defPS.get();
}

// -------------------------------------------------------
// OnDestroy(): UI削除時
// ポインタ無効化
// -------------------------------------------------------
void CanvasUI::OnDestroy()
{
    m_data.texture = nullptr;
    m_data.vs = nullptr;
    m_data.ps = nullptr;
}

// -------------------------------------------------------
// Draw(): 描画処理本体
//  - 行列更新
//  - GPU に UI パラメータ送信
//  - メッシュ描画
// -------------------------------------------------------
void CanvasUI::Draw()
{
    // 描画できない状況なら return
    if (!m_data.mesh || !m_data.vs || !m_data.ps || !m_data.texture) return;

    // 正しい UI ワールド変換行列をセット
    SetMatrix();

    // (VSバッファ b0) 行列をGPUへ送信
    m_data.vs->WriteBuffer(0, m_data.matrix);
    // (VSバッファ b1) UIパラメータをGPUへ送信
    m_data.vs->WriteBuffer(1, m_data.param);

    // シェーダとテクスチャをセットして描画
    m_data.vs->Bind();
    m_data.ps->SetTexture(0, m_data.texture);
    m_data.ps->Bind();

    // メッシュ描画実行
    m_data.mesh->Draw();
}

// =============================
// UI設定系関数
// 位置・サイズ・UV・色など
// =============================

// 左上座標(ピクセル)設定
void CanvasUI::SetOffset(Vector2  offset)
{
    m_data.param[0].x = offset.x;
    m_data.param[0].y = offset.y;
}
void CanvasUI::SetPosX(float x)
{
    m_data.param[0].x = x;
}
void CanvasUI::SetPosY(float y)
{
    m_data.param[0].y = y;
}

// UIのサイズ設定(テクスチャ比率)
// 実際のピクセルサイズは後で SetParam() で計算
void CanvasUI::SetSize(Vector2 size)
{
    m_data.param[0].z = size.x;
    m_data.param[0].w = size.y;
    SetParam();
}

// UV開始位置設定
void CanvasUI::SetUVPos(Vector2 pos)
{
    m_data.param[1].x = pos.x;
    m_data.param[1].y = pos.y;
}

// UV拡大率設定
void CanvasUI::SetUVScale(Vector2 scale)
{
    m_data.param[1].z = scale.x;
    m_data.param[1].w = scale.y;
}

// 色設定
void CanvasUI::SetColor(DirectX::XMFLOAT4 color)
{
    m_data.param[2] = color;
}

void CanvasUI::SetVertexShader(Shader* vs) {}
void CanvasUI::SetPixelShader(Shader* ps) {}

// -------------------------------------------------------
// CreateMesh(): UI用の四角形メッシュ作成
// 頂点：左上/左下/右上/右下 の4頂点 TRIANGLESTRIP
// -------------------------------------------------------
void CanvasUI::CreateMesh()
{
    struct Vertex
    {
        float pos[3]; // XY位置 + Z(常に0)
        float uv[2];  // UV座標
    } vtx[] = {
        {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, // 左上
        {{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}, // 左下
        {{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}, // 右上
        {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}}, // 右下
    };

    MeshBuffer::Description desc = {};
    desc.pVtx = vtx;
    desc.vtxSize = sizeof(Vertex);
    desc.vtxCount = _countof(vtx);
    desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

    m_data.mesh = std::make_shared<MeshBuffer>();
    m_data.mesh->Create(desc);
}

// -------------------------------------------------------
// ensureDefaultShaders():
// UI専用デフォルトシェーダを遅延コンパイル
// -------------------------------------------------------
void CanvasUI::ensureDefaultShaders()
{
    if (s_defVS && s_defPS) return;

    // ------- 頂点シェーダ -------
    // UI座標 → 画面へ正射影 する
    const char* VS = R"EOT(
struct VS_IN {
    float3 pos : POSITION0;
    float2 uv : TEXCOORD0;
};
struct VS_OUT {
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};
cbuffer Matrix : register(b0) {
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};
cbuffer Param : register(b1) {
    float2 offset;
    float2 size;
    float2 uvPos;
    float2 uvScale;
    float4 color;
};
VS_OUT main(VS_IN vin) {
    VS_OUT vout;
    // (0~1) の四角形を size ピクセルに拡大し offset に配置
    vout.pos = float4(vin.pos, 1.0f);
    vout.pos.xy *= size;
    vout.pos.xy += offset;

    // UI は常に画面固定 → world/view は単位行列
    vout.pos = mul(vout.pos, world);
    vout.pos = mul(vout.pos, view);
    vout.pos = mul(vout.pos, proj);

    vout.uv = vin.uv * uvScale + uvPos;
    vout.color = color;
    return vout;
})EOT";

    // ------- ピクセルシェーダ -------
    // Texture × Color → 最終色
    const char* PS = R"EOT(
struct PS_IN {
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};
Texture2D tex : register(t0);
SamplerState samp : register(s0);
float4 main(PS_IN pin) : SV_TARGET {
    return tex.Sample(samp, pin.uv) * pin.color;
})EOT";

    s_defVS = std::make_shared<VertexShader>(); s_defVS->Compile(VS);
    s_defPS = std::make_shared<PixelShader>();  s_defPS->Compile(PS);
}

// -------------------------------------------------------
// SetMatrix(): UI の投影行列を設定
// 正射影(Orthographic) を使い 1px = 1Unit で描画
// -------------------------------------------------------
void CanvasUI::SetMatrix()
{
    const float W = SCREEN_WIDTH;
    const float H = SCREEN_HEIGHT;

    auto I = XMMatrixIdentity();
    auto P = XMMatrixOrthographicOffCenterLH(0.f, W, H, 0.f, 0.f, 1.f);

    // UIは画面固定なので world と view = I に
    XMStoreFloat4x4(&m_data.matrix[0], XMMatrixTranspose(I)); // world
    XMStoreFloat4x4(&m_data.matrix[1], XMMatrixTranspose(I)); // view
    XMStoreFloat4x4(&m_data.matrix[2], XMMatrixTranspose(P)); // proj
}

// -------------------------------------------------------
// SetParam():
// テクスチャ実ピクセルサイズに応じて UI サイズ更新
// -------------------------------------------------------
void CanvasUI::SetParam()
{
    float texW = (float)m_data.texture->GetWidth();
    float texH = (float)m_data.texture->GetHeight();

    // param[0] = (offsetX, offsetY, width, height)
    m_data.param[0].z = texW * m_data.param[0].z;
    m_data.param[0].w = texH * m_data.param[0].w;
}

//CanvasUI::CanvasUI(const char* texPass, Vector2 pos, Vector2 size, Vector2 uvPos, Vector2 uvSize)
//{
//	m_data.texture = new Texture();
//	m_data.texture->Create(texPass);
//	texOwner = true;
//	SetOffset(pos);
//	SetSize(size);
//	SetParam();
//	SetUVPos(uvPos);
//	SetUVScale(uvSize);
//	m_data.param[2] = { 1, 1, 1, 1 };
//}