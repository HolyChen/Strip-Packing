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

const std::chrono::seconds PackingBase::MAX_RUNTIME(59);

PackingBase::PackingBase(double sheetWidth, const std::vector<Rectangle*>& rectangles, const PackingList& bestPacking)
    : m_sheetWidth(sheetWidth), m_nRect(rectangles.size()), m_rectangles(rectangles), m_bestPacking(bestPacking),
      m_bestSearchList(bestPacking.searchList)
{
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

        // 优先选择评分最高的
        auto rectangle = popHighestScoreRectangle(searchList, space, judegYang);
        // 但是如果评分最高的矩形，也是一个非常差的矩形，即宽度、h1、h2都不相等，
        // 那么把他放回序列，且选择一个宽度最大且不超过当前空间的
        //if (rectangle->width != space->width && rectangle->height != space->h1 && rectangle->height != space->h2)
        //{
        //	searchList.push_front(rectangle);
        //	rectangle = popMaxWidthRectangle(searchList, space->width);
        //}
        if (space->y + rectangle->height > packingList.h)
        {
            packingList.h = space->y + rectangle->height;
        }
        packingList.assgins.push_back(spaces.pack(spaceIndex, rectangle));
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
    int curScore = 0;
    for (auto iter = searchList.begin(); iter != searchList.end(); iter++)
    {
        curScore = judge(*iter, space);
        if (curScore > highestScore)
        {
            highestScore = curScore;
            highestScored = iter;
        }
    }

    Rectangle *toReturn = nullptr;
    if (highestScored != searchList.end())
    {
        toReturn = *highestScored;
        searchList.erase(highestScored);
    }
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
