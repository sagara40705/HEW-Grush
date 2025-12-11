// SkyDomeShader.h
//======================================================
//  スカイドーム（宇宙風の空）を描画するためのシェーダークラス
//  ・頂点シェーダー / ピクセルシェーダーを1セットだけ持つ
//  ・CPU からは WVP 行列（world / view / proj）だけを送る
//  ・テクスチャは使わず、PS 内でグラデーションと星を生成する
//======================================================

#ifndef __SKYDOME_SHADER_H__
#define __SKYDOME_SHADER_H__

#include <DirectXMath.h>    // XMFLOAT4X4 など
#include <d3d11.h>
#include "Shader.h"         // 既存の VertexShader / PixelShader クラスを利用

//======================================================
//  SkyDomeShader クラス
//  ・Init()   : シェーダーのコンパイルと初期化
//  ・SetWVP() : World / View / Projection 行列を GPU に送る
//  ・Bind()   : VS / PS をパイプラインにバインドする
//======================================================
class SkyDomeShader
{
public:
    SkyDomeShader();
    ~SkyDomeShader();

    // 初期化処理（アプリ起動時やシーン初期化時に一度だけ呼ぶ）
    bool Init();

    // WVP 行列（world / view / proj）を GPU に送る関数
    // ※ world / view / proj は「すでに転置済み」の行列を想定する
    void SetWVP(
        const DirectX::XMFLOAT4X4& world,
        const DirectX::XMFLOAT4X4& view,
        const DirectX::XMFLOAT4X4& proj
    );

    // シェーダーをパイプラインにバインドする（Draw の直前に呼ぶ）
    void Bind();

    // 必要であれば VS / PS を直接参照したい場合用のゲッター
    VertexShader* GetVS() const { return m_pVS; }
    PixelShader* GetPS() const { return m_pPS; }

    struct SkyParam
    {
        int   useTexture;
        float padding[3];
    };

    void SetUseTexture(bool use);
    void SetTexture(ID3D11ShaderResourceView* srv);

private:
    // 頂点シェーダー / ピクセルシェーダー
    VertexShader* m_pVS;
    PixelShader* m_pPS;

    // WVP 用の CPU 側構造体
    // HLSL 側の cbuffer WVP とレイアウトを一致させる（float4x4 を3個）
    struct WVPParam
    {
        DirectX::XMFLOAT4X4 world;
        DirectX::XMFLOAT4X4 view;
        DirectX::XMFLOAT4X4 proj;
    };

    // ★ SRV / サンプラを自前で持つのはやめて、
    //    Texture* だけ持つようにする
    // ★ テクスチャ（SRV）
    ID3D11ShaderResourceView* m_pSkySRV = nullptr;
    ID3D11SamplerState* m_pSampler = nullptr;

    ID3D11Buffer* m_pSkyParamCB = nullptr; // b1 用定数バッファ
   
    bool                      m_useTexture = false;   // フラグ
};

#endif // __SKYDOME_SHADER_H__