#pragma once
#include "Component.h"
#include "Vector2.h"
#include <memory>
#include "MeshBuffer.h"
#include "Shader.h"
#include "Texture.h"

class CanvasUI : public MonoBehaviour
{
public:
	//CanvasUI(const char* texPass , Vector2 pos = Vector2::zero(), Vector2 size = Vector2::one(), Vector2 uvPos = Vector2::zero(), Vector2 uvSize = Vector2::one());
	CanvasUI(Texture* texture, Vector2 pos = Vector2::zero(), Vector2 size = Vector2::one(), Vector2 uvPos = Vector2::zero(), Vector2 uvSize = Vector2::one());
	void Start() override;	
	void OnDestroy() override;
	void Draw() override;
public:

	struct Data
	{
		std::shared_ptr<MeshBuffer> mesh;
		DirectX::XMFLOAT4X4 matrix[3];
		DirectX::XMFLOAT4 param[3];
		Texture* texture;
		VertexShader* vs;
		PixelShader* ps;
	};
	Data m_data;
	//bool onAnimetion = false;
	//bool texOwner = false;
public:
	void SetOffset(Vector2 offset);
	void SetPosX(float x);
	void SetPosY(float y);
	void SetSize(Vector2  size);
	void SetUVPos(Vector2  pos);
	void SetUVScale(Vector2  scale);
	void SetColor(DirectX::XMFLOAT4 color);
	void SetVertexShader(Shader* vs);
	void SetPixelShader(Shader* ps);
private:
	void CreateMesh();
	void ensureDefaultShaders();
	void SetMatrix();
	void SetParam();
	static std::shared_ptr<VertexShader> s_defVS;
	static std::shared_ptr<PixelShader>  s_defPS;

};