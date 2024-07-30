<<<<<<< Updated upstream
﻿#include "MapChipField.h.h"
#include <map>
#include <fstream>
#include <sstream>
#include <cassert>

std::map<std::string, MapChipType> mapChipTable = 
{
	{"0", MapChipType::kBlank},
    {"1", MapChipType::kBlock},
};

void MapChipField::ResetMapChipData() {
	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBockVirtical);
	for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.data) {
		mapChipDataLine.resize(kNumBockHorizontal);
	}
};

void MapChipField::LoadMapChipCsv(const std::string& filePath) 
{

	//マップチップデータをリセット
	ResetMapChipData();

	//ファイルを開く
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

	//マップチップCSV(宣言)
	std::stringstream mapChipCsv;
	//ファイルの内容を文字列ストリームにコピー
	mapChipCsv << file.rdbuf();
	//ファイルを閉じる
	file.close();

	

	for (uint32_t i = 0; i < kNumBockVirtical; ++i)
	{
		std::string line;
		getline(mapChipCsv, line);

		std::istringstream line_stream(line);

		for (uint32_t j = 0; j < kNumBockHorizontal; ++j)
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
	if (xIndex < 0 || kNumBockHorizontal - 1 < xIndex)
	{
		return MapChipType::kBlank;
	}

	if (xIndex < 0 || kNumBockVirtical - 1 < yIndex) {
		return MapChipType::kBlank;
	}

	return mapChipData_.data[yIndex][xIndex]; 
}

Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) 
{
	return Vector3(kBockWidth * xIndex,kBockHeight * (kNumBockVirtical - 1 -yIndex),0); 
}


