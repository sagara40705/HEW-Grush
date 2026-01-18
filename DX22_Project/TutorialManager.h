#pragma once
#include "Component.h"
#include <cstdint>

enum class TutorialTask : std::uint8_t
{
	Move_01,
	ToGrade_2,
	Move_02,
	ToGrade_3,
	End
};
constexpr size_t ToIndex(TutorialTask task) noexcept { return static_cast<size_t>(task); }
constexpr size_t TASK_COUNT = ToIndex(TutorialTask::End);


class TutorialManager : public MonoBehaviour
{
private:
	bool endTasks[TASK_COUNT]{};
	UiSpriteRenderer* tasksUI[TASK_COUNT]{};
	TutorialTask nowTask = TutorialTask::Move_01;
public:
	void Awake() override;
	void Start() override;
	void OnDestroy() override;
	void Update()  override;
private:
	void CreateTasksUI();
private:
	void StepNextTask();
	bool CheckNowTask();
	bool CheckTutorialEnd();
};