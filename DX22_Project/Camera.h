#pragma once
#include <vector>
//#include <typeindex>
#include <cstdint>
#include "Vector3.h"
#include "Component.h"

enum class CameraTag : uint8_t
{
    Main,
    UI,
    MiniMap,
    Debug,
    Custom,
};

class Camera : public Component
{
public:
    static constexpr Vector3 kInitPos = { 0.f, 3.f, -6.f };
    static constexpr Vector3 kInitRotate = { 30.f, 0.f, 0.f };

    static constexpr float DEFAULT_FOVY = DirectX::XM_PIDIV4; // 45
    static constexpr float DEFAULT_NEAR_Z = 0.1f;
    static constexpr float DEFAULT_FAR_Z = 1000.0f;

    // �����Y�i����n�O��j
    float fovY = DEFAULT_FOVY; // 45
    float aspect = 16.0f / 9.0f;
    float nearZ = DEFAULT_NEAR_Z;
    float farZ = DEFAULT_FAR_Z;
    CameraTag tag = CameraTag::Main;

private:
    DirectX::XMFLOAT4X4 view_{};       
    DirectX::XMFLOAT4X4 proj_{};      
    bool projDirty_ = true;

    static Camera* s_main;
    static std::vector<Camera*> s_cameras;

public:
    Camera() = default;
    ~Camera() override = default;

    void Awake() override;
    void Start() override
    {
        // �ŏ��ɋN������ Camera �����C����
        if (s_main == nullptr) s_main = this;
        RebuildProjectionIfDirty();
    }
    void OnDestroy();

    static Camera* Main();
    static const std::vector<Camera*>& All() { return s_cameras; }
    void ForceMain() { s_main = this; }

    // ----------------- �����Y�ݒ� -----------------
    void SetPerspective(float fovYRad, float aspectRatio, float nearPlane, float farPlane);
    void SetFovY(float  fovYRad) { fovY = fovYRad; projDirty_ = true; }
    void SetAspect(float aspectRatio) { aspect = aspectRatio; projDirty_ = true; }

    // ----------------- �擾�n -----------------
    const DirectX::XMFLOAT4X4& GetView();       //View
    const DirectX::XMFLOAT4X4& GetProjection(); //Proj
    DirectX::XMFLOAT4X4 GetViewProjection();    //VP
private:
    void RebuildViewFromTransform();
    void RebuildProjectionIfDirty();
};
