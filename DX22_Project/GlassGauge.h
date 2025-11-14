#pragma once
#include "Component.h"

class HummerScript; class CanvasUI;

class GlassGauge : public MonoBehaviour
{
private:
	HummerScript* hs = nullptr;
	CanvasUI* gauge = nullptr;
public:
	GlassGauge(HummerScript* _hs): hs(_hs){}
	void Start() override;
	void OnDestroy() override;
	void Update() override;

};