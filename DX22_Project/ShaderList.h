#ifndef __SHADER_LIST_H__
#define __SHADER_LIST_H__

#include "Shader.h"
#include "Model.h"


class ShaderList
{
	// 定義
public:
	enum VSKind
	{
		VS_PRIM,
		VS_WORLD, // SetWVP
		VS_ANIME, // SetWVP,SetBones
		VS_SPRITE,
		
		VS_KIND_MAX
	};
	enum PSKind
	{
		PS_PRIM,
		PS_LAMBERT, //SetMaterial, SetLight
		PS_SPECULAR, //SetMaterial, SetLight, SetCameraPos
		PS_TOON, // SetMaterial, SetLight
		PS_FOG, // SetMaterial, SetLight,SetFog
		PS_SPRITE,
		PS_GLASS,
		PS_KIND_MAX
	};


public:
	ShaderList();
	~ShaderList();

	static void Init();
	static void Uninit();

	// シェーダー設定
	static VertexShader* GetVS(VSKind vs);
	static PixelShader* GetPS(PSKind ps);

	// 定数バッファへの設定
	static void SetWVP(DirectX::XMFLOAT4X4* wvp);
	static void SetBones(DirectX::XMFLOAT4X4* bones200);
	static void SetMaterial(const Model::Material& material);
	static void SetLight(DirectX::XMFLOAT4 color, DirectX::XMFLOAT3 dir);
	static void SetCameraPos(const DirectX::XMFLOAT3 pos);
	static void SetFog(DirectX::XMFLOAT4 color, float start, float range);
	static void SetGlassParam(const DirectX::XMFLOAT4& tint, float fresnelPower, float reflectionStrength, float transmissionStrength);
	
private:
	static void MakePrimVS();
	static void MakeWorldVS();
	static void MakeAnimeVS();
	static void MakeSpriteVS();
	static void MakeShardVS();
private:
	static void MakePrimPS();
	static void MakeLambertPS();
	static void MakeSpecularPS();
	static void MakeToonPS();
	static void MakeFogPS();
	static void MakeSpritePS();
	static void MakeGlassPS();

private:
	static VertexShader* m_pVS[VS_KIND_MAX];
	static PixelShader* m_pPS[PS_KIND_MAX];
	
};

#endif // __SHADER_LIST_H__