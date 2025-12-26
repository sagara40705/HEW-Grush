#pragma once
#include "Component.h"
#include "Shader.h"
#include "Texture.h"
#include <memory>

class UIFrostRenderer : public Renderer
{
private:
	static std::unique_ptr<VertexShader> vs_;
	static std::unique_ptr<PixelShader>  ps_;
	static Texture*	tex_;
	Canvas* canvas = nullptr;
public:
	void Start() override;
	void OnDestroy() override;
	void Render() override;
private:
	void CreateVS();
	void CreatePS();
private:
	struct Params
	{
		DirectX::XMFLOAT4 tint_opacity = { 0.05f, 0.08f, 0.12f, 0.35f }; // rgb=tint, a=opacity
		DirectX::XMFLOAT4 vignette = { 0.18f, 0.85f, 0.25f, 0.0f };  // strength, radius, softness
		DirectX::XMFLOAT4 noise = { 0.08f, 220.0f, 0.02f, 0.0f }; // strength, tileScale, speed, timeSec
		DirectX::XMFLOAT4 chroma = { 0.12f, 0.0f, 0.0f, 0.0f };    // amount
	};
	Params  params_{};
	float   timeSec_ = 0.0f;
};