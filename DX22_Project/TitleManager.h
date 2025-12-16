#include "Component.h"

class TitleManager : public MonoBehaviour
{
private:
	SpriteAnimation* fade_ = nullptr;
public:
	void Awake() override;
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