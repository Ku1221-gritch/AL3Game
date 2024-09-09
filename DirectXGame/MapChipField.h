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
	// マップチップ1マスのサイズ
	static inline const float kMapChipWidth = 2.0f;
	static inline const float kMapChipHeight = 2.0f;

	// マップチップ1マスの個数
	static inline const uint32_t kNumMapChipVirtical = 46;
	static inline const uint32_t kNumMapChipHorizontal = 46;
	const uint32_t GetNumMapChipVirtical() { return kNumMapChipVirtical; };
	const uint32_t GetNumMapChipHorizontal() { return kNumMapChipHorizontal; };

	void ResetMapChipData();

	void LoadMapChipCsv(const std::string& filePath);

	Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);

	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	MapChipData mapChipData_;

	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);
};
