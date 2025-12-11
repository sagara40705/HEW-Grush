#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"
#include "LineManager.h"
using namespace DirectX;

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

Ray Camera::GetForwardRay() const
{
    // Transform 取得
    auto* tf = gameObject->GetComponent<Transform>();
    assert(tf && "Camera requires Transform component");

    // ワールド位置と forward 方向を取得
    const XMFLOAT3 posF = tf->GetWorldPosition();
    const XMFLOAT3 fwdF = tf->Forward();

    Vector3 origin{ posF.x, posF.y, posF.z };
    Vector3 dir{ fwdF.x, fwdF.y, fwdF.z };
    dir.Normalize(); 

    return { origin, dir };
}

void Camera::DebugDrawFrustum()
{
    // ----- 1) view / proj を取得 -----
    const XMFLOAT4X4& viewF = GetView();      // 変換は内部で Rebuild
    const XMFLOAT4X4& projF = GetProjection();

    XMMATRIX view = XMLoadFloat4x4(&viewF);
    XMMATRIX proj = XMLoadFloat4x4(&projF);

    // ----- 2) clip -> world の逆行列を作成 -----
    XMMATRIX vp = XMMatrixMultiply(view, proj);
    XMMATRIX invVP = XMMatrixInverse(nullptr, vp);

    // ----- 3) NDC の 8 頂点を world 空間へ変換 -----
    XMFLOAT3 c[8];
    int idx = 0;

    const float ndcZ[2] = { 0.0f, 1.0f }; // DirectX NDC: near = 0, far = 1

    const float shrink = 0.8f;//デバック用

    for (int iz = 0; iz < 2; ++iz)
    {
        for (int iy = 0; iy < 2; ++iy)
        {
            for (int ix = 0; ix < 2; ++ix)
            {
                float x = (ix == 0 ? -1.0f : 1.0f)  * shrink; //デバック用
                float y = (iy == 0 ? -1.0f : 1.0f)  * shrink; //デバック用
                float z = ndcZ[iz];

                XMVECTOR p = XMVectorSet(x, y, z, 1.0f);
                p = XMVector4Transform(p, invVP);
                p = XMVectorScale(p, 1.0f / XMVectorGetW(p));

                XMStoreFloat3(&c[idx++], p);
            }
        }
    }

    // 分かりやすい参照名にしておく
    XMFLOAT3 nLB = c[0]; // near  Left-Bottom
    XMFLOAT3 nRB = c[1];
    XMFLOAT3 nLT = c[2];
    XMFLOAT3 nRT = c[3];
    XMFLOAT3 fLB = c[4]; // far   Left-Bottom
    XMFLOAT3 fRB = c[5];
    XMFLOAT3 fLT = c[6];
    XMFLOAT3 fRT = c[7];

    using LG = LineManager::LineGroup;

    // 必要なら毎回クリア
    //（General を他と共用している場合は注意）
    LineManager::Clear(LG::General);

    const XMFLOAT4 colNear = { 1,0,0,1 };
    const XMFLOAT4 colFar = { 1,0,0,1 };
    const XMFLOAT4 colSide = { 1,0,0,1 };

    // ----- 4) near 平面 -----
    LineManager::AddLine(nLB, nRB, colNear, LG::General);
    LineManager::AddLine(nRB, nRT, colNear, LG::General);
    LineManager::AddLine(nRT, nLT, colNear, LG::General);
    LineManager::AddLine(nLT, nLB, colNear, LG::General);

    // ----- 5) far 平面 -----
    LineManager::AddLine(fLB, fRB, colFar, LG::General);
    LineManager::AddLine(fRB, fRT, colFar, LG::General);
    LineManager::AddLine(fRT, fLT, colFar, LG::General);
    LineManager::AddLine(fLT, fLB, colFar, LG::General);

    // ----- 6) 側面 4 本 -----
    LineManager::AddLine(nLB, fLB, colSide, LG::General);
    LineManager::AddLine(nRB, fRB, colSide, LG::General);
    LineManager::AddLine(nLT, fLT, colSide, LG::General);
    LineManager::AddLine(nRT, fRT, colSide, LG::General);
}

bool Camera::IsVisibleSphere(const Vector3& center, float radius) const
{
    // view 行列を取得（内部で Transform から再計算される）:contentReference[oaicite:1]{index=1}
    Camera* self = const_cast<Camera*>(this);
    const XMFLOAT4X4& viewF = self->GetView();
    XMMATRIX view = XMLoadFloat4x4(&viewF);

    // ワールド座標の center をビュー空間へ
    XMVECTOR cWorld = XMVectorSet(center.x, center.y, center.z, 1.0f);
    XMVECTOR cViewV = XMVector4Transform(cWorld, view);
    float x = XMVectorGetX(cViewV);
    float y = XMVectorGetY(cViewV);
    float z = XMVectorGetZ(cViewV);

    // z がマイナス（カメラの後ろ）なら不可視扱い
    if (z <= 0.0f)
        return false;

    // 近平面・遠平面（前後方向）
    if (z + radius < nearZ) return false;
    if (z - radius > farZ)  return false;

    // 上下左右（fovY と aspect から算出）
    float tanY = std::tan(fovY * 0.5f);
    float tanX = tanY * aspect;

    const float shrink = 0.8f;

    // 左右
    if (std::fabs(x) - radius > z * tanX * shrink) return false;
    // 上下
    if (std::fabs(y) - radius > z * tanY * shrink) return false;

    return true;
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
