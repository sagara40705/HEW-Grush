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
	// 前回の時間を保存する変数
	static std::chrono::high_resolution_clock::time_point lastTime;
};