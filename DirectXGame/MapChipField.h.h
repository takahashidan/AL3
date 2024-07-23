#pragma once
#include <vector>
#include <string>
#include <Vector3.h>


enum class MapChipType
{
	kBlank,
	kBlock,
};

struct MapChipData 
{
	std::vector<std::vector<MapChipType>> data;
};

class MapChipField
{
public:

	void ResetMapChipData();
	void LoadMapChipCsv(const std::string& filePath);
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

private:

	static inline const float kBockWidth = 1.0f;
	static inline const float kBockHeight = 1.0f;

	static inline const uint32_t kNumBockVirtical = 20;
	static inline const uint32_t kNumBockHorizontal = 100;

	MapChipData* mapChipData_;
	WorldTransform worldTransformBlocks_;

};