#include <iostream>
#include <fstream>  // ファイル読み込み用
#include <sstream>  // 文字列分割用
#include <string>
#include <vector>
#include <DirectXMath.h> 

using namespace DirectX;

// 1行分のデータを格納する構造体
struct ObjectData 
{
    XMFLOAT3 position; // 位置
    // 必要ならここに scale, rotation, type などを追加
};

class CsvLoader
{
public:
    // CSVを読み込んでベクターで返す関数
    static std::vector<ObjectData> Load(const std::string& filename)
    {
        std::vector<ObjectData> result;
        std::ifstream file(filename);

        // ファイルが開けなかった場合
        if (!file.is_open())
        {
            std::cerr << "Error: ファイルが見つかりません: " << filename << std::endl;
            return result;
        }

        std::string line;

        // 1行ずつ読み込む
        while (std::getline(file, line))
        {
            std::stringstream ss(line);
            std::string segment;
            ObjectData data;
            std::vector<std::string> cells;

            // カンマ区切りで分解する
            while (std::getline(ss, segment, ',')) 
            {
                cells.push_back(segment);
            }

            // データが3つ（x, y, z）揃っているか確認して格納
            if (cells.size() >= 3) 
            {
                // 文字列(string)を数値(float)に変換
                data.position.x = std::stof(cells[0]);
                data.position.y = std::stof(cells[1]);
                data.position.z = std::stof(cells[2]);

                result.push_back(data);
            }
        }

        file.close();
        return result;
    }
};