#pragma once
#include "Space.h"
#include "Rectangle.h"
#include <vector>
#include "PackingList.h"

#include <random>


class SpaceManager
{
private:
	std::vector<Space*> m_spaces;
	int m_sheetWidth;
	std::default_random_engine eg;
public:
	SpaceManager(int sheetWidth = 0);
	~SpaceManager();

	// 如果当前用完所有的可用的空间，那么重新设置整个画布上方未使用的部分作为可用空间

	// 将一个矩形pack到第index空间上
	Assign pack(int index, const Rectangle* const);
	// 删除第index个空间
	void erase(int index);
	// 获取最下、最左侧的空间的索引位置
	int lowestLeftest();
	// 根据空间的索引位置获取该空间
	Space *const get(int index) const;
};

