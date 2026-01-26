#include "Canvas.h"
#include <cassert>

using namespace DirectX;

Canvas* Canvas::s_main = nullptr;

Canvas::Canvas()
{
    // デフォルト射影行列（初期値 1280x720）
    SetScreenSize(screenWidth, screenHeight);
}

void Canvas::Awake()
{
	s_main = this;

	assert(s_main != nullptr && "Canvas::Awake: Main canvas already exists.");
}

void Canvas::OnDestroy()
{
    if (s_main == this )
    {
        s_main = nullptr;
    }

}

void Canvas::SetScreenSize(int w, int h)
{
    screenWidth = w;
    screenHeight = h;

    // 左上(0,0)〜右下(w,h) を NDC(-1,1)〜(1,-1) に写す正射影。
    // 「画面左上 (0,0)」を UI 座標の原点として扱いたいので
    // OrthographicOffCenterLH の上下は (top = h, bottom = 0) にする。
    XMMATRIX P = XMMatrixOrthographicOffCenterLH(
        0.0f, static_cast<float>(w),   // left,  right
        static_cast<float>(h), 0.0f,   // top,   bottom
        0.0f, 1.0f                      // near,  far
    );
    XMStoreFloat4x4(&proj, P);
}
