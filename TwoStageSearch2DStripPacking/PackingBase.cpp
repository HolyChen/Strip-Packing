#include "stdafx.h"

#include <memory>
#include <algorithm>
#include <numeric>
#include <random>
#include <chrono>
#include <iostream>
#include <cassert>

#include "PackingBase.h"
#include "Rectangle.h"
#include "Space.h"
#include "SpaceManager.h"

PackingBase::PackingBase(double sheetWidth, const std::vector<Rectangle*>& rectangles, const PackingList& bestPacking, std::chrono::microseconds runtime)
    : m_sheetWidth(sheetWidth), m_nRect(rectangles.size()), m_rectangles(rectangles), m_bestPacking(bestPacking),
      m_bestSearchList(bestPacking.searchList), m_maxRunTime(runtime)
{
    m_bestPacking = heuristicPacking(m_bestSearchList);
    m_beginTime = std::chrono::steady_clock::now();
}

PackingBase::~PackingBase()
{
}

PackingList PackingBase::heuristicPacking(const std::vector<Rectangle*>& searchListSrc) const
{
    PackingList packingList;

    double minWidth = 0.0;

    std::list<Rectangle*> searchList(searchListSrc.cbegin(), searchListSrc.cend());

    SpaceManager spaces(m_sheetWidth);

    for (int pin = 0; pin < m_nRect; pin++)
    {
        minWidth = getMinWidth(searchList);

        int spaceIndex = spaces.lowestLeftest();
        const Space * space = spaces.get(spaceIndex);
        while (space->width < minWidth)
        {
            spaces.erase(spaceIndex);
            spaceIndex = spaces.lowestLeftest();
            space = spaces.get(spaceIndex);
        }

        // ����ѡ��������ߵ�
        auto rectangle = popHighestScoreRectangle(searchList, space, judegYang);
        // �������������ߵľ��Σ�Ҳ��һ���ǳ���ľ��Σ�����ȡ�h1��h2������ȣ�
        if (space->y + rectangle->height > packingList.h)
        {
            packingList.h = space->y + rectangle->height;
        }
        packingList.assigns.push_back(spaces.pack(spaceIndex, rectangle));
    }

    packingList.searchList = searchListSrc;

    return packingList;
}


double PackingBase::getMinWidth(const std::list<Rectangle*>& searchList) const
{
    double minWidth = std::numeric_limits<double>::max();
    for (auto rect : searchList)
    {
        minWidth = std::min(minWidth, rect->width);
    }
    return minWidth;
}

Rectangle* PackingBase::popHighestScoreRectangle(std::list<Rectangle*>& searchList, const Space* const space, int(*judge)(const Rectangle* const, const Space* const)) const
{
    auto highestScored = searchList.begin();
    int highestScore = std::numeric_limits<int>::min();
    int *scores = new int[searchList.size()];

    {
        int i = 0;
        for (auto iter = searchList.begin(); iter != searchList.end(); iter++)
        {
            scores[i] = judge(*iter, space);
            if (scores[i] > highestScore)
            {
                highestScore = scores[i];
                highestScored = iter;
            }
            i++;
        }
    }

    // ��С�˷Ѳ���
    // �������Ϊ2��1����ôѡ��һ���߶���ȣ��߶Ⱦ����ܴ�ľ���
    if (highestScore == 2 || highestScore == 1)
    {
        int i = 0;
        for (auto iter = searchList.begin(); iter != searchList.end(); iter++)
        {
            if (highestScore == scores[i] && (*iter)->width > (*highestScored)->width)
            {
                highestScored = iter;
            }
            i++;
        }
    }
    // �������Ϊ0����ôѡ��һ����͸߶���С������
    else if (highestScore == 0)
    {
        int i = 0;
        for (auto iter = searchList.begin(); iter != searchList.end(); iter++)
        {
            if (highestScore == scores[i] && (*iter)->width >= (*highestScored)->width && (*iter)->height >= (*highestScored)->height)
            {
                highestScored = iter;
            }
            i++;
        }
    }


    Rectangle *toReturn = nullptr;
    if (highestScored != searchList.end())
    {
        toReturn = *highestScored;
        searchList.erase(highestScored);
    }
    delete[] scores;
    return toReturn;
}

Rectangle* PackingBase::popMaxWidthRectangle(std::list<Rectangle*>& searchList, double spaceWidth) const
{
    auto maxWidthRect = searchList.begin();
    auto iter = searchList.begin();
    for (iter++; iter != searchList.end(); iter++)
    {
        if ((*iter)->width <= spaceWidth && (*iter)->width > (*maxWidthRect)->width)
        {
            maxWidthRect = iter;
        }
    }

    Rectangle *toReturn = nullptr;
    if (maxWidthRect != searchList.end())
    {
        toReturn = *maxWidthRect;
        searchList.erase(maxWidthRect);
    }
    return toReturn;
}

int PackingBase::judgeZhang(const Rectangle * const rect, const Space * const space)
{
    if (rect->width > space->width)
    {
        return -1;
    }

    if (rect->width == space->width)
    {
        if (rect->height == space->h1 || rect->height == space->h2)
        {
            return 4;
        }
        else if (rect->height > space->h1 && rect->height > space->h2)
        {
            return 3;
        }
        else
        {
            return 2;
        }
    }
    else
    {
        if (rect->height == space->h1 || rect->height == space->h2)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }

}

int PackingBase::judegYang(const Rectangle * const rect, const Space * const space)
{
    if (rect->width > space->width)
    {
        return -1;
    }

    double highWallH = 0.0, lowWallH = 0.0;
    if (space->h1 >= space->h2)
    {
        highWallH = space->h1;
        lowWallH = space->h2;
    }
    else
    {
        highWallH = space->h2;
        lowWallH = space->h1;
    }
    if (rect->width == space->width)
    {
        if (rect->height == highWallH)
        {
            return 6;
        }
        else if (rect->height == lowWallH)
        {
            return 5;
        }
        else if (rect->height > highWallH)
        {
            return 4;
        }
        else
        {
            return 3;
        }
    }
    else
    {
        if (rect->height == highWallH)
        {
            return 2;
        }
        else if (rect->height == lowWallH)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    return 0;
}
