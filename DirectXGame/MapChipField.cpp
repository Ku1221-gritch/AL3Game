#include "GameScene.h"
#include"MapChipField.h"
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>

namespace 
{

	std::map<std::string, MapChipType> mapChipTable =
	{
	    {"0", MapChipType::kBlank},
	    {"1", MapChipType::kBlock},
	};

}

void MapChipField::ResetMapChipData() 
{
	// マップチップデータをリセット
	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumMapChipVirtical);
	for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.data) {
		mapChipDataLine.resize(kNumMapChipHorizontal);
	}
}

void MapChipField::LoadMapChipCsv(const std::string& filePath) 
{
	//マップチップデータをリセット
	ResetMapChipData();

	//ファイルを開く
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

	//マップチップCSV
	std::stringstream mapChipCsv;
	//ファイルの内容を文字列ストリームにコピー
	mapChipCsv << file.rdbuf();
	//ファイルを閉じる
	file.close();
	//CSVからマップチップデータを読み込む
	for (uint32_t i = 0; i < kNumMapChipVirtical; ++i) 
	{
		std::string line;
		getline(mapChipCsv, line);

		//１桁分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		for (uint32_t j = 0; j < kNumMapChipHorizontal; ++j) 
		{
			
			std::string word;
			getline(line_stream, word, ',');

			if (mapChipTable.contains(word)) 
			{
				mapChipData_.data[i][j] = mapChipTable[word];
			}
		}
	}
}

MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) 
{
	if (xIndex < 0 || kNumMapChipHorizontal - 1 < xIndex)
	{
		return MapChipType::kBlank;
	}
	if (yIndex < 0 || kNumMapChipVirtical - 1 < yIndex)
	{
		return MapChipType::kBlank;
	}
	return mapChipData_.data[yIndex][xIndex];
}

Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) 
{ 
	
	return Vector3(kMapChipWidth * xIndex,kMapChipHeight * (kNumMapChipVirtical - 1 - yIndex),0);

}

IndexSet MapChipField::GetMapChipIndexSetByPosition(const Vector3& position)
{ 
	IndexSet indexSet = {};
	indexSet.xIndex = ((uint32_t)position.x + (uint32_t)kMapChipWidth / 2) / (uint32_t)kMapChipWidth;
	indexSet.yIndex = (((uint32_t)kMapChipHeight * (uint32_t)(kNumMapChipVirtical-1) - (uint32_t)position.y) / (uint32_t)kMapChipHeight);
	return indexSet; 
}

Rect MapChipField::GetRectByIndex(uint32_t xIndex, uint32_t yIndex) 
{ 
	//指定マップチップの中心座標を取得する
	Vector3 center = GetMapChipPositionByIndex(xIndex, yIndex);

	Rect rect;
	rect.left = center.x - kMapChipWidth / 2.0f;
	rect.right = center.x + kMapChipWidth / 2.0f;
	rect.top = center.y - kMapChipHeight / 2.0f;
	rect.bottom = center.y + kMapChipHeight / 2.0f;

	return rect;
}
