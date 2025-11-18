#include "GlassScript.h"
#include "ScoreManager.h"


void GlassScript::Start()
{

}

void GlassScript::OnWillDestroy()
{
	ScoreManager::Instance().AddDestroyedKg(kWeightKg[ToInt(grade)]);
}

void GlassScript::OnDestroy()
{
	
}
