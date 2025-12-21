// SkyDomeMesh.h
#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

// スカイドーム専用のメッシュクラス
// ・頂点バッファ
// ・インデックスバッファ
// ・インプットレイアウト
// を内部で管理する
class SkyDomeMesh
{
public:
    SkyDomeMesh();
    ~SkyDomeMesh();

    // 初期化
    // device : ID3D11Device
    // slices : 経線方向の分割数
    // stacks : 緯線方向の分割数
    // radius : ローカル空間での半径（基本は 1.0f で作り、ワールド行列で拡大する）
    bool Init(ID3D11Device* device,
        int slices = 32,
        int stacks = 16,
        float radius = 1.0f);

    // 描画
    // context : ID3D11DeviceContext
    // 前提条件:
    //  ・SkyDome 用の VS / PS がすでに Bind されている
    //  ・WVP は SkyDomeShader::SetWVP などで設定済み
    void Draw(ID3D11DeviceContext* context);

private:
    // スカイドーム用頂点データ
    struct Vertex
    {
        DirectX::XMFLOAT3 pos;  // 位置
        DirectX::XMFLOAT3 nor;  // 法線
        DirectX::XMFLOAT4 col;  // 色
        DirectX::XMFLOAT2 uv;   // UV
    };

    ID3D11Buffer* m_pVB;          // 頂点バッファ
    ID3D11Buffer* m_pIB;          // インデックスバッファ
    ID3D11InputLayout* m_pInputLayout; // インプットレイアウト
    UINT                m_indexCount;   // インデックス数

    // コピー禁止
    SkyDomeMesh(const SkyDomeMesh&) = delete;
    SkyDomeMesh& operator=(const SkyDomeMesh&) = delete;
};
