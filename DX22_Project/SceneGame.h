#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#include "Scene.h"
#include "SkyDomeMesh.h"
#include "SkyDomeShader.h"
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
private:
	void LoadModels();
	void LoadTextures();
	void LoadSound() {}

	void CreateManagers();
	void CreateCameraAndPlayer();
	void CreateStage();
	void CreateStage1();
	void CreateStage2();
	void CreateStage3();
	void CreateUi();
	void CreateGaugeUI();
	void CreateTimerUI();
	void CreateScoreUI();
	void CreateActionUI();

	//ギミック	
	void CreateGlassObjectSpawner();
	void CreateWarp();

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
};

#endif // __SCENE_GAME_H__