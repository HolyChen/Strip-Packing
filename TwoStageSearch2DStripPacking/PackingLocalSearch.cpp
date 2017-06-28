#include "stdafx.h"
#include "PackingLocalSearch.h"

#include <algorithm>


PackingLocalSearch::PackingLocalSearch(double sheetWidth, const std::vector<Rectangle*>& rectangles, const PackingList& bestPacking)
    : PackingBase(sheetWidth, rectangles, bestPacking)
{
}


PackingLocalSearch::~PackingLocalSearch()
{
}

PackingList PackingLocalSearch::operator()()
{

    std::vector<Rectangle*> curSeachList(m_rectangles);
    std::sort(curSeachList.begin(), curSeachList.end(),
        static_cast<bool(*)(const Rectangle::_PRectangle&, const Rectangle::_PRectangle&)>(Rectangle::comparatorPerimeter));
    m_bestPacking = heuristicPacking(curSeachList);
    m_bestSearchList = curSeachList;
    for (int i = 0; i < m_nRect - 1; i++)
    {
        for (int j = i + 1; j < m_nRect; j++)
        {
            auto newSearchList(curSeachList);
            // 交换第i、j个元素位置
            {
                auto temp = newSearchList.at(i);
                newSearchList.at(i) = newSearchList.at(j);
                newSearchList.at(j) = temp;
            }

            auto currentPacking = heuristicPacking(newSearchList);
            // 如果这个新的序列的搜索结果好于原始序列，则替换原始序列为新序列，并更新全局最优序列
            if (currentPacking.h <= m_bestPacking.h)
            {
                m_bestPacking = currentPacking;
                m_bestSearchList = newSearchList;
                curSeachList = newSearchList;
            }

            // 这里判断是否超时
            if (std::chrono::steady_clock::now() - m_beginTime >= MAX_RUNTIME)
            {
                return m_bestPacking;
            }
        }
    }
    return m_bestPacking;
}
