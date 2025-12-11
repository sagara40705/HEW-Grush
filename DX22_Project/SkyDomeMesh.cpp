// SkyDomeMesh.cpp
#include "SkyDomeMesh.h"

#include <vector>
#include <d3dcompiler.h>

// コンストラクタ
SkyDomeMesh::SkyDomeMesh()
    : m_pVB(nullptr)
    , m_pIB(nullptr)
    , m_pInputLayout(nullptr)
    , m_indexCount(0)
{
}

// デストラクタ
SkyDomeMesh::~SkyDomeMesh()
{
    if (m_pVB)
    {
        m_pVB->Release();
        m_pVB = nullptr;
    }
    if (m_pIB)
    {
        m_pIB->Release();
        m_pIB = nullptr;
    }
    if (m_pInputLayout)
    {
        m_pInputLayout->Release();
        m_pInputLayout = nullptr;
    }
}

// 初期化
bool SkyDomeMesh::Init(ID3D11Device* device, int slices, int stacks, float radius)
{
    if (!device)
    {
        return false;
    }

    // 頂点配列とインデックス配列の一時バッファ
    std::vector<Vertex>   vertices;
    std::vector<UINT>     indices;

    // 球の分割数が小さすぎる場合の保険
    if (slices < 3) slices = 3;
    if (stacks < 2) stacks = 2;

    //=========================
    // 頂点データの生成
    //=========================
    // 緯度方向: stacks + 1
    // 経度方向: slices + 1
    for (int stack = 0; stack <= stacks; ++stack)
    {
        // 緯度角 (0 ～ π)
        float v = static_cast<float>(stack) / static_cast<float>(stacks);
        float theta = v * DirectX::XM_PI;

        float sinTheta = sinf(theta);
        float cosTheta = cosf(theta);

        for (int slice = 0; slice <= slices; ++slice)
        {
            float u = static_cast<float>(slice) / static_cast<float>(slices);
            float phi = u * DirectX::XM_2PI;

            float sinPhi = sinf(phi);
            float cosPhi = cosf(phi);

            // 球面上の位置
            float x = radius * sinTheta * cosPhi;
            float y = radius * cosTheta;
            float z = radius * sinTheta * sinPhi;

            DirectX::XMFLOAT3 pos(x, y, z);

            // 法線は球の中心から外向き（半径で割って正規化）
            DirectX::XMFLOAT3 nor;
            if (radius != 0.0f)
            {
                nor.x = x / radius;
                nor.y = y / radius;
                nor.z = z / radius;
            }
            else
            {
                nor = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
            }

            // 色はひとまず白にする（必要に応じて変えてよい）
            DirectX::XMFLOAT4 col(1.0f, 1.0f, 1.0f, 1.0f);

            // UV は経度と緯度から決定
            DirectX::XMFLOAT2 uv(u, v);

            Vertex vtx;
            vtx.pos = pos;
            vtx.nor = nor;
            vtx.col = col;
            vtx.uv = uv;

            vertices.push_back(vtx);
        }
    }

    //=========================
    // インデックスデータの生成
    //=========================
    // stacks 段 × slices 列 の四角形を 2 つの三角形に分割
    int vertsPerRow = slices + 1;
    for (int stack = 0; stack < stacks; ++stack)
    {
        for (int slice = 0; slice < slices; ++slice)
        {
            int i0 = stack * vertsPerRow + slice;
            int i1 = stack * vertsPerRow + (slice + 1);
            int i2 = (stack + 1) * vertsPerRow + slice;
            int i3 = (stack + 1) * vertsPerRow + (slice + 1);

            // 三角形1
            indices.push_back(static_cast<UINT>(i0));
            indices.push_back(static_cast<UINT>(i2));
            indices.push_back(static_cast<UINT>(i1));

            // 三角形2
            indices.push_back(static_cast<UINT>(i1));
            indices.push_back(static_cast<UINT>(i2));
            indices.push_back(static_cast<UINT>(i3));
        }
    }

    m_indexCount = static_cast<UINT>(indices.size());

    //=========================
    // 頂点バッファ作成
    //=========================
    {
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.ByteWidth = static_cast<UINT>(sizeof(Vertex) * vertices.size());

        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = vertices.data();

        HRESULT hr = device->CreateBuffer(&bd, &initData, &m_pVB);
        if (FAILED(hr))
        {
            return false;
        }
    }

    //=========================
    // インデックスバッファ作成
    //=========================
    {
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bd.ByteWidth = static_cast<UINT>(sizeof(UINT) * indices.size());

        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = indices.data();

        HRESULT hr = device->CreateBuffer(&bd, &initData, &m_pIB);
        if (FAILED(hr))
        {
            return false;
        }
    }

    //=========================
    // インプットレイアウト作成
    //=========================
    // SkyDomeShader の VS と同じ入力シグネチャを持つ
    // 最小限の頂点シェーダーをここでコンパイルし、
    // そのバイトコードから InputLayout を生成する。
    const char* vsCode = R"(
        struct VS_IN {
            float3 pos : POSITION;
            float3 nor : NORMAL;
            float4 col : COLOR;
            float2 uv  : TEXCOORD0;
        };
        struct VS_OUT {
            float4 pos : SV_POSITION;
            float3 dir : TEXCOORD0;
            float2 uv  : TEXCOORD1;
        };
        VS_OUT main(VS_IN vin)
        {
            VS_OUT vout;
            // ここでは行列変換は行わず、レイアウト定義だけに使う
            vout.pos = float4(vin.pos, 1.0f);
            vout.dir = vin.pos;
            vout.uv  = vin.uv;
            return vout;
        }
    )";

    ID3DBlob* vsBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;

    HRESULT hr = D3DCompile(
        vsCode,
        strlen(vsCode),
        nullptr,
        nullptr,
        nullptr,
        "main",              // エントリポイント
        "vs_5_0",            // ターゲットプロファイル
        0,
        0,
        &vsBlob,
        &errorBlob);

    if (FAILED(hr))
    {
        if (errorBlob)
        {
            errorBlob->Release();
        }
        return false;
    }

    // 頂点レイアウトの定義
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        // pos : float3
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        // nor : float3
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        // col : float4
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        // uv : float2
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    hr = device->CreateInputLayout(
        layout,
        _countof(layout),
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        &m_pInputLayout);

    vsBlob->Release();
    if (errorBlob)
    {
        errorBlob->Release();
    }

    if (FAILED(hr))
    {
        return false;
    }

    return true;
}

// 描画
void SkyDomeMesh::Draw(ID3D11DeviceContext* context)
{
    if (!context || !m_pVB || !m_pIB || !m_pInputLayout || m_indexCount == 0)
    {
        return;
    }

    // 頂点バッファ設定
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, &m_pVB, &stride, &offset);

    // インデックスバッファ設定
    context->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);

    // プリミティブトポロジ設定（三角形リスト）
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // インプットレイアウト設定
    context->IASetInputLayout(m_pInputLayout);

    // 描画
    context->DrawIndexed(m_indexCount, 0, 0);
}