#pragma once
#include <memory>
#include <array>
#include <cstdint>
#include "Model.h"

enum class ModelTag : uint8_t
{
	Karubi,

	// grade1 0bj
	Swing,
	Akican,
	Desktop,
	Gomibukuro,
	Isu,
	Kuukseizixyoukii,
	Ningixyou,
	SagaraDoll,
	Testp,


	// grade2 obj
	Benti,
	Aircon,
	Jihanki,
	Reizouko,
	AirPurifier,
	fridge,

	// grade3 obj
	Ufo,
	Eiseii,
	Star,

	// player関連
	ProtPlay,
	Player,
	Hammer_grade1,
	Hammer_grade2,
	Hammer_grade3,
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