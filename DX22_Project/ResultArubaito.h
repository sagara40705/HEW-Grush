#pragma once
#include <cstdint>
#include "Component.h"
enum class Arubaito : std::uint8_t
{
	One,
	Ten,
	Count
};
constexpr size_t ToIndex(Arubaito d) noexcept { return static_cast<size_t>(d); }


class ResultArubaito : public MonoBehaviour
{
public:
     void  Awake();

private:
private:
	UiSpriteRenderer* uiMinute_[ToIndex(Arubaito::Count)]{};
	UiSpriteRenderer* uiColon_ = nullptr;
	UiSpriteRenderer* uiSecond_[ToIndex(Arubaito::Count)]{};

};



