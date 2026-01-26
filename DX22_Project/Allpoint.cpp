#include "Allpoint.h"
#include "GameState.h"
float point;
int arubaitoLevelpoint;
float allpoint()
{
	float recentTime = GetRecentClearTimes();
	if (recentTime >= 1)
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
	float point = allpoint();
	if (point >= 600)
	{
		if (point >= 1200)
		{

			if (point >= 1800)
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

int GetarubaitoLevel()
{
	return arubaitoLevelpoint;
}
