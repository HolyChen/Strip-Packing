#include "stdafx.h"
#include "PackingLocalSearch.h"

#include <algorithm>


PackingLocalSearch::PackingLocalSearch(double sheetWidth, const std::vector<Rectangle*>& rectangles, const PackingList& bestPacking, std::chrono::microseconds runtime)
    : PackingBase(sheetWidth, rectangles, bestPacking, runtime), isSingleSearch(true)
{
}

PackingLocalSearch::~PackingLocalSearch()
{
}

void PackingLocalSearch::operator()(PackingList& result, std::vector<PackingList> &out)
{
    if (isSingleSearch)
    {
        singleSearch(result, out);
    }
}

void PackingLocalSearch::singleSearch(PackingList & result, std::vector<PackingList> &out)
{
    std::vector<Rectangle*> curSearchList(m_bestSearchList);
    m_bestPacking = heuristicPacking(curSearchList);

    /*
    // 首先进行以原始解为中心的搜索
    for (int i = 0; i < m_nRect - 1; i++)
    {
        for (int j = i + 1; j < m_nRect; j++)
        {
            // 这里判断是否超时
            if (std::chrono::steady_clock::now() - m_beginTime >= m_maxRunTime)
            {
                result = m_bestPacking;
                out.clear();
                out.push_back(m_bestPacking);
                out.push_back(heuristicPacking(curSearchList));
                return;
            }
            auto newSearchList(curSearchList);
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
            }

        }
    }*/

    // 如果还有时间继续进行移动解的搜索，充分利用计算时间
    for (int i = 0; i < m_nRect - 1; i++)
    {
        for (int j = i + 1; j < m_nRect; j++)
        {
            // 这里判断是否超时
            if (std::chrono::steady_clock::now() - m_beginTime >= m_maxRunTime)
            {
                result = m_bestPacking;
                out.clear();
                out.push_back(heuristicPacking(curSearchList));
                return;
            }
            auto newSearchList(curSearchList);
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
                curSearchList = newSearchList;
            }

        }
    }

    out.clear();
    out.push_back(heuristicPacking(curSearchList));
    result = m_bestPacking;
}
