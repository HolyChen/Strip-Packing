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
    // ���Ƚ�����ԭʼ��Ϊ���ĵ�����
    for (int i = 0; i < m_nRect - 1; i++)
    {
        for (int j = i + 1; j < m_nRect; j++)
        {
            // �����ж��Ƿ�ʱ
            if (std::chrono::steady_clock::now() - m_beginTime >= m_maxRunTime)
            {
                result = m_bestPacking;
                out.clear();
                out.push_back(m_bestPacking);
                out.push_back(heuristicPacking(curSearchList));
                return;
            }
            auto newSearchList(curSearchList);
            // ������i��j��Ԫ��λ��
            {
                auto temp = newSearchList.at(i);
                newSearchList.at(i) = newSearchList.at(j);
                newSearchList.at(j) = temp;
            }

            auto currentPacking = heuristicPacking(newSearchList);
            // �������µ����е������������ԭʼ���У����滻ԭʼ����Ϊ�����У�������ȫ����������
            if (currentPacking.h <= m_bestPacking.h)
            {
                m_bestPacking = currentPacking;
                m_bestSearchList = newSearchList;
            }

        }
    }*/

    // �������ʱ����������ƶ����������������ü���ʱ��
    for (int i = 0; i < m_nRect - 1; i++)
    {
        for (int j = i + 1; j < m_nRect; j++)
        {
            // �����ж��Ƿ�ʱ
            if (std::chrono::steady_clock::now() - m_beginTime >= m_maxRunTime)
            {
                result = m_bestPacking;
                out.clear();
                out.push_back(heuristicPacking(curSearchList));
                return;
            }
            auto newSearchList(curSearchList);
            // ������i��j��Ԫ��λ��
            {
                auto temp = newSearchList.at(i);
                newSearchList.at(i) = newSearchList.at(j);
                newSearchList.at(j) = temp;
            }

            auto currentPacking = heuristicPacking(newSearchList);
            // �������µ����е������������ԭʼ���У����滻ԭʼ����Ϊ�����У�������ȫ����������
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
