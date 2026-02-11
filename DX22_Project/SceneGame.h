#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#include "Scene.h"
#include "SkyDomeMesh.h"
#include "SkyDomeShader.h"
#include "UiSpriteRenderer.h"
#include "Time/Timer.h"
#include <cstdint>
#include <string>

//前方宣言
enum class PrefabId : std::uint8_t;

class SceneGame : public Scene
{
public:

	void Update() final;
	void Draw() final;
	void Load() final;
	void Unload() final;
	// 非同期ロード用
	void BuildLoadTasks(std::vector<LoadTask>& outTasks) override;
	void OnReturnFromOption() override;
private:
	void CreateManagers();
	void CreateCameraAndPlayer();
	void CreateStage();
	void CreateStage1();
	void CreateStage1Wall();
	void CreateStage2();
	void CreateStage2Wall();
	void CreateStage3();
	void CreateStage3Wall();
	void CreateUi();
	void CreateGaugeUI();
	void CreateTimerUI();
	void CreateScoreUI();
	void CreateActionUI();
	void CreateStartUI();
	void CreateTutorialWall();

	//ギミック	
	void CreateGlassObjectSpawner();
	void CreateWarpStage2();
	void CreateWarpStage3();

	void CreateStageFromCSV(const std::string& filename);
	PrefabId GetPrefabIdFromString(const std::string& rawname);

	void InitPlayerTex();
	void InitSkyDome();
	void SkyDomeDorw();
private:
	DirectX::XMFLOAT4X4 m_world;
	DirectX::XMFLOAT4X4 m_view;
	DirectX::XMFLOAT4X4 m_proj;
	ID3D11DepthStencilState* m_pDepthNoWrite = nullptr;

	SkyDomeShader m_skyShader; 
	SkyDomeMesh   m_skyMesh; 
	Texture* m_pSkyTex = nullptr;

	bool CheckGameSound;
	bool CheckStartSE;
private:
	//スタートUI用
	UiSpriteRenderer* uiStart_ = nullptr;
	float m_animationTime = 0.0f; // アニメーションの経過時間
	Timer waitTimer_;
	const float WAIT_TIME = 4.0f;
};

#endif // __SCENE_GAME_H__