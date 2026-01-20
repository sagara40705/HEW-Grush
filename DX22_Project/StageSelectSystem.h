#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "Vector3.h"

//====================
//
// データ定義
// 
//====================

enum class NodeType
{
	Start,
	Stage,
	Junction,
	Castle
};

// ノードクラス
class MapNode
{
public:
	int id;
	std::string name;
	NodeType type;
	Vector3 position; // Unity互換の座標を使用

	int stageid; // StageSceneへ渡すID
	bool isUnlocked;
	bool isCleared;

	// 隣接ノード [0:Uo. 1: Down 2:Left 3:Right]	
	MapNode* neighbors[4]; 

	MapNode(int _id, const std::string& _name, NodeType _type, const Vector3& _position, int _stageid)
		: id(_id), name(_name), type(_type), position(_position), stageid(_stageid), isUnlocked(false), isCleared(false)
	{
		for (int i = 0; i < 4; ++i)
		{
			neighbors[i] = nullptr;
		}
	}
};



//====================	
// システムクラス
//====================

// マップグラフ管理
class MapGraph
{
public:
	std::vector<MapNode*> nodes;
	
	// 方向定数
	static const int DIR_UP = 0;
	static const int DIR_DOWN = 1;
	static const int DIR_LEFT = 2;
	static const int DIR_RIGHT = 3;

	MapNode* GetNextNode(MapNode* current, int direction)
	{
		if (!current) return nullptr;
		MapNode* next = current->neighbors[direction];

		// ロック判定:接続先があり、解放済みなら移動可
		if (next && next->isUnlocked)
		{
			return next;
		}
		return nullptr;
	}

	// 後始末
	~MapGraph()
	{
		for(auto node : nodes) delete node;
		nodes.clear();
	}
};

