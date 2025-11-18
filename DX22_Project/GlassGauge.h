#pragma once
#include "Component.h"

class HummerScript; class CanvasUI;

class GlassGauge : public MonoBehaviour
{
private:
	enum Gauge
	{
		Base,
		UpEffect,
		Count
	};
private:
	HummerScript* hs = nullptr;
	CanvasUI* gUi[Gauge::Count]{};
private:
	//Effect再生用
	int   efFrame = 0;        // 現在のフレーム番号
	int efCounter = 0;        // カウンタ
	bool  efPlaying = false;  // 再生中フラグ

	float prevGauge = 0.f;    // 1フレーム前のゲージ量
public:
	GlassGauge(CanvasUI* base, CanvasUI* effect);
	void Start() override;
	void OnDestroy() override;
	void Update() override;

	void GaugeEffect();

};