#include "PlayerAnimation.h"
#include "GameObject.h"
#include "PlayerState.h"
#include "Animator.h"
#include <cassert>

/*
* @brief 初期化処理
*/
void PlayerAnimation::Start()
{
	state_ = gameObject->GetComponent<PlayerState>();
	anime_ = gameObject->GetComponent<Animator>();
	AddAllAnimations();
	anime_->Play(animation_[ToIndex(Type::Move)], false, 1.f);
	//anime_->Stop();*/


	assert(state_ && anime_ && "PlayerAnimation	で プレイヤーの <PlayerState> か <Animator> が nullptr です");
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
	CheckMoveAnime();
}

/*
* @brief 移動アニメーションの制御
*/
void PlayerAnimation::CheckMoveAnime()
{
	const float t = anime_->GetCurrentAnimeTime();
	constexpr float kLoopStart = 0.5f;
	constexpr float kLoopEnd = 4.0f;
	if (t > kLoopEnd && state_->isMoving)
	{
		anime_->SetTimeSeconds(kLoopStart);
	}

	if (state_->moveStartedThisFrame) 
	{
		anime_->Play(animation_[ToIndex(Type::Move)], false, 1.f);
		anime_->SetTimeSeconds(0.3f);
	}
	if (state_->moveStoppedThisFrame)
	{
		anime_->SetTimeSeconds(4.8f);
	}
}

/*
* @brief 全アニメーションをmodelに追加
* @brief animation_にAnimeNoを格納
*/
void PlayerAnimation::AddAllAnimations()
{
	Model* model = anime_->GetModel();
	if (!model) return;

	animation_[ToIndex(Type::Move)] = model->AddAnimation("Assets/Model/animation_hashiru.fbx");

}

/*
* @brief プレイヤーのステートから次のアニメーションをSetする
*/
PlayerAnimation::Type PlayerAnimation::DecideNextTypeFromState() const
{
	return Type::None;
}

/*
* @brief 次のアニメーションへ遷移する
* @param[from] no 現在のアニメーション Type
* @param[from] no 次のアニメーション Type
*/
void PlayerAnimation::ChangeState(Type from, Type to)
{

}

/*
* @brief 現在アニメーションの更新処理
*/
void PlayerAnimation::UpdateCurrentState()
{
}


