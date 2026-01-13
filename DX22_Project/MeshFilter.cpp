#include "MeshFilter.h"
#include "LineManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "DebugLog.h"

using namespace DirectX;
void MeshFilter::DebugDrawBounds() const
{
	const auto& m = gameObject->transform->GetMatrix(); //
    Vector3 center{ m._41, m._42, m._43 };

    center += boundsOffset;

	const float r = boundsRadius;
  
	//if (r <= 0.0f) return;

    const XMFLOAT4& color = kBlue;
    LineManager::LineGroup group = LineManager::LineGroup::General;

    // ŠÈˆÕ”Å‚Æ‚µ‚Äu’†S } ”¼Œav‚Ì—§•û‘Ì AABB ‚ðì‚é
    XMFLOAT3 p[8] = {
        { center.x - r, center.y - r, center.z - r }, // 0
        { center.x + r, center.y - r, center.z - r }, // 1
        { center.x - r, center.y + r, center.z - r }, // 2
        { center.x + r, center.y + r, center.z - r }, // 3
        { center.x - r, center.y - r, center.z + r }, // 4
        { center.x + r, center.y - r, center.z + r }, // 5
        { center.x - r, center.y + r, center.z + r }, // 6
        { center.x + r, center.y + r, center.z + r }, // 7
    };

    // 12 –{‚ÌƒGƒbƒW‚ð LineManager ‚É“o˜^
    // near –Ê
    LineManager::AddLine(p[0], p[1], color, group);
    LineManager::AddLine(p[1], p[3], color, group);
    LineManager::AddLine(p[3], p[2], color, group);
    LineManager::AddLine(p[2], p[0], color, group);

    // far –Ê
    LineManager::AddLine(p[4], p[5], color, group);
    LineManager::AddLine(p[5], p[7], color, group);
    LineManager::AddLine(p[7], p[6], color, group);
    LineManager::AddLine(p[6], p[4], color, group);

    // ‘¤–Ê 4 –{
    LineManager::AddLine(p[0], p[4], color, group);
    LineManager::AddLine(p[1], p[5], color, group);
    LineManager::AddLine(p[2], p[6], color, group);
    LineManager::AddLine(p[3], p[7], color, group);
}
