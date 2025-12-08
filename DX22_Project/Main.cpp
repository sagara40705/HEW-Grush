#include "Main.h"
#include <memory>
#include "DirectX.h"
#include "PrimitiveMeshManager.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "InputKeyboard.h"
#include "InputGamepad.h"
#include "SceneGame.h"
#include "Defines.h"
#include "ShaderList.h"
#include "LineManager.h"
#include "Game.h"
#pragma commment(lib,Effekseer.lib)
#pragma commment(lib,EffekseerRendererDX11.lib)


HRESULT Init(HWND hWnd, UINT width, UINT height)
{
	HRESULT hr;
	// DirectX初期化
	hr = InitDirectX(hWnd, width, height, false);
	if (FAILED(hr)) { return hr; }

	// 他機能初期化
	PrimitiveMeshManager::Init();
	Sprite::Init();
	InputKeyboard::Init();
	InputGamepad::Init();
	ShaderList::Init();
	LineManager::Init();
	// シーン
	Game::Init();

	return hr;
}

void Uninit()
{
	Game::Uninit();

	LineManager::Uninit();
	ShaderList::Uninit();
	InputGamepad::Uninit();
	InputKeyboard::Uninit();
	ModelManager::Clear();
	TextureManager::Clear();
	Sprite::Uninit();
	PrimitiveMeshManager::Uninit();
	UninitDirectX();
}

void Update()
{
	InputKeyboard::Update();
	InputGamepad::Update();
	Game::Update();
}

void Draw()
{
	BeginDrawDirectX();
	Game::Draw();
	EndDrawDirectX();
}

// EOF