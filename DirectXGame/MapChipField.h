#include <Rect.h>
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#pragma once
enum class MapChipType {
	kBlank, // 空白
	kBlock, // ブロック
};

struct IndexSet {
	uint32_t xIndex;
	uint32_t yIndex;
};

struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};

class MapChipField {

public:
	// 1ブロックのサイズ
	static inline const float kBlockWidth = 2.0f;
	static inline const float kBlockHeight = 2.0f;

	// ブロックの個数
	static inline const uint32_t kNumBlockVirtical = 46;
	static inline const uint32_t kNumBlockHorizontal = 46;
	const uint32_t GetNumBlockVirtical() { return kNumBlockVirtical; };
	const uint32_t GetNumBlockHorizontal() { return kNumBlockHorizontal; };

	void ResetMapChipData();

	void LoadMapChipCsv(const std::string& filePath);

	Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);

	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	MapChipData mapChipData_;

	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);
};
