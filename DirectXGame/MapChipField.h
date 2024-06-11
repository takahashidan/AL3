#include <stdint.h>
#include <vector>
#include"Vector3.h"

enum class MapChipType {
	kBlank, // 空白
	kBlock, // ブロック
};

struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};

class MapChipField {
public:

	void ResetMapChipData();
	void LoadMapChipCsv(const std::string& filePath);

private:

	// ブロックサイズ
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;
	// ブロックの個数
	static inline const uint32_t kNumBlockVertical = 20;
	static inline const uint32_t kNumBlockHorizontal = 20;
	MapChipData mapChipData_;
};