#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <memory>
#include "Vector2.h"
#include "MeshBuffer.h"
#include "Texture.h"

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

private:
    // 共有メッシュ＆デフォルトシェーダ
    static std::unique_ptr<MeshBuffer> s_quad;
};

#endif // __SPRITE_H__
