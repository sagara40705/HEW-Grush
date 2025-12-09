// SceneTitle.cpp
#include "SceneSample.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "AllComponent.h"
#include "TextureManager.h"
#include "Defines.h"
#include "Prefab.h"
#include "PrefabSet.h"
//Model* g_Model;

void SceneSample::Load()
{
	ModelManager::Load(ModelTag::Player);
	ModelManager::Load(ModelTag::Swing);
	ModelManager::Load(ModelTag::Isu);
	ModelManager::Load(ModelTag::Zihanki);
	ModelManager::Load(ModelTag::Eiseii);

	RegisterAllPrefabs();

	World& w = GetWorld();

	GameObject& mainCamera = w.CreateGameObject(Camera::kInitPos, Camera::kInitRotate, Vector3::one());
	mainCamera.SetTag(Tag::MainCamera);
	mainCamera.AddComponent<Camera>();

	GameObject& player1 = w.CreateGameObject(Vector3(-2.5f, 1.f, 0.f), Vector3(0.f, 0.f, 0.f));
	player1.AddComponent<MeshFilter>(ModelTag::Isu);
	player1.AddComponent<MeshRenderer>();

	Prefab::Instantiate(w, PrefabId::Swing, Vector3(0.f, 0.f, 0.f), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));

	GameObject& player3 = w.CreateGameObject(Vector3(2.5f, 1.f, 0.f), Vector3(0.f, 0.f, 0.f));
	player3.AddComponent<MeshFilter>(ModelTag::Eiseii);
	player3.AddComponent<MeshRenderer>();

}

void SceneSample::Unload()
{

}

void SceneSample::Update()
{
	//g_Model->Step(1.0f / 60.0f);
}

void SceneSample::Draw()
{
	//// 頂点シェーダーに渡す変換行列の変数を宣言 
	//DirectX::XMFLOAT4X4 fWVP[3];    // World,View,Projectionの略  
	//DirectX::XMMATRIX world, view, proj; // 各変換行列の格納先 


	//// 作成した行列を各変数へ格納 
	////ワールド座標用

	//world = DirectX::XMMatrixIdentity();

	////ビュー座標用

	//view = DirectX::XMLoadFloat4x4(&Camera::Main()->GetView());

	////プロジェクション座標用
	//proj = DirectX::XMLoadFloat4x4(&Camera::Main()->GetProjection());

	//// 計算用のデータから読み取り用のデータに変換 
	//DirectX::XMStoreFloat4x4(&fWVP[0], DirectX::XMMatrixTranspose(world));
	//DirectX::XMStoreFloat4x4(&fWVP[1], DirectX::XMMatrixTranspose(view));
	//DirectX::XMStoreFloat4x4(&fWVP[2], DirectX::XMMatrixTranspose(proj));

	//ShaderList::SetWVP(fWVP);

	////ボーン行列をシェーダーに設定
	////200はModelクラスの最大ボーン数に合わせる
	//for (unsigned int i = 0; i < g_Model->GetMeshNum(); ++i)
	//{
	//	const Model::Mesh* mesh = g_Model->GetMesh(i);
	//	if (!mesh) continue;

	//	// ボーン行列をシェーダーに設定
	//	DirectX::XMFLOAT4X4 bones[200]; // Model::MAX_BONE を使ってもOK
	//	for (int boneIdx = 0; boneIdx < mesh->bones.size(); ++boneIdx)
	//	{
	//		const Model::Bone& bone = mesh->bones[boneIdx];
	//		DirectX::XMStoreFloat4x4(
	//			&bones[boneIdx],
	//			DirectX::XMMatrixTranspose(
	//				bone.invOffset * g_Model->GetBoneMatrix(bone.nodeIndex)
	//			)
	//		);
	//	}

	//	ShaderList::SetBones(bones);

	//	// メッシュ描画（必要ならここで）
	//	//g_pModel->Draw(i);
	//}

	//// モデルに使用する頂点シェーダー、ピクセルシェーダーを設定 
	//g_Model->SetVertexShader(ShaderList::GetVS(ShaderList::VS_ANIME));
	//g_Model->SetPixelShader(ShaderList::GetPS(ShaderList::PS_LAMBERT));


	//// マテリアル別にメッシュを表示 
	//for (unsigned int i = 0; i < g_Model->GetMeshNum(); ++i) {
	//	// モデルのメッシュを取得 
	//	const Model::Mesh* mesh = g_Model->GetMesh(i);
	//	// メッシュに割り当てられているマテリアルを取得 
	//	Model::Material material = *g_Model->GetMaterial(mesh->materialID);
	//	// シェーダーへマテリアルを設定 
	//	ShaderList::SetMaterial(material);
	//	// モデルの描画 
	//	g_Model->Draw(i);
	//}
}

