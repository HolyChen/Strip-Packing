#pragma once

#include <list>
#include <vector>
#include <stdexcept>
#include <chrono>

#include "PackingList.h"

class Rectangle;
class Space;

class Packing
{
public:
	Packing(double sheetWidth, int nRect);
	~Packing();
	void pushRectangle(const Rectangle & rect);
	void pushRectangle(Rectangle * const rect) throw(std::invalid_argument);
	PackingList isa();
private:
	double m_sheetWidth;
	PackingList m_bestPacking;
	std::vector<Rectangle*> m_bestSearchList;
	// 总共要裁剪的矩形数量
	int m_nRect;
	// 所有要裁剪的矩形
	std::vector<Rectangle*> m_rectangles;

    // 算法启动时间
    std::chrono::steady_clock::time_point m_beginTime;
};

