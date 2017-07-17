#include "stdafx.h"
#include <memory>
#include <algorithm>
#include <numeric>
#include <random>
#include <chrono>
#include <iostream>
#include <cassert>
#include <thread>
#include <set>

#include "Packing.h"
#include "Rectangle.h"
#include "Space.h"
#include "SpaceManager.h"
#include "PackingBase.h"
#include "PackingLocalSearch.h"
#include "PackingSimulatedAnnealing.h"
#include "PackingGenetic.h"


Packing::Packing(double sheetWidth, int nRect, double lowerBound)
    : m_sheetWidth(sheetWidth), m_nRect(nRect), m_lowerBound(lowerBound)
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

    // 对搜索列表首先进行初始化
    m_bestSearchList = m_rectangles;
    m_bestPacking.searchList = m_bestSearchList;
    m_bestPacking.h = std::numeric_limits<double>::max();
    m_bestPacking.assigns.clear();

    const int nThread = 14;
    std::thread* threads[nThread];

    // 局部搜索
    int nLocal = 4;
    // 模拟退火
    int nSA = 0;
    // 遗传算法
    int nGenetic = nThread - nSA - nLocal;

    // 上一次搜索的随机解集，从第二轮开始使用
    std::vector<PackingList> randomResultsVector;
    // 上一次搜索的最好的解集，从第二轮开始使用
    std::vector<PackingList> bestResultsVector;
    // 遗传算法的初始种群集合
    std::vector<PackingList> geneticInitalPop;

    const int totaltime = 60'000'000;
    const int aRoundRuntime = 14'500'000;
    int runtime = aRoundRuntime;
    int leftTime = totaltime;

    for (int round = 0; round < 4; round++)
    {
        auto startTime = std::chrono::steady_clock::now();

        if (m_bestPacking.h <= m_lowerBound)
        {
            break;
        }
        std::vector<PackingList> threadBestResultsRawVector(nThread);
        std::vector<std::vector<PackingList>> threadRandomResultsRawVector(nThread);

        int threadId = 0;

        for (int i = 0; i < nLocal; i++)
        {
            if (round < 1)
            {
                // 第一次我们使用4个先验的顺序进行局部搜索
                if (i == 0)
                {
                    threads[threadId] = new std::thread([&](PackingList& result, std::vector<PackingList>& out)
                    {
                        std::vector<Rectangle*> initSeachList = m_rectangles;
                        std::sort(initSeachList.begin(), initSeachList.end(),
                            static_cast<bool(*)(const Rectangle::_PRectangle&, const Rectangle::_PRectangle&)>(Rectangle::comparatorPerimeter));
                        PackingList initList;
                        initList.searchList = initSeachList;
                        initList.h = std::numeric_limits<double>::max();
                        PackingLocalSearch(m_sheetWidth, m_rectangles, initList, std::chrono::microseconds(runtime))(result, out);
                    }, std::ref(threadBestResultsRawVector[threadId]), std::ref(threadRandomResultsRawVector[threadId]));
                }
                else if (i == 1)
                {
                    threads[threadId] = new std::thread([&](PackingList& result, std::vector<PackingList>& out)
                    {
                        std::vector<Rectangle*> initSeachList = m_rectangles;
                        std::sort(initSeachList.begin(), initSeachList.end(),
                            static_cast<bool(*)(const Rectangle::_PRectangle&, const Rectangle::_PRectangle&)>(Rectangle::compratorWidth));
                        PackingList initList;
                        initList.searchList = initSeachList;
                        initList.h = std::numeric_limits<double>::max();
                        PackingLocalSearch(m_sheetWidth, m_rectangles, initList, std::chrono::microseconds(runtime))(result, out);
                    }, std::ref(threadBestResultsRawVector[threadId]), std::ref(threadRandomResultsRawVector[threadId]));
                }
                else if (i == 2)
                {
                    threads[threadId] = new std::thread([&](PackingList& result, std::vector<PackingList>& out)
                    {
                        std::vector<Rectangle*> initSeachList = m_rectangles;
                        std::sort(initSeachList.begin(), initSeachList.end(),
                            static_cast<bool(*)(const Rectangle::_PRectangle&, const Rectangle::_PRectangle&)>(Rectangle::comparatorArea));
                        PackingList initList;
                        initList.searchList = initSeachList;
                        initList.h = std::numeric_limits<double>::max();
                        PackingLocalSearch(m_sheetWidth, m_rectangles, initList, std::chrono::microseconds(runtime))(result, out);
                    }, std::ref(threadBestResultsRawVector[threadId]), std::ref(threadRandomResultsRawVector[threadId]));
                }
                else if (i == 3)
                {
                    threads[threadId] = new std::thread([&](PackingList& result, std::vector<PackingList>& out)
                    {
                        std::vector<Rectangle*> initSeachList = m_rectangles;
                        std::sort(initSeachList.begin(), initSeachList.end(),
                            static_cast<bool(*)(const Rectangle::_PRectangle&, const Rectangle::_PRectangle&)>(Rectangle::compratorHeight));
                        PackingList initList;
                        initList.searchList = initSeachList;
                        initList.h = std::numeric_limits<double>::max();
                        PackingLocalSearch(m_sheetWidth, m_rectangles, initList, std::chrono::microseconds(runtime))(result, out);
                    }, std::ref(threadBestResultsRawVector[threadId]), std::ref(threadRandomResultsRawVector[threadId]));
                }
            }
            else
            {
                threads[threadId] = new std::thread([&](int indexOfResult, PackingList& result, std::vector<PackingList>& out)
                {
                    PackingLocalSearch(m_sheetWidth, m_rectangles, randomResultsVector[indexOfResult], std::chrono::microseconds(runtime))(result, out);
                }, i, std::ref(threadBestResultsRawVector[threadId]), std::ref(threadRandomResultsRawVector[threadId]));
            }
            threadId++;
        }


        for (int i = 0; i < nSA; i++)
        {
            threads[threadId] = new std::thread([&](PackingList& result, std::vector<PackingList>& out)
            {
                PackingSimulatedAnnealing(m_sheetWidth, m_rectangles, bestResultsVector[i % bestResultsVector.size()], std::chrono::microseconds(runtime))(result, out);
            }, std::ref(threadBestResultsRawVector[threadId]), std::ref(threadRandomResultsRawVector[threadId]));
            threadId++;
        }

        for (int i = 0; i < nGenetic; i++)
        {
            if (round < 1)
            {
                threads[threadId] = new std::thread([&](PackingList& result, std::vector<PackingList>& out)
                {
                    PackingGenetic(m_sheetWidth, m_rectangles, m_bestPacking, std::chrono::microseconds(runtime))(result, out);
                }, std::ref(threadBestResultsRawVector[threadId]), std::ref(threadRandomResultsRawVector[threadId]));
            }
            else
            {
                threads[threadId] = new std::thread([&](PackingList& result, std::vector<PackingList>& out)
                {
                    PackingGenetic(m_sheetWidth, m_rectangles, geneticInitalPop, std::chrono::microseconds(runtime))(result, out);
                }, std::ref(threadBestResultsRawVector[threadId]), std::ref(threadRandomResultsRawVector[threadId]));
            }
            threadId++;
        }


        // 将所有线程的前k个（k为遗传算法确定的）结果导入同一个集合之中，每个对象的唯一性通过其哈希值确定
        std::set<PackingList, PackingListCmp> threadResultsSet;
        std::set<PackingList, PackingListCmp> threadBestResultsSet;

        // 运算结束从中读取数据，如果某个方法取得了更好的效果，那么下一次给他分配更多的线程计算
        threadId = 0;
        for (int i = 0; i < nLocal; i++)
        {
            threads[threadId]->join();
            threadResultsSet.insert(threadRandomResultsRawVector[threadId].begin(), threadRandomResultsRawVector[threadId].end());
            if (threadBestResultsRawVector[threadId].h < m_bestPacking.h)
            {
                m_bestPacking = threadBestResultsRawVector[threadId];
            }
            threadBestResultsSet.insert(threadBestResultsRawVector[threadId]);
            delete threads[threadId];
            threadId++;
        }

        for (int i = 0; i < nSA; i++)
        {
            threads[threadId]->join();
            threadResultsSet.insert(threadRandomResultsRawVector[threadId].begin(), threadRandomResultsRawVector[threadId].end());
            if (threadBestResultsRawVector[threadId].h < m_bestPacking.h)
            {
                m_bestPacking = threadBestResultsRawVector[threadId];
            }
            threadBestResultsSet.insert(threadBestResultsRawVector[threadId]);
            delete threads[threadId];
            threadId++;
        }

        for (int i = 0; i < nGenetic; i++)
        {
            threads[threadId]->join();
            threadResultsSet.insert(threadRandomResultsRawVector[threadId].begin(), threadRandomResultsRawVector[threadId].end());
            if (threadBestResultsRawVector[threadId].h < m_bestPacking.h)
            {
                m_bestPacking = threadBestResultsRawVector[threadId];
            }
            threadBestResultsSet.insert(threadBestResultsRawVector[threadId]);
            delete threads[threadId];
            threadId++;
        }



        if (std::chrono::steady_clock::now() - m_beginTime >= std::chrono::seconds(60))
        {
            break;
        }

        {
            auto threadResultsSetSize = threadResultsSet.size();
            const PackingList** ptrThreadResults = new const PackingList*[threadResultsSetSize];

            {
                int i = 0;
                for (auto iter = threadResultsSet.begin(); iter != threadResultsSet.end(); iter++)
                {
                    ptrThreadResults[i] = &(*iter);
                    i++;
                }
            }

            // 这里对之前的结果进行排序，因为这两个算法都基于相对较好的结果进行
            std::sort(ptrThreadResults, ptrThreadResults + threadResultsSetSize,
                [](const PackingList* lhs, const PackingList* rhs)
            {
                return lhs->h < rhs->h;
            }
            );

            // 将所有线程随机导出的序列留给下一轮搜索
            randomResultsVector.clear();
            randomResultsVector.resize(threadResultsSetSize);

            for (int i = 0; i < threadResultsSetSize; i++)
            {
                randomResultsVector[i] = *ptrThreadResults[i];
            }

            delete[] ptrThreadResults;
        }

        {

            threadBestResultsSet.insert(m_bestPacking);
            auto threadBestResultsSetSize = threadBestResultsSet.size();

            const PackingList** ptrThreadBestResults = new const PackingList*[threadBestResultsSetSize];

            {
                int i = 0;
                for (auto iter = threadBestResultsSet.begin(); iter != threadBestResultsSet.end(); iter++)
                {
                    ptrThreadBestResults[i] = &(*iter);
                    i++;
                }
            }


            // 这里对之前的结果进行排序，因为这两个算法都基于相对较好的结果进行
            std::sort(ptrThreadBestResults, ptrThreadBestResults + threadBestResultsSetSize,
                [](const PackingList* lhs, const PackingList* rhs)
            {
                return lhs->h < rhs->h;
            }
            );

            bestResultsVector.clear();
            bestResultsVector.resize(threadBestResultsSetSize);

            // 将线程最好的结果留给模拟退火
            for (int i = 0; i < threadBestResultsSetSize; i++)
            {
                bestResultsVector[i] = *ptrThreadBestResults[i];
            }
            delete[] ptrThreadBestResults;
        }

        geneticInitalPop.clear();

        int timesInsertBest = randomResultsVector.size() * 0.5 / bestResultsVector.size();
        int nGeneticInitalPop = timesInsertBest * bestResultsVector.size() + randomResultsVector.size();
        geneticInitalPop.resize(nGeneticInitalPop);

        {
            int index = 0;
            for (int i = 0; i < bestResultsVector.size(); i++)
            {
                for (int j = 0; j < timesInsertBest; j++)
                {
                    geneticInitalPop[index] = bestResultsVector[i];
                    index++;
                }
            }
            for (int i = 0; i < randomResultsVector.size(); i++)
            {
                geneticInitalPop[index] = randomResultsVector[i];
                index++;
            }
        }


        // 更新下一次各种方法的线程数。
        // 局部搜索并非是随机算法，它负责深度挖掘，固定4
        nLocal = std::min(4, (int)randomResultsVector.size());
        nSA = std::min(nThread - nLocal, 2);
        nGenetic = nThread - nLocal - nSA;


        auto actualRuntime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - startTime).count();
        leftTime -= actualRuntime;
        runtime = std::min(leftTime, aRoundRuntime);

        int nextLeftTime = leftTime - actualRuntime;

        if (runtime <= 0 || nextLeftTime <= -0'050'000)
        {
            break;
        }

    }

    return m_bestPacking;
}

std::vector<Rectangle*>& Packing::getRectangles()
{
    return m_rectangles;
}
