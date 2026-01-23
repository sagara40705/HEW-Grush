#pragma once
#include <cstdint>
#include "Component.h"
#include "ResultGaugeUI.h"
enum class Arubaito : std::uint8_t
{
	One,
	Ten,
	Count
};
constexpr size_t ToIndex(Arubaito d) noexcept { return static_cast<size_t>(d); }

class ResultArubauto : public MonoBehaviour
{
private:
	static constexpr int MAX_SECONDS = 99 * 60 + 59;

private:
	ResultGaugeUI* arubaitoLevelUI;
	UiSpriteRenderer* uiMinute_[ToIndex(Arubaito::Count)]{};
	UiSpriteRenderer* uiColon_ = nullptr;
	UiSpriteRenderer* uiSecond_[ToIndex(Arubaito::Count)]{};
public:
	//<<<<<<< HEAD
//	float timer_ = 0.f;
	//=======
		//float timer_ = 0.f;		//©’Ç‰Á
	//>>>>>>> e348d9a321fd84f4a3ac8ea23cc62e58cc620fd9
	void Start() override;
	void Update() override;
	void OnDestroy() override;

};

