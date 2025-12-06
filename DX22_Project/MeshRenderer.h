#pragma once
#include "Component.h"
#include "ShaderList.h"

class GlassObject;

class MeshRenderer final : public Renderer
{
private:
	Transform* tf = nullptr;
	Camera* cam = nullptr;
	MeshFilter* mf = nullptr;
	Animator* animator = nullptr;
	GlassObject* glass = nullptr;
public:
	ShaderList::VSKind vsKind = ShaderList::VS_WORLD;
	ShaderList::PSKind psKind = ShaderList::PS_LAMBERT; 
public:
	MeshRenderer() = default;
	~MeshRenderer() override = default;
	void Awake() override;
	void Start() override;
	void OnDestroy() override;
	void Render()  override;
private:
	void PrimitiveRender();
	void ModelRender();
};