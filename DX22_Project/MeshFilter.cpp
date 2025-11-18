#include "MeshFilter.h"
#include <vector>
#include <DirectXMath.h>

void MeshFilter::Start()
{
	BuildPrimitive();
}

void MeshFilter::OnDestroy()
{
	mMeshBuffer.reset();
}

void MeshFilter::BuildPrimitive()
{
	switch (PrimitiveType)
	{
	case MeshFilter::Cube:		BuildCube();
		break;
	case MeshFilter::Sphere:	BuildSphere();
		break;
	case MeshFilter::Plane:		BuildPlane();
		break;
	case MeshFilter::Cylinder:	BuildCylinder();
		break;
	default:
		BuildCube();
		break;
	}
}

void MeshFilter::BuildCube()
{
	//--- 頂点の作成
	Vertex vtx[] = {
		// -Z面 
		{{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f}},
		{{ 0.5f, 0.5f, -0.5f}, {1.0f, 0.0f}},
		{{-0.5f,-0.5f, -0.5f}, {0.0f, 1.0f}},
		{{ 0.5f,-0.5f, -0.5f}, {1.0f, 1.0f}},
		// +Z面 
		{{-0.5f, 0.5f,  0.5f}, {0.0f, 0.0f}},
		{{ 0.5f, 0.5f,  0.5f}, {1.0f, 0.0f}},
		{{-0.5f,-0.5f,  0.5f}, {0.0f, 1.0f}},
		{{ 0.5f,-0.5f,  0.5f}, {1.0f, 1.0f}},
		//+x
		{{ 0.5f, 0.5f,-0.5f}, {0.0f, 0.0f}},
		{{ 0.5f, 0.5f,  0.5f}, {1.0f, 0.0f}},
		{{ 0.5f,-0.5f,-0.5f}, {0.0f, 1.0f}},
		{{ 0.5f,-0.5f, 0.5f}, {1.0f, 1.0f}},
		//-x
		{{ -0.5f, 0.5f,-0.5f}, {0.0f, 0.0f}},
		{{ -0.5f, 0.5f,  0.5f}, {1.0f, 0.0f}},
		{{ -0.5f,-0.5f,-0.5f}, {0.0f, 1.0f}},
		{{ -0.5f,-0.5f, 0.5f}, {1.0f, 1.0f}},
		//+y
		{{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f}},
		{{-0.5f, 0.5f,  0.5f}, {1.0f, 0.0f}},
		{{ 0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}},
		{{ 0.5f, 0.5f,  0.5f}, {1.0f, 1.0f}},
		//-y
		{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
		{{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
		{{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
		{{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}},
	};

	//--- インデックスの作成
	int idx[] = {
		0, 1, 2,  1, 3, 2, // -Z面 
		4, 6, 5,  5, 6, 7, // +Z面 

		8, 9, 10,  9, 11, 10, //+x
		13,12,14, 13, 14, 15, //-x

		16, 17, 18,  17, 19, 18, //+y
		21, 20, 22,  21, 22, 23, //-y
	};

	// バッファの作成
	MeshBuffer::Description desc = {};
	desc.pVtx = vtx;
	desc.vtxCount = 24;
	desc.vtxSize = 20;
	desc.pIdx = idx;
	desc.idxCount = 36;
	desc.idxSize = 4;
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	mMeshBuffer = std::make_shared<MeshBuffer>();
	mMeshBuffer->Create(desc);
}

void MeshFilter::BuildSphere()
{
	const int slice = 16; // 経度分割
	const int stack = 16; // 緯度分割
	const float radius = 0.5f;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	// 頂点生成
	for (int i = 0; i <= stack; ++i)
	{
		float phi = DirectX::XM_PI * i / stack;
		float y = std::cos(phi);
		float r = std::sin(phi);

		for (int j = 0; j <= slice; ++j)
		{
			float theta = DirectX::XM_2PI * j / slice;
			float x = r * std::cos(theta);
			float z = r * std::sin(theta);

			Vertex v{};
			v.pos[0] = x * radius;
			v.pos[1] = y * radius;
			v.pos[2] = z * radius;
			v.uv[0] = static_cast<float>(j) / slice;
			v.uv[1] = static_cast<float>(i) / stack;
			vertices.push_back(v);
		}
	}

	// インデックス生成
	for (int i = 0; i < stack; ++i)
	{
		for (int j = 0; j < slice; ++j)
		{
			int a = i * (slice + 1) + j;
			int b = a + slice + 1;

			indices.push_back(a);
			indices.push_back(b);
			indices.push_back(a + 1);

			indices.push_back(a + 1);
			indices.push_back(b);
			indices.push_back(b + 1);
		}
	}

	MeshBuffer::Description desc{};
	desc.pVtx = vertices.data();
	desc.vtxCount = static_cast<UINT>(vertices.size());
	desc.vtxSize = sizeof(Vertex);
	desc.pIdx = indices.data();
	desc.idxCount = static_cast<UINT>(indices.size());
	desc.idxSize = sizeof(unsigned int);
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	mMeshBuffer = std::make_shared<MeshBuffer>();
	mMeshBuffer->Create(desc);
}

void MeshFilter::BuildPlane()
{
	// y=0 の XZ 平面（原点中心、1x1）
	Vertex vtx[4] =
	{
		{{-0.5f, 0.0f, -0.5f}, {0.0f, 1.0f}}, // 左下
		{{-0.5f, 0.0f,  0.5f}, {0.0f, 0.0f}}, // 左上
		{{ 0.5f, 0.0f,  0.5f}, {1.0f, 0.0f}}, // 右上
		{{ 0.5f, 0.0f, -0.5f}, {1.0f, 1.0f}}, // 右下
	};
	// インデックス生成
	unsigned int idx[6] = { 0,1,2, 0,2,3 };

	MeshBuffer::Description desc{};
	desc.pVtx = vtx;
	desc.vtxCount = 4;
	desc.vtxSize = sizeof(Vertex);
	desc.pIdx = idx;
	desc.idxCount = 6;
	desc.idxSize = sizeof(unsigned int);
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	mMeshBuffer = std::make_shared<MeshBuffer>();
	mMeshBuffer->Create(desc);
}

void MeshFilter::BuildCylinder()
{
	using namespace DirectX;

	const int   slice = 24;       // 角度分割
	const float radius = 0.5f;     // 半径
	const float halfH = 0.5f;     // 高さ/2

	std::vector<Vertex>        vertices;
	std::vector<unsigned int>  indices;

	// ===== 側面 =====
	// 下段(y=-halfH) と 上段(y=+halfH) の2列、それぞれ (slice+1) 頂点でクローズ
	for (int yStep = 0; yStep <= 1; ++yStep)
	{
		const float y = (yStep == 0) ? -halfH : +halfH;
		const float v = static_cast<float>(yStep); // 0 or 1

		for (int i = 0; i <= slice; ++i)
		{
			const float t = static_cast<float>(i) / slice; // 0..1
			const float theta = XM_2PI * t;
			const float x = std::cos(theta) * radius;
			const float z = std::sin(theta) * radius;

			Vertex vert{};
			vert.pos[0] = x;  vert.pos[1] = y;  vert.pos[2] = z;
			vert.uv[0] = t;  vert.uv[1] = v;  // U=角度, V=高さ
			vertices.push_back(vert);
		}
	}
	const unsigned int sideStride = static_cast<unsigned int>(slice + 1);

	// 側面インデックス（外側から見て CW）
	for (int i = 0; i < slice; ++i)
	{
		const unsigned int a = i;                 // 下段
		const unsigned int b = i + sideStride;    // 上段

		indices.push_back(a);
		indices.push_back(b);
		indices.push_back(a + 1);

		indices.push_back(a + 1);
		indices.push_back(b);
		indices.push_back(b + 1);
	}

	// ===== 下キャップ (y = -halfH) =====
	// D3D11 既定の CW を満たすには、「下から見て」CW＝「上から見て」CCW。
	// よって上から見たとき CCW になる (center, i, i+1) を採用。
	const unsigned int bottomCenter = static_cast<unsigned int>(vertices.size());
	{
		Vertex c{};
		c.pos[0] = 0.0f; c.pos[1] = -halfH; c.pos[2] = 0.0f;
		c.uv[0] = 0.5f;  c.uv[1] = 0.5f;
		vertices.push_back(c);

		const unsigned int ringStart = static_cast<unsigned int>(vertices.size());
		for (int i = 0; i <= slice; ++i)
		{
			const float t = static_cast<float>(i) / slice;
			const float theta = XM_2PI * t;
			const float x = std::cos(theta) * radius;
			const float z = std::sin(theta) * radius;

			Vertex v{};
			v.pos[0] = x; v.pos[1] = -halfH; v.pos[2] = z;
			// 中心(0.5,0.5) の平面マッピング
			v.uv[0] = 0.5f + (x / (radius * 2.0f));
			v.uv[1] = 0.5f - (z / (radius * 2.0f));
			vertices.push_back(v);
		}
		for (int i = 0; i < slice; ++i)
		{
			indices.push_back(bottomCenter);
			indices.push_back(ringStart + i);
			indices.push_back(ringStart + i + 1); // ← 上から見て CCW
		}
	}

	// ===== 上キャップ (y = +halfH) =====
	// 上から見たときに CW であれば可視（D3D11 既定）。
	// よって (center, i+1, i) にして CW を作る。
	const unsigned int topCenter = static_cast<unsigned int>(vertices.size());
	{
		Vertex c{};
		c.pos[0] = 0.0f; c.pos[1] = +halfH; c.pos[2] = 0.0f;
		c.uv[0] = 0.5f;  c.uv[1] = 0.5f;
		vertices.push_back(c);

		const unsigned int ringStart = static_cast<unsigned int>(vertices.size());
		for (int i = 0; i <= slice; ++i)
		{
			const float t = static_cast<float>(i) / slice;
			const float theta = XM_2PI * t;
			const float x = std::cos(theta) * radius;
			const float z = std::sin(theta) * radius;

			Vertex v{};
			v.pos[0] = x; v.pos[1] = +halfH; v.pos[2] = z;
			v.uv[0] = 0.5f + (x / (radius * 2.0f));
			v.uv[1] = 0.5f - (z / (radius * 2.0f));
			vertices.push_back(v);
		}
		for (int i = 0; i < slice; ++i)
		{
			indices.push_back(topCenter);
			indices.push_back(ringStart + i + 1);
			indices.push_back(ringStart + i);     // ← 上から見て CW
		}
	}

	// ===== バッファ作成 =====
	MeshBuffer::Description desc{};
	desc.pVtx = vertices.data();
	desc.vtxCount = static_cast<UINT>(vertices.size());
	desc.vtxSize = sizeof(Vertex);
	desc.pIdx = indices.data();
	desc.idxCount = static_cast<UINT>(indices.size());
	desc.idxSize = sizeof(unsigned int);
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	mMeshBuffer = std::make_shared<MeshBuffer>();
	mMeshBuffer->Create(desc);
}

//void MeshFilter::BuildCylinder()
//{
//	using namespace DirectX;
//
//	const int slice = 24;          // 角度分割
//	const float radius = 0.5f;     // 半径
//	const float halfH = 0.5f;     // 高さ/2
//
//	std::vector<Vertex> vertices;
//	std::vector<unsigned int> indices;
//
//	// -------- 側面頂点 --------
//	// 縦は bottom(0) / top(1) の2本
//	for (int yStep = 0; yStep <= 1; ++yStep)
//	{
//		const float y = (yStep == 0) ? -halfH : +halfH;
//		const float v = static_cast<float>(yStep); // 0 or 1
//
//		for (int i = 0; i <= slice; ++i)
//		{
//			const float t = static_cast<float>(i) / slice; // 0..1
//			const float theta = XM_2PI * t;
//			const float x = std::cos(theta) * radius;
//			const float z = std::sin(theta) * radius;
//
//			Vertex vert{};
//			vert.pos[0] = x;
//			vert.pos[1] = y;
//			vert.pos[2] = z;
//			vert.uv[0] = t; // U: 角度
//			vert.uv[1] = v; // V: 高さ
//			vertices.push_back(vert);
//		}
//	}
//	const unsigned int sideStride = static_cast<unsigned int>(slice + 1);
//
//	// -------- 側面インデックス --------
//	for (int i = 0; i < slice; ++i)
//	{
//		const unsigned int a = i;                 // bottom row
//		const unsigned int b = i + sideStride;    // top row
//
//		indices.push_back(a);
//		indices.push_back(b);
//		indices.push_back(a + 1);
//
//		indices.push_back(a + 1);
//		indices.push_back(b);
//		indices.push_back(b + 1);
//	}
//
//	// -------- 下キャップ（triangle fan）--------
//	const unsigned int bottomCenterIndex = static_cast<unsigned int>(vertices.size());
//	{
//		Vertex c{};
//		c.pos[0] = 0.0f; c.pos[1] = -halfH; c.pos[2] = 0.0f;
//		c.uv[0] = 0.5f; c.uv[1] = 0.5f;
//		vertices.push_back(c);
//
//		const unsigned int ringStart = static_cast<unsigned int>(vertices.size());
//		for (int i = 0; i <= slice; ++i)
//		{
//			const float t = static_cast<float>(i) / slice;
//			const float theta = XM_2PI * t;
//			const float x = std::cos(theta) * radius;
//			const float z = std::sin(theta) * radius;
//
//			Vertex v{};
//			v.pos[0] = x; v.pos[1] = -halfH; v.pos[2] = z;
//			// 平面マッピング（中心(0.5,0.5)）
//			v.uv[0] = 0.5f + (x / (radius * 2.0f));
//			v.uv[1] = 0.5f - (z / (radius * 2.0f));
//			vertices.push_back(v);
//		}
//		for (int i = 0; i < slice; ++i)
//		{
//			indices.push_back(bottomCenterIndex);
//			indices.push_back(ringStart + i + 1);
//			indices.push_back(ringStart + i);
//		}
//	}
//
//	// -------- 上キャップ（triangle fan）--------
//	const unsigned int topCenterIndex = static_cast<unsigned int>(vertices.size());
//	{
//		Vertex c{};
//		c.pos[0] = 0.0f; c.pos[1] = +halfH; c.pos[2] = 0.0f;
//		c.uv[0] = 0.5f; c.uv[1] = 0.5f;
//		vertices.push_back(c);
//
//		const unsigned int ringStart = static_cast<unsigned int>(vertices.size());
//		for (int i = 0; i <= slice; ++i)
//		{
//			const float t = static_cast<float>(i) / slice;
//			const float theta = XM_2PI * t;
//			const float x = std::cos(theta) * radius;
//			const float z = std::sin(theta) * radius;
//
//			Vertex v{};
//			v.pos[0] = x; v.pos[1] = +halfH; v.pos[2] = z;
//			v.uv[0] = 0.5f + (x / (radius * 2.0f));
//			v.uv[1] = 0.5f - (z / (radius * 2.0f));
//			vertices.push_back(v);
//		}
//		for (int i = 0; i < slice; ++i)
//		{
//			indices.push_back(topCenterIndex);
//			indices.push_back(ringStart + i);
//			indices.push_back(ringStart + i + 1);
//		}
//	}
//
//	// ---- バッファ作成 ----
//	MeshBuffer::Description desc{};
//	desc.pVtx = vertices.data();
//	desc.vtxCount = static_cast<UINT>(vertices.size());
//	desc.vtxSize = sizeof(Vertex);
//	desc.pIdx = indices.data();
//	desc.idxCount = static_cast<UINT>(indices.size());
//	desc.idxSize = sizeof(unsigned int);
//	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
//
//	mMeshBuffer = std::make_shared<MeshBuffer>();
//	mMeshBuffer->Create(desc);
//}

