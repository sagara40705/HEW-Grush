#pragma once
#include "Vector3.h"
#include "Component.h"
#include "DebugLog.h"

class Camera : public Component
{
public:
    // ==== 定数定義 ====
    static constexpr Vector3 initPos()      noexcept { return { 0, 5, -10 }; }
    static constexpr Vector3 initRotate()   noexcept { return { 20, 0, 0 }; }
    static constexpr DirectX::XMFLOAT3 defaltPos = { 0.0f, 1.0f, -3.0f };
    static constexpr DirectX::XMFLOAT3 defaltRotate = { 30.0f, 0.0f,  0.0f };
    static constexpr float DEFAULT_FOVY = DirectX::XM_PIDIV4; // 45°
    static constexpr float DEFAULT_NEAR_Z = 0.1f;
    static constexpr float DEFAULT_FAR_Z = 1000.0f;
    // レンズ（左手系前提）
    float fovY = DEFAULT_FOVY; // 45°
    float aspect = 16.0f / 9.0f;
    float nearZ = DEFAULT_NEAR_Z;
    float farZ = DEFAULT_FAR_Z;

private:
    DirectX::XMFLOAT4X4 view_{};       // 毎フレーム Transform から再計算
    DirectX::XMFLOAT4X4 proj_{};       // パラメータ変更時のみ再計算
    bool projDirty_ = true;

    static Camera* s_main;

public:
    Camera() = default;
    ~Camera() override = default;

    void Start() override
    {
        // 最初に起動した Camera をメインに
        if (s_main == nullptr) s_main = this;
        Debug::LogValue("Camera-Start", (s_main == nullptr));
        RebuildProjectionIfDirty();
    }
    void OnDestroy();

    static Camera* Main() { return s_main; }
    void ForceMain() { s_main = this; }

    // ----------------- レンズ設定 -----------------
    void SetPerspective(float fovYRad, float aspectRatio, float nearPlane, float farPlane);
    void SetFovY(float  fovYRad) { fovY = fovYRad; projDirty_ = true;}
    void SetAspect(float aspectRatio) { aspect = aspectRatio; projDirty_ = true; }

    // ----------------- 取得系 -----------------
    const DirectX::XMFLOAT4X4& GetView();       //View
    const DirectX::XMFLOAT4X4& GetProjection(); //Proj
    DirectX::XMFLOAT4X4 GetViewProjection();    //VP
private:
    void RebuildViewFromTransform();
    void RebuildProjectionIfDirty();
};
