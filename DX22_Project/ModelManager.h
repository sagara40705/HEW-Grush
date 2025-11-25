#pragma once
#include <memory>
#include <array>
#include "Model.h"
#include "Vector3.h"

enum class ModelTag: int
{
	Karubi,
	Swing,
	Benti,
	Ufo,
	ProtPlay,
	Player,
	Count
};

struct ModelLoadData
{
	const char* pass = "";
	float size = 0.0f;
	Model::Flip flip = Model::Flip::None;
	Vector3 colliderPos = Vector3::zero();
	Vector3 colliderSize = Vector3::one();
};

class ModelManager
{
private:
	static std::array<std::unique_ptr<Model>, static_cast<int>(ModelTag::Count)> s_models;
	static const ModelLoadData kModelData[static_cast<int>(ModelTag::Count)];
public:
	/// 成功時 true, 失敗時 false
	static bool Register(ModelTag tag, const ModelLoadData& data);
	static bool RegisterAuto(ModelTag tag);   // ファイルパスを自動参照

	/// 取得：tag に対応する Texture のポインタを返す（なければ nullptr）
	static Model* Get(ModelTag tag);

	/// 全削除：登録されているすべての Texture を解放
	static void Clear();
};