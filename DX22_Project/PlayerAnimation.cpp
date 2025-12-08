#include "PlayerAnimation.h"
#include "GameObject.h"
#include "PlayerState.h"
#include "Animator.h"

/*
* @brief 初期化処理
*/
void PlayerAnimation::Start()
{
	AddAllAnimations();
}

/*
* @brief 終了処理
*/
void PlayerAnimation::OnDestroy()
{
}

/*
* @brief 更新処理
*/
void PlayerAnimation::Update()
{
}

/*
* @brief 全アニメーションをmodelに追加
* @brief animation_にAnimeNoを格納
*/
void PlayerAnimation::AddAllAnimations()
{
	//アニメーションファイルパスのリスト
	const char* FilePass[ToIndex(Type::Count)] = 
	{
		"",
		"",
		"",
		"",
	};

	Model* model = anime_->GetModel();
	if (!model) return;

	for (size_t type = 0; type < ToIndex(Type::Count); ++type) 
	{
		if (FilePass[type] == "") continue;

		animation_[type] = model->AddAnimation(FilePass[type]);
	}

}
