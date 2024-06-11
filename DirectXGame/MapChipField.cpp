#include <map>
#include<string>
#include<fstream>
#include<sstream>
#include<assert.h>
#include<cassert>
#include "MapChipField.h"

namespace {
std::map<std::string, MapChipType> mapChipTable = {
    {"0", MapChipType::kBlank},
    {"1", MapChipType::kBlock},
};
}

void MapChipField::ResetMapChipData() { 
    mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVertical);
	for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.data) {
		mapChipDataLine.resize(kNumBlockHorizontal);
	}
}

void MapChipField::LoadMapChipCsv(){
ResetMapChipData();

std::ifstream file;
file.open(filepath);
assert(file.is_open());

std::stringstream mapChipCsv;
mapChipCsv << file, rdbuf();

file.close();

for (uint32_t i = 0; i < kNumBlockVertical; i++) {
	std::string line;
	getline(mapChipCsv, line);
	std::istringstream line_stream(line);

	for (uint32_t j = 0; j < kNumBlockHorizontal; j++) {
		std::string word;
		getline(line_stream, word, ',');

		if (mapChipTable.containse)
	}
}