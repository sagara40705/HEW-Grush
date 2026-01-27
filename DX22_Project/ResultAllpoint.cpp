#include "ResultAllpoint.h"
#include "GameState.h"
float point;
int arubaitoLevelpoint;
float oldpoint;//+ point;
float allpoint()
{
	float recentTime = GetRecentClearTimes();
	if (recentTime >= 60)
	{
		if (recentTime >= 120)
		{
			if (recentTime >= 180)
			{
				point = 250.f;
			}
			else
			{
				point = 300.f;
			}
		}
		else
		{
			point = 450.f;

		}
	}
	else
	{
		point = 600.f;
	}
	return point;
}


int arubaitoLevel()
{
	float point= allpoint();

	//¡‚Ü‚Å‚Ìƒ|ƒCƒ“ƒg‚Ì•Û‘¶
	oldpoint += point;
	if (oldpoint >= 600)
	{
		if (oldpoint >= 1200)
		{

			if (oldpoint >= 1800)
			{
				arubaitoLevelpoint = 3;
			}
			else
			{
				arubaitoLevelpoint = 2;
			}
		}
		else
		{
			arubaitoLevelpoint = 1;

		}
	}
	else
	{
		arubaitoLevelpoint = 1;

	}
	//	return arubaitoLevelpoint;
	return arubaitoLevelpoint;
}