#pragma once
#include "Component.h"
#include "ShaderList.h"

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
    GlassScript* gs = nullptr;

    // 使用する VS / PS をインスタンスごとに選択可能
    ShaderList::VSKind vsKind = ShaderList::VS_WORLD;
    ShaderList::PSKind psKind = ShaderList::PS_GLASS;

public:
    ModelRenderer() {}
    ~ModelRenderer() override = default;

    void Start() override;
    void Draw()  override;
    void OnDestroy() override;

private:
    void EnsureShaders();
};
