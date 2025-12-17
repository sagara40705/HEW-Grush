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
public:
	void Start() override;
	void OnDestroy() override;
	void Render() override;
private:
	void CreateVS();
	void CreatePS();
};