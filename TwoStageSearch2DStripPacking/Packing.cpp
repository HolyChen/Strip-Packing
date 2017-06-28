#include "stdafx.h"
#include <memory>
#include <algorithm>
#include <numeric>
#include <random>
#include <chrono>
#include <iostream>
#include <cassert>

#include "Packing.h"
#include "Rectangle.h"
#include "Space.h"
#include "SpaceManager.h"
#include "PackingBase.h"
#include "PackingLocalSearch.h"
#include "PackingSimulatedAnnealing.h"
#include "PackingGenetic.h"

const std::chrono::seconds Packing::MAX_RUNTIME(59);

Packing::Packing(double sheetWidth, int nRect)
	: m_sheetWidth(sheetWidth), m_nRect(nRect)
{
	
}

Packing::~Packing()
{
	for (auto& rect : m_rectangles)
	{
		if (rect != nullptr)
		{
			delete rect;
		}
	}
}

void Packing::pushRectangle(const Rectangle & rect)
{
	pushRectangle(new Rectangle(rect));
}

void Packing::pushRectangle(Rectangle * const rect) throw(std::invalid_argument)
{
	if (rect->width > m_sheetWidth)
	{
		throw std::invalid_argument("Any rectangle's width shuold be less than sheet's width.");
	}
	m_rectangles.push_back(rect);
}

PackingList Packing::isa()
{
    m_beginTime = std::chrono::steady_clock::now();

    m_bestSearchList = m_rectangles;
    m_bestPacking.searchList = m_bestSearchList;
    m_bestPacking.h = std::numeric_limits<double>::max();

    m_bestPacking = PackingLocalSearch(m_sheetWidth, m_rectangles, m_bestPacking)();
	m_bestPacking = PackingSimulatedAnnealing(m_sheetWidth, m_rectangles, m_bestPacking)();
    m_bestPacking = PackingGenetic(m_sheetWidth, m_rectangles, m_bestPacking)();
	return m_bestPacking;
}
