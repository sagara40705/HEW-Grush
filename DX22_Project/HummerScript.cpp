#include "HummerScript.h"
#include "Transform.h"
#include "Collider.h" 
#include "GameObject.h"
#include "Input.h"
#include <algorithm>
#include <cmath>
#include "DebugLog.h"

void HummerScript::Start()
{
	t = Parent->TryGetComponent<Transform>();
	assert(t && "HummerScript requires Transform");
}

//void HummerScript::Update()
//{
//	//if (!t) return;
//
//	//rotate += 0.1;
//	//Vector3 pos = owner->GetPosition() + Vector3(owner->GetScale().x * sinf(rotate), 0, owner->GetScale().x * cosf(rotate));
//
//	//t->SetPosition(pos);
//	////t->SetRotation(Vector3(0, rotate * 31, 0));
//}

void HummerScript::OnDestroy()
{
	t = nullptr;
	owner = nullptr;
}

void HummerScript::OnTriggerEnter(const TriggerInfo& c)
{
	if (c.other)
	{
		if (!c.other->CompareTag(Tag::Glass)) return;
		Debug::LogValue(" HummerScript::Destroy id", c.other->id);
		c.other->Destroy();
	}
}