#pragma once
#include "Component.h"
class ResultManeger : public MonoBehaviour
{
private:
	
public:
	ResultManeger();
	//void Start() override;
	void Update() override;
	//void OnDestroy() override;
private:
	bool inputNext = false;
	//float timer = 0.0f;
	bool canLoadNext = false;
private:
	bool InputNext();
	void TitleTimer();
	void GoNext();
};

