#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <memory>
#include "Vector2.h"
#include "MeshBuffer.h"
#include "Texture.h"
#include "Shader.h"

// ・Sprite は「UI / 2D 用」の低レベル描画ヘルパ（コンポーネントではない）

class Sprite
{
public:
    // 一度だけ呼ぶ（Game 起動時）
    static void Init();
    static void Uninit();

    // 実際の描画
    static void Draw();

    struct ParamCB
    {
        DirectX::XMFLOAT4 offset_size;    // xy=offset(px), zw=size(px)
        DirectX::XMFLOAT4 uvPos_uvScale;  // xy=uvPos,     zw=uvScale
        DirectX::XMFLOAT4 color;          // RGBA
    };

    static VertexShader* Get3dVS() { return s_3dVS.get(); }
    static PixelShader* Get3dPS() { return s_3dPS.get(); }

private:
    // 共有メッシュ＆デフォルトシェーダ
    static std::unique_ptr<MeshBuffer> s_quad;
    static std::unique_ptr<VertexShader> s_3dVS;
    static std::unique_ptr<PixelShader> s_3dPS;

    static void Create3dVS();
    static void Create3dPS();
};

#endif // __SPRITE_H__