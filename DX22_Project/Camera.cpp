#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"

Camera* Camera::s_main = nullptr;
std::vector<Camera*> Camera::s_cameras{};

void Camera::Awake()
{
	s_cameras.push_back(this);
}

void Camera::OnDestroy()
{
	s_main = nullptr;
}

Camera* Camera::Main()
{
	// 優先：tag == Main のカメラ
		for (auto* cam : s_cameras)
		{
			if (cam && cam->tag == CameraTag::Main && cam->IsActiveAndEnabled())
				return cam;
		}

	// フォールバック：最初の有効なカメラ
	for (auto* cam : s_cameras)
	{
		if (cam && cam->IsActiveAndEnabled())
			return cam;
	}

	return nullptr;
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
	auto* tf = gameObject->GetComponent<Transform>();
	assert(tf && "Camera requires Transform component");

	const XMFLOAT4X4& M = tf->GetMatrix();  // 親子込みのworldMatrix

	XMFLOAT3 eyeF{ M._41, M._42, M._43 }; 	// 位置（行4,1〜3）
	XMFLOAT3 fwdF{ M._31, M._32, M._33 };  	// 基底（行3 = forward, 行2 = up）
	XMFLOAT3 upF{ M._21, M._22, M._23 };

	XMVECTOR eye = XMLoadFloat3(&eyeF);
	XMVECTOR fwd = XMLoadFloat3(&fwdF);
	XMVECTOR up = XMLoadFloat3(&upF);

	// 念のため正規化
	fwd = XMVector3Normalize(fwd);
	up = XMVector3Normalize(up);

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
