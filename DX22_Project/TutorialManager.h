#pragma once
#include "Component.h"
#include <cstdint>

enum class TutorialTask : std::uint8_t
{
	Move_01,
	ToGrade_2,
	Move_02,
	ToGrade_3,
	End,
	Count
};
constexpr size_t ToIndex(TutorialTask task) noexcept { return static_cast<size_t>(task); }
constexpr size_t TASK_COUNT = ToIndex(TutorialTask::Count);

enum class TutorialLocation : std::uint8_t
{
	Move_01,
	Move_02,
	Move_03,
	Count
};
constexpr size_t ToIndex(TutorialLocation location) noexcept { return static_cast<size_t>(location); }
constexpr size_t LOCATION_COUNT = ToIndex(TutorialLocation::Count);

class GlassGaugeManager;

class TutorialManager : public MonoBehaviour
{
private:
	bool endTasks[TASK_COUNT]{};
	UiSpriteRenderer* tasksUI[TASK_COUNT]{};
	TutorialTask nowTask = TutorialTask::Move_01;
	GameObject* targetLocation[LOCATION_COUNT];
	Transform* playerTf_ = nullptr;
	GlassGaugeManager* gauge_ = nullptr;

public:
	void Awake() override;
	void Start() override;
	void OnDestroy() override;
	void Update()  override;
private:
	void CreateTasksUI();
	void CreateLocations();
private:
	void CheckNowTask();
	void CheckTask_1();
	void CheckTask_2();
	void CheckTask_3();
	void CheckTask_4();
	void CheckTask_5();
private:
	void CompleteTask(TutorialTask task);
};