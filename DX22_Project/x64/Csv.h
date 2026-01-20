#pragma once 
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <DirectXMath.h> 

using namespace DirectX;

// CSVの1行分のデータを格納する構造体
struct ObjectData
{
    int typeID;        // PrefabId に対応する整数
    XMFLOAT3 position; // 位置 (x, y, z)
    XMFLOAT3 rotation; // 回転 (x, y, z)
};

class CsvLoader
{
public:
    // CSVを読み込んでベクターで返す関数
    // 想定フォーマット: TypeID, PosX, PosY, PosZ, RotX, RotY, RotZ
    static std::vector<ObjectData> Load(const std::string& filename)
    {
        std::vector<ObjectData> result;
        std::ifstream file(filename);

        if (!file.is_open())
        {
            std::cerr << "Error: ファイルが見つかりません: " << filename << std::endl;
            return result;
        }

        std::string line;

        // 1行ずつ読み込む
        while (std::getline(file, line))
        {
            // コメント行（#で始まる）や空行はスキップ
            if (line.empty() || line[0] == '#' || line[0] == '/') continue;

            std::stringstream ss(line);
            std::string segment;
            std::vector<std::string> cells;

            // カンマ区切りで分解する
            while (std::getline(ss, segment, ','))
            {
                cells.push_back(segment);
            }

            // データが7つ（ID, px, py, pz, rx, ry, rz）揃っているか確認
            if (cells.size() >= 7)
            {
                ObjectData data;
                try {
                    // 文字列を数値に変換
                    data.typeID = std::stoi(cells[0]);

                    data.position.x = std::stof(cells[1]);
                    data.position.y = std::stof(cells[2]);
                    data.position.z = std::stof(cells[3]);

                    data.rotation.x = std::stof(cells[4]);
                    data.rotation.y = std::stof(cells[5]);
                    data.rotation.z = std::stof(cells[6]);

                    result.push_back(data);
                }
                catch (...) {
                    // 数値変換エラーなどはスキップしてログを出す
                    std::cerr << "CSV Parse Error in line: " << line << std::endl;
                }
            }
        }

        file.close();
        return result;
    }
};