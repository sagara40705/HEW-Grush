#pragma once
#include "Component.h"

class TitleManager : public MonoBehaviour
{
private:
	GameObject* player_ = nullptr;
	SpriteAnimation* playerAnimation_ = nullptr;
public:
	TitleManager(GameObject* player);
	void Start() override;
	void Update() override;
	void OnDestroy() override;
private:
	bool inputNext = false;
	float timer = 0.0f;
	bool canLoadNext = false;
private:
	bool InputNext();
	void AnimationTimer();
	void GoNext();
};