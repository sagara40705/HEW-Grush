#pragma once
#include "Component.h"
#include "ShaderList.h"

class GlassObject; class GlassShard;

class MeshRenderer final : public Renderer
{
private:
	Transform* tf = nullptr;
	Camera* cam = nullptr;
	MeshFilter* mf = nullptr;
	Animator* animator = nullptr;
	GlassObject* glass = nullptr;
	GlassShard* shard = nullptr;

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
public://追加点
	// ピクセルシェーダだけ上書きしたいとき用
	void SetPixelShaderKind(int ps);  // ← 型はあなたのpsKindに合わせて後で直す
private:
	bool usePSOverride_ = false;
	int  psKind_ = 0; // ← 型と初期値はMeshRenderer.cppのpsKindに合わせる
};