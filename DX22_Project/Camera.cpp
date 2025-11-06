#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"
#include "DebugLog.h"

Camera* Camera::s_main = nullptr;

void Camera::OnDestroy()
{
	s_main = nullptr;
}

void Camera::SetPerspective(float fovYRad, float aspectRatio, float nearPlane, float farPlane)
{
	fovY = fovYRad; aspect = aspectRatio; nearZ = nearPlane; farZ = farPlane;
	projDirty_ = true;
}

const DirectX::XMFLOAT4X4& Camera::GetView()
{
	RebuildViewFromTransform();
	return view_;
}

const DirectX::XMFLOAT4X4& Camera::GetProjection()
{
	RebuildProjectionIfDirty();
	return proj_;
}

DirectX::XMFLOAT4X4 Camera::GetViewProjection()
{
	using namespace DirectX;
	RebuildViewFromTransform();
	RebuildProjectionIfDirty();
	XMMATRIX V = XMLoadFloat4x4(&view_);
	XMMATRIX P = XMLoadFloat4x4(&proj_);
	XMFLOAT4X4 VP{};
	XMStoreFloat4x4(&VP, XMMatrixMultiply(V, P));
	return VP;
}

void Camera::RebuildViewFromTransform()
{
	using namespace DirectX;
	// Transform 要求
	auto* tf = Parent->TryGetComponent<Transform>();
	assert(tf && "Camera requires Transform component");

	const XMFLOAT3 posF = tf->GetPosition();
	const XMFLOAT4 rotQ = tf->GetRotationQuaternion();

	// ローカルの基底ベクトルを回転して、カメラの forward / up を得る
	const XMVECTOR q = XMLoadFloat4(&rotQ);            // 正規化前提（Transformが保持）
	const XMVECTOR fwd = XMVector3Rotate(XMLoadFloat3(&Vector3::XMforward), q); // +Z
	const XMVECTOR up = XMVector3Rotate(XMLoadFloat3(&Vector3::XMup), q);

	const XMVECTOR eye = XMLoadFloat3(&posF);

	// LookTo
	const XMMATRIX V = XMMatrixLookToLH(eye, fwd, up);
	XMStoreFloat4x4(&view_, V);
}

void Camera::RebuildProjectionIfDirty()
{
	if (!projDirty_) return;

	using namespace DirectX;
	const XMMATRIX P = XMMatrixPerspectiveFovLH(fovY, aspect, nearZ, farZ);
	XMStoreFloat4x4(&proj_, P);
	projDirty_ = false;
}
