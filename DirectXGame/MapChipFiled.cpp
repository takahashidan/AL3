#include "MapChipFiled.h"
#include <cassert>
#include <sstream>
#include <fstream>
#include <map>

namespace
{
std::map<std::string, MapChipType> mapChipTable = 
{
{"0", MapChipType::kBlank},
{"0", MapChipType::kBlock},
};

}



void MapChipField::ResetMapChipData() 
{
mapChipData_.deta.clear(); 
mapChipData_.deta.resize(kNumBlockVirtical);
for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.deta)
{
mapChipDataLine.resize(kNumBlockHorizontal);
}
}

uint32_t MapChipField::GetNumBlockVirtical() const { return kNumBlockVirtical; }
uint32_t MapChipField::GetNumBlockHorizontal() const { return kNumBlockHorizontal; }

void MapChipField::LoadMapChipCsv(const std::string& filePath) 
{
ResetMapChipData();

std::ifstream file;
file.open(filePath);

assert(file.is_open());

//マップチップCSV
std::stringstream mapChipCsv;

//ファイルの内容を文字列ストリームにコピー
mapChipCsv << file.rdbuf();
//ファイルを閉じる
file.close();

for (uint32_t i = 0; i < kNumBlockVirtical; ++i)
{
std::string line;
getline(mapChipCsv, line);

std::istringstream line_stream(line);

for (uint32_t j = 0; j < kNumBlockHorizontal; ++j)
{
std::string word;
getline(line_stream, word);

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
return MapChipType::kBlock;
}

return mapChipData_.deta[yIndex][xIndex];
}

Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) 
{ 
return Vector3 (kBlockWidth + xIndex, kBlockHeight*(kNumBlockVirtical - 1 - yIndex),0);
}