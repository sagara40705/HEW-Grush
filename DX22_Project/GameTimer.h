#pragma once

#include<Windows.h>

class GameTimer
{
private:
	LARGE_INTEGER m_frequency; // タイマー周波数
	LARGE_INTEGER m_prevTime;  // 前のフレームの時間
	float m_deltaTime;         // 経過時間

public:
	GameTimer() : m_deltaTime(0.0f)
	{
		// コンストラクタで周波数と現在時間を初期化
		QueryPerformanceFrequency(&m_frequency);
		QueryPerformanceFrequency(&m_prevTime);
	}

	// 毎フレーム呼び出して時間を更新する
	void Updata()
	{
		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);

		// 経過カウント数を計算
		long long elapsedTicks = currentTime.QuadPart - m_prevTime.QuadPart;
		// 経過時間を秒に変換（カウント数/ 周波数)
		// doubleで計算してからfloatにキャストして精度を保つ
		m_deltaTime = static_cast<float>((double)elapsedTicks / (double)m_frequency.QuadPart);

		// 現在時間を保持
		m_prevTime = currentTime;
	}

	// Unityのtime.deltatimeに相当
	float GetDeltaTime() const
	{
		return m_deltaTime;
	}
};
