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
	// �ܹ�Ҫ�ü��ľ�������
	int m_nRect;
	// ����Ҫ�ü��ľ���
	std::vector<Rectangle*> m_rectangles;

    // �㷨����ʱ��
    std::chrono::steady_clock::time_point m_beginTime;
};

