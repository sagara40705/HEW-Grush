#pragma once
#include "Component.h"
#include "Shader.h"

class Transform;
class Camera;
class ModelComponent;

class ModelRenderer : public MonoBehaviour
{
public:
    Transform* t = nullptr;
    Camera* c = nullptr;
    ModelComponent* mc = nullptr;
    VertexShader* m_pVS = nullptr;
    PixelShader* m_pPS = nullptr;

public:
    ModelRenderer() {}
    ~ModelRenderer() override = default;

    void Start() override;
    void Draw()  override;
    void OnDestroy() override;

private:
    void EnsureShaders();
};
