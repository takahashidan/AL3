#include "MapChipField.h"
#include <cassert>
#include <fstream>
#include <map>
#include <sstream>

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
	mapChipData_.deta.clear();
	mapChipData_.deta.resize(kNumBlockVirtical);
	for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.deta) {
		mapChipDataLine.resize(kNumBlockHorizontal);
	}
}


void MapChipField::LoadMapChipCsv(const std::string& filePath)
{
	ResetMapChipData();

	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

	std::stringstream mapChipCsv;

	mapChipCsv << file.rdbuf();
	file.close();

	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) 
	{
		std::string line;
		getline(mapChipCsv, line);
		std::istringstream line_stream(line);
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j)
		{
			std::string word;
			getline(line_stream, word, ',');

			if (mapChipTable.contains(word))
			{
				mapChipData_.deta[i][j] = mapChipTable[word];
			}
		}
	}
}

MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) 
{

	if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex) 
	{
		return MapChipType::kBlank;
	}

	if (yIndex < 0 || kNumBlockVirtical - 1 < yIndex) 
	{
		return MapChipType::kBlank;
	}

	return mapChipData_.deta[yIndex][xIndex];
}

Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) 
{
	float x = kBlockWidth * xIndex;
	uint32_t iy = (kNumBlockVirtical - 1 - yIndex);
	float y = kBlockHeight * iy;
	return Vector3(x, y, 0);
}

IndexSet MapChipField::GetMapChipIndexSetByPoition(const Vector3& position)
{ 
	IndexSet indexSet = {};
	float x = (position.x + kBlockWidth / 2) / kBlockWidth;
	float y = (position.y + kBlockHeight / 2) / kBlockHeight;
	indexSet.xIndex = static_cast<uint32_t>(x);
	indexSet.yIndex = kNumBlockVirtical - 1 - static_cast<uint32_t>(y);
	return indexSet; 
}

Rect MapChipField::GetRectByIndex(uint32_t xIndex, uint32_t yIndex) 
{
	Vector3 center = GetMapChipPositionByIndex(xIndex, yIndex);

	Rect rect;
	rect.left = center.x - kBlockWidth / 2.0f;
	rect.right = center.x + kBlockWidth / 2.0f;
	rect.bottom = center.y - kBlockHeight / 2.0f;
	rect.top = center.y + kBlockHeight / 2.0f;
	return rect;
}


uint32_t MapChipField::GetNumBlockVirtical() { return kNumBlockVirtical; }
uint32_t MapChipField::GetNumBlockHorizontal() { return kNumBlockHorizontal; }
