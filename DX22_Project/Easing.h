#pragma once
#include <cmath>
#include <algorithm>

namespace Easing
{
	//値を 0.0 ～ 1.0 の範囲に固定する
	static float Saturate(float t)       { return std::clamp(t, 0.0f, 1.0f); }

	// ==== イージング関数群 ====
	// 一定の速度で動く
	static float EaseLinear(float t)     { return Saturate(t); }                                                                         // そのまま
	static float EaseInQuad(float t)     { t = Saturate(t); return t * t; }	                                                             // ゆっくり動き出し、だんだん加速する
	static float EaseOutQuad(float t)    { t = Saturate(t); return t * (2 - t); }                                                        // 勢いよく動き出し、ゆっくり止まる
	static float EaseInOutQuad(float t)  { t = Saturate(t); return t < 0.5f ? 2 * t * t : -1 + (4 - 2 * t) * t; }                        // ゆっくり動き出し、ゆっくり止まる
	static float EaseInCubic(float t)    { t = Saturate(t); return t * t * t; }                                                          // 最初はかなり遅く、後半で急加速する
	static float EaseOutCubic(float t)   { t = Saturate(t) - 1.0f; return t * t * t + 1; }                                               // 鋭く動き出し、後半にかけて滑らかに止まる
	static float EaseInOutCubic(float t) { t = Saturate(t); return t < 0.5f ? 4 * t * t * t : (t - 1) * (2 * t - 2) * (2 * t - 2) + 1; } // 最初と最後が非常にゆっくりで、中間が速い
	static float EaseOutBack(float t)    {                                                                                               // 目的地の少し先まで行き過ぎてから、戻るように止まる
		t = Saturate(t);
		float c1 = 1.70158f;
		float c3 = c1 + 1.0f;
		return 1.0f + c3 * powf(t - 1, 3) + c1 * powf(t - 1, 2);
	}
	static float EaseOutBounce(float t) {                                                                                                // 目的地でボールが跳ねるように止まる
		t = Saturate(t);
		if (t < 1 / 2.75f) return 7.5625f * t * t;
		else if (t < 2 / 2.75f) return 7.5625f * (t -= 1.5f / 2.75f) * t + 0.75f;
		else if (t < 2.5 / 2.75f) return 7.5625f * (t -= 2.25f / 2.75f) * t + 0.9375f;
		else return 7.5625f * (t -= 2.625f / 2.75f) * t + 0.984375f;
	}
}