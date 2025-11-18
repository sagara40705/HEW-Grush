#pragma once
#include <memory>
#include <array>
#include "Model.h"

enum class ModelTag: int
{
	Karubi,
	Swing,
	Benti,
	Ufo,
	Count
};

class ModelManager
{
private:
	static std::array<std::unique_ptr<Model>, static_cast<int>(ModelTag::Count)> s_models;
	static const char* kModelPath[static_cast<int>(ModelTag::Count)];
public:
	/// 成功時 true, 失敗時 false
	static bool Register(ModelTag tag, const char* filePath);
	static bool RegisterAuto(ModelTag tag);   // ファイルパスを自動参照

	/// 取得：tag に対応する Texture のポインタを返す（なければ nullptr）
	static Model* Get(ModelTag tag);

	/// 全削除：登録されているすべての Texture を解放
	static void Clear();
};