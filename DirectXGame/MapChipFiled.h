#pragma once
#include <vector>
#include <string>
#include <Vector3.h>


/// <sumary>
/// マップチップフィールド
/// <sumary>

enum class MapChipType {
kBlank, // 空白
kBlock, // ブロック
};

struct  MapChipData 
{
std::vector<std::vector<MapChipType>> deta;
};

class MapChipField 
{
public:

void ResetMapChipData();
uint32_t GetNumBlockVirtical() const;

uint32_t GetNumBlockHorizontal() const;


void LoadMapChipCsv(const std::string& filePath);
MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);


MapChipData mapChipData_;

private:
//1ブロックのサイズ
static inline const float kBlockWidth = 1.0f;
static inline const float kBlockHeight = 1.0f;

//ブロックの個数
static inline const uint32_t kNumBlockVirtical = 20;
static inline const uint32_t kNumBlockHorizontal = 100;

};