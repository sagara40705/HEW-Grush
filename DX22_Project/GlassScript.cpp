#include "GlassScript.h"
#include "ScoreManager.h"


void GlassScript::Start()
{

}

//void GlassScript::OnWillDestroy()
//{
//	
//}

void GlassScript::OnDestroy()
{
	ScoreManager::Instance().AddDestroyedKg(kWeightKg[ToInt(grade)]);
}
