#pragma once
#include "Component.h"
#include "Shader.h"
#include <memory>

class MeshRender : public MonoBehaviour
{
public:
	Transform* t = nullptr;
	Camera* c = nullptr;
	MeshFilter* mf = nullptr;
	std::shared_ptr<VertexShader> m_pVS;
	std::shared_ptr<PixelShader>  m_pPS;
	int PStype = 0;
public:
	MeshRender(int type = 0) : PStype(type){}
	~MeshRender() override = default;
	void Start() override;
	void Draw() override;
	void OnDestroy() override;
private:
	void SetDefaultVS();
	void SetDefaultPS();

	void SetWhitePS();
};
