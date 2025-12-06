#include "GlassShard.h"
#include "GameObject.h"
#include "ShaderList.h"

void GlassShard::OnEnable()
{
}

void GlassShard::OnDisable()
{

}

void GlassShard::Start()
{
}

void GlassShard::Update()
{
}

void GlassShard::OnDestroy()
{
}

void GlassShard::SetGlassParamPS()
{
	DirectX::XMFLOAT4 Tint[ToIndex(GlassGrade::Count)] = { {0.7f, 0.7f, 1.0f, 0.6f}, {0.7f, 1.0f, 0.7f, 0.6f}, {1.0f, 0.7f, 0.7f, 0.6f} };

	ShaderList::SetGlassParam(Tint[ToIndex(grade_)], 3.0f, 0.5f, 1.0f);
}


