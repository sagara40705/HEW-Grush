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
	anime_->Play(animation_[Type::Idle], true, 1.f);
	anime_->SetTimeSeconds(4.8f);

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
	//移動を先に判定
	CheckMoveAnime();
	CheckSpinAttackAnime();
	CheckDashAttackAnime();


}

/*
* @brief 移動アニメーションの制御
*/
void PlayerAnimation::CheckMoveAnime()
{
	constexpr float kLoopStart = 0.5f;
	constexpr float kLoopEnd = 4.0f;

	if (currentType_ == Type::Move)
	{ 
		const float t = anime_->GetCurrentAnimeTime();

		//ループ範囲外に出たらループ開始位置に戻す
		if (t > kLoopEnd && state_->isMoving)
		{
			anime_->SetTimeSeconds(kLoopStart);
		}

		//移動停止したら終了位置まで再生
		if (state_->moveStoppedThisFrame)
		{
			currentType_ = Type::Idle;
			anime_->Play(animation_[Type::Idle], true, 1.f);
		}

		
	}
	else
	{
		//移動開始したら移動アニメーションへ
		if (state_->moveStartedThisFrame)
		{
			currentType_ = Type::Move;
			anime_->Play(animation_[Type::Move], false, 1.f);
			anime_->SetTimeSeconds(0.3f);
		}
	}
}

void PlayerAnimation::CheckSpinAttackAnime()
{
	if (currentType_ == Type::SpinAttack)
	{
		const float t = anime_->GetCurrentAnimeTime();

		//アニメーション終了したら待機アニメーションへ
		if (t > 2.5f && t < 5.f)
		{
			currentType_ = Type::Idle;
			anime_->Play(animation_[Type::Idle], true, 1.f);

		}


	}
	else
	{
		//回転攻撃アクションを実行したら回転攻撃アニメーションへ
		if (state_->actionedThisFrame[ToIndex(ActionId::SpinAttack)])
		{
			currentType_ = Type::SpinAttack;
			anime_->Play(animation_[Type::SpinAttack], false, 1.0f);
			anime_->SetTimeSeconds(1.0f);
		}
	}
}


void PlayerAnimation::CheckDashAttackAnime()
{

	if (currentType_ == Type::DashAttack)
	{
		const float t = anime_->GetCurrentAnimeTime();

		//アニメーション終了したら待機アニメーションへ
		if (t > 2.5)
		{
			currentType_ = Type::Idle;
			anime_->Play(animation_[Type::Idle], true, 1.f);
		}
	}
	else
	{
		//ダッシュ攻撃アクションを実行したらダッシュ攻撃アニメーションへ
		if (state_->actionedThisFrame[ToIndex(ActionId::DashAttack)])
		{
			currentType_ = Type::DashAttack;
			anime_->Play(animation_[Type::DashAttack], false, 1.0f);
			anime_->SetTimeSeconds(1.0f);
		}
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

	//待機
	animation_[Type::Idle] = model->AddAnimation("Assets/Animation/animation_taiki1.fbx");
	//移動
	animation_[Type::Move] = model->AddAnimation("Assets/Animation/animation_hashiru_2.fbx");
	//回転攻撃
	animation_[Type::SpinAttack] = model->AddAnimation("Assets/Animation/animation_rotateattack.fbx");
	//突進攻撃
	animation_[Type::DashAttack] = model->AddAnimation("Assets/Animation/animation_dashattack.fbx");
	//のけぞり
	animation_[Type::LeanBack] = model->AddAnimation("Assets/Animation/animation_knockback.fbx");
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
void PlayerAnimation::ChangeState()
{

}

/*
* @brief 現在アニメーションの更新処理
*/
void PlayerAnimation::UpdateCurrentState()
{

}


