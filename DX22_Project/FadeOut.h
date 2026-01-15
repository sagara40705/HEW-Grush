#pragma once
#include "Component.h"

class FadeOut : public Component
{
public:
	// Fade.h Ç‡ÇµÇ≠ÇÕ FadeOut.h ÇÃè„ïî
	enum class FadeType
	{
		In,
		Out
	};
	FadeOut(FadeType type);
	//void SetFade(FadeType type);
	void Awake() override;
	void Start() override;
	void OnDestroy() override;
private:
	SpriteAnimation* anime_ = nullptr;
	FadeType FadeJudgment;
};