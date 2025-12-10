#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#include "Scene.h"
#include"SkyDomeMesh.h"
#include"SkyDomeShader.h"

class SceneGame : public Scene
{
public:
	SceneGame();
	~SceneGame();
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
	void CreateUi();
	void CreateGaugeUI();
	void CreateTimerUI();
	void CreateScoreUI();

private:
	void SkyDomeDorw();

	// ここに追加
	void InitSkyDome();

	// カメラ用の行列など（すでにあればそのまま）
	DirectX::XMFLOAT4X4 m_world;
	DirectX::XMFLOAT4X4 m_view;
	DirectX::XMFLOAT4X4 m_proj;
	ID3D11DepthStencilState* m_pDepthNoWrite = nullptr; // アウトライン用（書き込まない）x

	SkyDomeShader m_skyShader;  // 宇宙スカイドーム用シェーダー
	SkyDomeMesh   m_skyMesh;    // ★ スカイドーム専用メッシュ

	Texture* m_pSkyTex = nullptr; // ★ 追加：スカイドーム用テクスチャ

};

#endif // __SCENE_GAME_H__