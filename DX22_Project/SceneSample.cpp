// SceneTitle.cpp
#include "SceneSample.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "AllComponent.h"
#include "TextureManager.h"
#include "Defines.h"

//Model* g_Model;

void SceneSample::Load()
{
	ModelManager::Load(ModelTag::Player);

	World& w = GetWorld();

	GameObject& mainCamera = w.CreateGameObject(Camera::kInitPos, Camera::kInitRotate, Vector3::one());
	mainCamera.SetTag(Tag::MainCamera);
	mainCamera.AddComponent<Camera>();

	GameObject& player = w.CreateGameObject(Vector3(0.f, 2.f, 0.f), Vector3(0.f, 0.f, 0.f));
	player.AddComponent<MeshFilter>(ModelTag::Player);
	player.AddComponent<MeshRenderer>();
	player.AddComponent<Animator>();

	//g_Model = new Model();
	//const char* fileName = "Assets/Model/playerchara_20251202/space_suit_12.fbx";
	//if (!g_Model->Load(fileName, 0.005))
	//{ // 倍率と反転は省略可 
	//	MessageBox(NULL, "読み込んだファイル名", "Error", MB_OK);
	//}

	//Model::AnimeNo jumpAnim = g_Model->AddAnimation("Assets/Model/animation_taiki1.fbx");

	//g_Model->PlayAnime(jumpAnim, true, 1.0f);
}

void SceneSample::Unload()
{
	//if (g_Model)
	//{
	//	delete g_Model;
	//	g_Model = nullptr;
	//}
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

