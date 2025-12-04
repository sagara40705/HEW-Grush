#pragma once
#include <chrono>
#include <string>

class Timer
{
public:
	static void Init();				//初期化
	static void Uninit();			//終了
	static void Update();			//更新(DeltaTime計算)
	static float GetDeltaTime();	//DeltaTimeを所得
private:
	static float deltaTime;
};