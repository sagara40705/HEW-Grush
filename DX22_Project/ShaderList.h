#ifndef __SHADER_LIST_H__
#define __SHADER_LIST_H__

#include "Shader.h"
#include "Model.h"
#include "Texture.h"

class ShaderList
{
public:
    enum VSKind
    {
        VS_PRIM,
        VS_WORLD,
        VS_ANIME,
        VS_SPRITE,
        VS_SHARD,
        VS_KIND_MAX
    };

    enum PSKind
    {
        PS_PRIM,
        PS_LAMBERT,
        PS_SPECULAR,
        PS_TOON,
        PS_FOG,
        PS_SPRITE,
        PS_GLASS,
        PS_GLASS_SHARD,
        PS_GLASS_TITLE,
        PS_RAINBOW,
        PS_KIND_MAX
    };

public:
    ShaderList();
    ~ShaderList();

    static void Init();
    static void Uninit();

    static VertexShader* GetVS(VSKind vs);
    static PixelShader* GetPS(PSKind ps);

    static void SetWVP(DirectX::XMFLOAT4X4* wvp);
    static void SetBones(DirectX::XMFLOAT4X4* bones200);
    static void SetMaterial(const Model::Material& material);
    static void SetLight(DirectX::XMFLOAT4 color, DirectX::XMFLOAT3 dir);
    static void SetCameraPos(const DirectX::XMFLOAT3 pos);
    static void SetFog(DirectX::XMFLOAT4 color, float start, float range);

    static void SetGlassParam(const DirectX::XMFLOAT4& tint, float fresnelPower, float reflectionStrength, float transmissionStrength, float v = 0.5f);
    static void SetRainbowTime(float time);

    // Refraction inputs
    static void SetGlassSceneTexture(RenderTarget* sceneColorRT);                 // t1
    static void SetGlassScreenParam(float screenW, float screenH, float refractStrength, float chromaStrength); // b4

private:
    static void MakePrimVS();
    static void MakeWorldVS();
    static void MakeAnimeVS();
    static void MakeSpriteVS();
    static void MakeShardVS();

    static void MakePrimPS();
    static void MakeLambertPS();
    static void MakeSpecularPS();
    static void MakeToonPS();
    static void MakeFogPS();
    static void MakeSpritePS();
    static void MakeGlassPS();

	static void MakeGlassTitlePS();

    static void MakeGlassShardPS();
    static void MakeRainbowPS();

private:
    static VertexShader* m_pVS[VS_KIND_MAX];
    static PixelShader* m_pPS[PS_KIND_MAX];
};

#endif // __SHADER_LIST_H__