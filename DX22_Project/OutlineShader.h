// OutlineShader.h
//======================================================
//  モデルの「外側だけ」光らせるためのシェーダークラス
//  ・VS/PS を1セットだけ持つ
//  ・CPU からは WVP 行列（world / view / proj）だけを送る
//  ・PS は単色発光（アウトライン色専用）
//======================================================

#ifndef __OUTLINE_SHADER_H__
#define __OUTLINE_SHADER_H__

#include <DirectXMath.h>    // XMFLOAT4X4 など
#include "Shader.h"         // 既存の VertexShader / PixelShader クラスを再利用する

//======================================================
// OutlineShader クラス
//  ・Init()  : シェーダーのコンパイル＆初期化
//  ・SetWVP(): World / View / Projection 行列を GPU に送る
//  ・Bind()  : VS / PS をパイプラインにセット
//======================================================
class OutlineShader
{
public:
    OutlineShader();
    ~OutlineShader();

    // シェーダーの初期化（アプリ起動時 or SceneGame コンストラクタで呼ぶ）
    bool Init();

    // World / View / Projection の3つをシェーダーに送る
    void SetWVP(
        const DirectX::XMFLOAT4X4& world,
        const DirectX::XMFLOAT4X4& view,
        const DirectX::XMFLOAT4X4& proj
    );

    // シェーダーを描画パイプラインにセット（Draw の直前で呼ぶ）
    void Bind();

    // デバッグ用に VS / PS を直接触りたい場合用
    VertexShader* GetVS() const { return m_pVS; }
    PixelShader* GetPS() const { return m_pPS; }

private:
    // 実際のシェーダーオブジェクト
    VertexShader* m_pVS;    // 頂点シェーダ
    PixelShader* m_pPS;    // ピクセルシェーダ

    // WVP 用の CPU 側構造体
    // ※ HLSL の cbuffer WVP とレイアウトを合わせる（float4x4 が 3つ）
    struct WVPParam
    {
        DirectX::XMFLOAT4X4 world;
        DirectX::XMFLOAT4X4 view;
        DirectX::XMFLOAT4X4 proj;
    };
};

#endif // __OUTLINE_SHADER_H__