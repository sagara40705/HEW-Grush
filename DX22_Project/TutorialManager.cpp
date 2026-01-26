#include "TutorialManager.h"
#include "GameObject.h"
#include "UiSpriteRenderer.h"
#include "Sprite3DRenderer.h"
#include "GlassGaugeManager.h"

#include "InputKeyboard.h"
#include "DebugLog.h"

void TutorialManager::Awake()
{
	CreateTasksUI();
	CreateLocations();
}

void TutorialManager::Start()
{
	targetLocation[ToIndex(TutorialLocation::Move_01)]->active = true;

	GameObject* playerObj = gameObject->FindWithTag(Tag::Player);
	playerTf_ = playerObj->transform;
	assert(playerTf_ && "TutorialManagerでPlayerのTransformの参照がnullptrです");

	GameObject* gaugeMgrObj = gameObject->FindWithTag(Tag::GaugeManager);
	gauge_ = gaugeMgrObj->GetComponent<GlassGaugeManager>();
	assert(gauge_ && "TutorialManagerでGlasGaugeManagerの参照がnullptrです");
	gauge_->isTutorial_ = true;
}

void TutorialManager::OnDestroy()
{
	for (int i = 0; i < TASK_COUNT; ++i)
	{
		tasksUI[i] = nullptr;
	}
}

void TutorialManager::Update()
{
	CheckNowTask();
}

void TutorialManager::CreateTasksUI()
{
	//constexpr Vector2 POS[TASK_COUNT] = 
	//{
	//	{ 0.f, 50.f }, { 0.f, 100.f }, { 0.f, 150.f }, { 0.f, 200.f },{ 0.f, 250.f }
	//};
	//constexpr Vector2 SIZE = { 300.f, 40.f };

	//constexpr Vector2 UV_POS_OFF[TASK_COUNT] =
	//{
	//	{ 0.0f, 0.0f }, { 0.0f, 0.2f }, { 0.0f, 0.4f }, { 0.0f, 0.6f },{ 0.0f, 0.8f }
	//};

	//constexpr Vector2 UV_SIZE = { 0.5f, 0.2f };

	//for (int i = 0; i < TASK_COUNT; ++i)
	//{
	//	GameObject* obj = &gameObject->CreateChild();
	//	tasksUI[i] = &(obj->AddComponent<UiSpriteRenderer>(TextureTag::TasksUI, POS[i], SIZE, UV_POS_OFF[i], UV_SIZE));
	//}
}

void TutorialManager::CreateLocations()
{
	Vector3 POS[LOCATION_COUNT] =
	{
		{   0.f, -0.9f,  -8.f },
		{ -10.f, -0.9f, -12.f },
		{ -14.f, -0.9f, -15.f }
	};

	Vector3 ROT = { 90.f, 0.f, 0.f };

	for (int i = 0; i < LOCATION_COUNT; ++i)
	{
		GameObject* go = &gameObject->CreateChild(POS[i], ROT);
		go->AddComponent<Sprite3DRenderer>(TextureTag::TutorialLocation, Vector2::zero(), Vector2{ 4.f, 4.f });
		go->SetActive(false);
		targetLocation[i] = go;
	}
}

void TutorialManager::CheckNowTask()
{
	switch (nowTask)
	{
	case TutorialTask::Move_01:
		CheckTask_1();
		break;
	case TutorialTask::ToGrade_2:
		CheckTask_2();
		break;
	case TutorialTask::Move_02:
		CheckTask_3();
		break;
	case TutorialTask::ToGrade_3:
		CheckTask_4();
		break;
	case TutorialTask::End:
		CheckTask_5();
		break;
	default:
		break;
	}

}

void TutorialManager::CheckTask_1()
{
	//プレイヤーの位置とターゲット位置の距離を測る
	const Vector3 playerPos = playerTf_->GetWorldPosition();
	constexpr Vector3 TARGET_POS = { 0.f, -0.9f, -8.f };
	constexpr float TARGET_RADIUS = 1.5f;
	const float distance = Vector3::Distance(playerPos, TARGET_POS);

	if (distance < TARGET_RADIUS)
	{
		CompleteTask(TutorialTask::Move_01);
		targetLocation[ToIndex(TutorialLocation::Move_01)]->SetActive(false);
		gauge_->warkingTask_ = true;
	}
#ifdef _DEBUG
	if (InputKeyboard::GetKeyDown('T'))
	{
		CompleteTask(TutorialTask::Move_01);
		targetLocation[ToIndex(TutorialLocation::Move_01)]->SetActive(false);
		gauge_->warkingTask_ = true;
	}
#endif // DEBUG	
}

void TutorialManager::CheckTask_2()
{
	if (gauge_->endTask_02_)
	{
		CompleteTask(TutorialTask::ToGrade_2);
		targetLocation[ToIndex(TutorialLocation::Move_02)]->SetActive(true);
		gauge_->warkingTask_ = false;
	}
#ifdef _DEBUG
	if (InputKeyboard::GetKeyDown('T'))
	{
		CompleteTask(TutorialTask::ToGrade_2);
		targetLocation[ToIndex(TutorialLocation::Move_02)]->SetActive(true);
		gauge_->warkingTask_ = false;
	}
#endif // DEBUG	
}

void TutorialManager::CheckTask_3()
{
	//プレイヤーの位置とターゲット位置の距離を測る
	const Vector3 playerPos = playerTf_->GetWorldPosition();
	constexpr Vector3 TARGET_POS = { -10.f, -0.9f, -12.f };
	constexpr float TARGET_RADIUS = 1.5f;
	const float distance = Vector3::Distance(playerPos, TARGET_POS);

	if (distance < TARGET_RADIUS)
	{
		CompleteTask(TutorialTask::Move_02);
		targetLocation[ToIndex(TutorialLocation::Move_02)]->SetActive(false);
		gauge_->warkingTask_ = true;
	}
#ifdef _DEBUG
	if (InputKeyboard::GetKeyDown('T'))
	{
		CompleteTask(TutorialTask::Move_02);
		targetLocation[ToIndex(TutorialLocation::Move_02)]->SetActive(false);
		gauge_->warkingTask_ = true;
	}
#endif // DEBUG	
}

void TutorialManager::CheckTask_4()
{
	if (gauge_->endTask_04_)
	{
		CompleteTask(TutorialTask::ToGrade_3);
		targetLocation[ToIndex(TutorialLocation::Move_03)]->SetActive(true);
		gauge_->warkingTask_ = false;
	}
#ifdef _DEBUG
	if (InputKeyboard::GetKeyDown('T'))
	{
		CompleteTask(TutorialTask::ToGrade_3);
		targetLocation[ToIndex(TutorialLocation::Move_03)]->SetActive(true);
		gauge_->warkingTask_ = false;
	}
#endif // DEBUG	
}

void TutorialManager::CheckTask_5()
{
}

void TutorialManager::CompleteTask(TutorialTask task)
{
	constexpr float COMPLETED_UV_POS_X = 0.5f;

	endTasks[ToIndex(task)] = true;
	tasksUI[ToIndex(task)]->SetUVPosX(COMPLETED_UV_POS_X);
	nowTask = static_cast<TutorialTask>(ToIndex(task) + 1);

	DebugLog::OutputValue("TutorialTaskが完了 id = &d", static_cast<int>(task));
}
