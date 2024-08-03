#pragma once

#include "WorldTransform.h"
#include <Vector3.h>
#include <string>
#include <vector>
#include "Rect.h"

enum class MapChipType {
	kBlank, 
	kBlock, 
};

struct MapChipData {
	std::vector<std::vector<MapChipType>> deta;
};

struct IndexSet {
	uint32_t xIndex;
	uint32_t yIndex;
};

class MapChipField {
public:
	void ResetMapChipData();
	uint32_t GetNumBlockVirtical();

	uint32_t GetNumBlockHorizontal();

	void LoadMapChipCsv(const std::string& filePath);
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);
	IndexSet GetMapChipIndexSetByPoition(const Vector3& position);
	Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);

private:
	static inline const float kBlockWidth = 2.0f;
	static inline const float kBlockHeight = 2.0f;

	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	MapChipData mapChipData_;
	WorldTransform worldTransformBlocks_;
};
