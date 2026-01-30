#pragma once
#include <memory>
#include <array>
#include <cstdint>
#include "Model.h"

enum class ModelTag : uint8_t
{
	/*0-7*/
	Swing,
	Akican,
	Desktop,
	Gomibukuro,
	Isu,
	Kuukiseizyouki,
	Ningixyou,
	SagaraDoll,

	/*8-11*/
	Benti,
	Aircon,
	Jihanki,
	Reizouko,

	/*12-14*/
	Ufo,
	Eiseii,
	Star,

	Player, // 15

	/*16-18*/
	Hammer_1,
	Hammer_2,
	Hammer_3,

	Fence, //19
	Warp,

	Count
};
constexpr size_t ToIndex(ModelTag tag) noexcept
{
	return static_cast<size_t>(tag);
}

struct ModelLoadData
{
	const char* pass = "";
	float size = 0.0f;
	Model::Flip flip = Model::Flip::None;
};

class ModelManager final
{
private:
	static std::array<std::unique_ptr<Model>, static_cast<size_t>(ModelTag::Count)> s_models;
	static std::array<float, static_cast<size_t>(ModelTag::Count)> modelBounds;
	static const ModelLoadData kModelData[static_cast<size_t>(ModelTag::Count)];
public:
	static bool Load(ModelTag tag);
	static void Unload(ModelTag tag);

	static Model* Get(ModelTag tag);
	static float GetBounds(ModelTag tag);

	static Model::AnimeNo AddAnimation(ModelTag tag, const char* file);

	static void Clear();
};