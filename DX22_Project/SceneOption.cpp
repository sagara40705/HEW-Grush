#include "SceneOption.h"
#include "GameObject.h"
#include "AllComponent.h"
#include "SceneManager.h"
#include "DebugLog.h"
#include "InputKeyboard.h"

void SceneOption::Load()
{
	TexturesLoad();
	CheckCanvas();
	CreateBG();
	CreateOption();
}

void SceneOption::Unload()
{
	
}

void SceneOption::TexturesLoad()
{
	TextureManager::Load(TextureTag::Option_BG);
	TextureManager::Load(TextureTag::option_Back);
	TextureManager::Load(TextureTag::option_Font1);
	TextureManager::Load(TextureTag::option_Font1_Back); 
	TextureManager::Load(TextureTag::option_Font1_OK);
	TextureManager::Load(TextureTag::option_Font2); 
	TextureManager::Load(TextureTag::option_Font2_Back); 
	TextureManager::Load(TextureTag::option_Font2_OK); 
	TextureManager::Load(TextureTag::option_Font3);
	TextureManager::Load(TextureTag::option_Font3_Back); 
	TextureManager::Load(TextureTag::option_Font3_OK); 
	TextureManager::Load(TextureTag::option_Font4); 
	TextureManager::Load(TextureTag::option_Font4_OK); 
	TextureManager::Load(TextureTag::option_Font5); 
	TextureManager::Load(TextureTag::option_Font5_OK); 
	TextureManager::Load(TextureTag::option_Font1_Break); 
	TextureManager::Load(TextureTag::option_Font2_Break); 
	TextureManager::Load(TextureTag::option_Font3_Break); 
	TextureManager::Load(TextureTag::option_Font4_screen); 
	TextureManager::Load(TextureTag::option_translucent); ;
}

void SceneOption::CheckCanvas()
{
	if (Canvas::GetMain())
	{
		DebugLog::Output("Canvas already exists.");
		return;
	}
	else
	{
		World& world = GetWorld();

		GameObject& canvasObj = world.CreateGameObject();
		canvasObj.AddComponent<Canvas>();
		DebugLog::Output("Canvas created.");
		return;
	}
}

void SceneOption::CreateBG()
{
	World& world = GetWorld();

	GameObject& bgObj = world.CreateGameObject();
	bgObj.AddComponent<UiSpriteRenderer>(TextureTag::Option_BG, Vector2::zero(), Vector2(1280.f, 720.f));
}

void SceneOption::CreateOption()
{
	World& w = GetWorld();

	// �I�v�V�����S�̂�Root�i�ŏ��͔�\���j
	GameObject& optionRoot = w.CreateGameObject();
	optionRoot.SetActive(false);

	// �R���g���[���͏��Active�ȃI�u�W�F�N�g�ɕt����
	GameObject& optionCtrlObj = w.CreateGameObject();
	auto& ctrl = optionCtrlObj.AddComponent<OptionUIController>();
	ctrl.SetOptionRoot(&optionRoot);

	// �w�i�i�K�v�Ȃ�j
	{
		GameObject& bg = optionRoot.CreateChild();
		bg.AddComponent<UiSpriteRenderer>
			(TextureTag::option_translucent, Vector2(0.f, 0.f),
				Vector2::WindowSize());

		GameObject& bg1 = optionRoot.CreateChild();
		bg1.AddComponent<UiSpriteRenderer>
			(TextureTag::option_Back, Vector2(0.f, 0.f),
				Vector2::WindowSize());
	}

	// 5���ځFNormal �� Selected ���y�A�ō��
	// Selected(Back) �͍ŏ�OFF�ɂ��āA�I�𒆂���ON�ɂȂ�悤�ɂ���

	// ------------------------
// 1
// ------------------------

// �ʏ�g�ifs = �ʏ�g�Ƃ��Ďg���j
	GameObject& fs1 = optionRoot.CreateChild();
	fs1.AddComponent<UiSpriteRenderer>(TextureTag::option_Font1_Back,
		Vector2(0.f, 0.f), Vector2::WindowSize());
	fs1.SetActive(false);

	// ����g�i1�`3�̂݁j
	GameObject& fp1 = optionRoot.CreateChild();
	fp1.AddComponent<UiSpriteRenderer>(TextureTag::option_Font1_Break,
		Vector2(0.f, 0.f), Vector2::WindowSize());
	fp1.SetActive(false);

	// �ʏ핶��
	GameObject& tn1 = optionRoot.CreateChild();
	tn1.AddComponent<UiSpriteRenderer>(TextureTag::option_Font1,
		Vector2(0.f, 0.f), Vector2::WindowSize());
	tn1.SetActive(false);

	// �I�𒆕����i������� option_Font1 �ɂ���OK�j
	GameObject& ts1 = optionRoot.CreateChild();
	ts1.AddComponent<UiSpriteRenderer>(TextureTag::option_Font1_OK,
		Vector2(0.f, 0.f), Vector2::WindowSize());
	ts1.SetActive(false);

	ctrl.AddItem(&tn1, &ts1, &fs1, &fp1, false);


	// ------------------------
	// 2
	// ------------------------

	GameObject& fs2 = optionRoot.CreateChild();
	fs2.AddComponent<UiSpriteRenderer>(TextureTag::option_Font2_Back,
		Vector2(0.f, 0.f), Vector2::WindowSize());
	fs2.SetActive(false);

	// ����g�i1�`3�̂݁j
	GameObject& fp2 = optionRoot.CreateChild();
	fp2.AddComponent<UiSpriteRenderer>(TextureTag::option_Font2_Break,
		Vector2(0.f, 0.f), Vector2::WindowSize());
	fp2.SetActive(false);

	// �ʏ핶��
	GameObject& tn2 = optionRoot.CreateChild();
	tn2.AddComponent<UiSpriteRenderer>(TextureTag::option_Font2,
		Vector2(0.f, 0.f), Vector2::WindowSize());
	tn2.SetActive(false);

	// �I�𒆕����i������� option_Font2 �ɂ���OK�j
	GameObject& ts2 = optionRoot.CreateChild();
	ts2.AddComponent<UiSpriteRenderer>(TextureTag::option_Font2_OK,
		Vector2(0.f, 0.f), Vector2::WindowSize());
	ts2.SetActive(false);

	ctrl.AddItem(&tn2, &ts2, &fs2, &fp2, false);


	// ------------------------
	// 3
	// ------------------------

	GameObject& fs3 = optionRoot.CreateChild();
	fs3.AddComponent<UiSpriteRenderer>(TextureTag::option_Font3_Back,
		Vector2(0.f, 0.f), Vector2::WindowSize());
	fs3.SetActive(false);

	// ����g�i1�`3�̂݁j
	GameObject& fp3 = optionRoot.CreateChild();
	fp3.AddComponent<UiSpriteRenderer>(TextureTag::option_Font3_Break,
		Vector2(0.f, 0.f), Vector2::WindowSize());
	fp3.SetActive(false);

	// �ʏ핶��
	GameObject& tn3 = optionRoot.CreateChild();
	tn3.AddComponent<UiSpriteRenderer>(TextureTag::option_Font3,
		Vector2(0.f, 0.f), Vector2::WindowSize());
	tn3.SetActive(false);

	// �I�𒆕����i������� option_Font3 �ɂ���OK�j
	GameObject& ts3 = optionRoot.CreateChild();
	ts3.AddComponent<UiSpriteRenderer>(TextureTag::option_Font3_OK,
		Vector2(0.f, 0.f), Vector2::WindowSize());
	ts3.SetActive(false);

	ctrl.AddItem(&tn3, &ts3, &fs3, &fp3, false);


	// ------------------------
	// 4�iEnter�ŕω��Ȃ��j
	// ------------------------

	GameObject& fs4 = optionRoot.CreateChild();
	fs4.AddComponent<UiSpriteRenderer>(TextureTag::option_Font4,
		Vector2(0.f, 0.f), Vector2::WindowSize());
	fs4.SetActive(false);

	// ����g�i1�`3�̂݁j
	GameObject& fp4 = optionRoot.CreateChild();
	fp4.AddComponent<UiSpriteRenderer>(TextureTag::option_Font4_screen,
		Vector2(0.f, 0.f), Vector2::WindowSize());
	fp4.SetActive(false);

	// �ʏ핶��
	GameObject& tn4 = optionRoot.CreateChild();
	tn4.AddComponent<UiSpriteRenderer>(TextureTag::option_Font4,
		Vector2(0.f, 0.f), Vector2::WindowSize());
	tn4.SetActive(false);

	// �I�𒆕����i������� option_Font4 �ɂ���OK�j
	GameObject& ts4 = optionRoot.CreateChild();
	ts4.AddComponent<UiSpriteRenderer>(TextureTag::option_Font4_OK,
		Vector2(0.f, 0.f), Vector2::WindowSize());
	ts4.SetActive(false);

	// 4�� pressed �Ȃ�
	ctrl.AddItem(&tn4, &ts4, &fs4, &fp4, true);


	// ------------------------
	// 5�iEnter�ŕω��Ȃ��j
	// ------------------------

	GameObject& fs5 = optionRoot.CreateChild();
	fs5.AddComponent<UiSpriteRenderer>(TextureTag::option_Font5,
		Vector2(0.f, 0.f), Vector2::WindowSize());
	fs5.SetActive(false);

	// ����g�i1�`3�̂݁j

	// �ʏ핶��
	GameObject& tn5 = optionRoot.CreateChild();
	tn5.AddComponent<UiSpriteRenderer>(TextureTag::option_Font5,
		Vector2(0.f, 0.f), Vector2::WindowSize());
	tn5.SetActive(false);

	// �I�𒆕����i������� option_Font5 �ɂ���OK�j
	GameObject& ts5 = optionRoot.CreateChild();
	ts5.AddComponent<UiSpriteRenderer>(TextureTag::option_Font5_OK,
		Vector2(0.f, 0.f), Vector2::WindowSize());
	ts5.SetActive(false);

	// 5�� pressed �Ȃ�
	ctrl.AddItem(&tn5, &ts5, &fs5, nullptr, false);
}

void SceneOption::Update()
{
	if (InputKeyboard::GetKeyDown('O'))
	{
		SceneManager::PopScene();
	}
}

void SceneOption::Draw()
{
}