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

    // 对搜索列表首先进行初始化
    m_bestSearchList = m_rectangles;
    m_bestPacking.searchList = m_bestSearchList;
    m_bestPacking.h = std::numeric_limits<double>::max();


    const int nThread = 6;
    std::thread* threads[nThread];

    // 局部搜索
    int nLocal = 4;
    // 模拟退火
    // int nSA = nThread * 0.3;
    int nSA = 0;
    // 遗传算法
    int nGenetic = 0;

    // 上一次搜索最好的解集，从第二轮开始使用
    std::vector<PackingList> randomNResult;

    const int runtime = 9;

    for (int round = 0; round < 6; round++)
    {

        std::vector<PackingList> threadSingleResult(nThread);
        std::vector<std::vector<PackingList>> threadMultiResult(nThread);

        int weightSA = 1;
        int weightGenetic = 1;

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
                        PackingLocalSearch(m_sheetWidth, m_rectangles, initList, std::chrono::seconds(runtime))(result, out);
                    }, std::ref(threadSingleResult[threadId]), std::ref(threadMultiResult[threadId]));
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
                        PackingLocalSearch(m_sheetWidth, m_rectangles, initList, std::chrono::seconds(runtime))(result, out);
                    }, std::ref(threadSingleResult[threadId]), std::ref(threadMultiResult[threadId]));
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
                        PackingLocalSearch(m_sheetWidth, m_rectangles, initList, std::chrono::seconds(runtime))(result, out);
                    }, std::ref(threadSingleResult[threadId]), std::ref(threadMultiResult[threadId]));
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
                        PackingLocalSearch(m_sheetWidth, m_rectangles, initList, std::chrono::seconds(runtime))(result, out);
                    }, std::ref(threadSingleResult[threadId]), std::ref(threadMultiResult[threadId]));
                }
            }
            else
            {
                threads[threadId] = new std::thread([&](int indexOfResult, PackingList& result, std::vector<PackingList>& out)
                {
                    PackingLocalSearch(m_sheetWidth, m_rectangles, randomNResult[indexOfResult], std::chrono::seconds(runtime))(result, out);
                }, i, std::ref(threadSingleResult[threadId]), std::ref(threadMultiResult[threadId]));
            }
            threadId++;
        }


        for (int i = 0; i < nSA; i++)
        {
            threads[threadId] = new std::thread([&](PackingList& result, std::vector<PackingList>& out)
            {
                PackingSimulatedAnnealing(m_sheetWidth, m_rectangles, m_bestPacking, std::chrono::seconds(runtime))(result, out);
            }, std::ref(threadSingleResult[threadId]), std::ref(threadMultiResult[threadId]));
            threadId++;
        }

        for (int i = 0; i < nGenetic; i++)
        {
            if (round < 1)
            {
                threads[threadId] = new std::thread([&](PackingList& result, std::vector<PackingList>& out)
                {
                    PackingGenetic(m_sheetWidth, m_rectangles, m_bestPacking, std::chrono::seconds(runtime))(result, out);
                }, std::ref(threadSingleResult[threadId]), std::ref(threadMultiResult[threadId]));
            }
            else
            {
                threads[threadId] = new std::thread([&](PackingList& result, std::vector<PackingList>& out)
                {
                    PackingGenetic(m_sheetWidth, m_rectangles, randomNResult, std::chrono::seconds(runtime))(result, out);
                }, std::ref(threadSingleResult[threadId]), std::ref(threadMultiResult[threadId]));
            }
            threadId++;
        }


        // 将所有线程的前k个（k为遗传算法确定的）结果导入同一个集合之中，每个对象的唯一性通过其哈希值确定
        std::set<PackingList, PackingListCmp> topResults;

        // 运算结束从中读取数据，如果某个方法取得了更好的效果，那么下一次给他分配更多的线程计算
        double oldH = m_bestPacking.h;
        threadId = 0;
        for (int i = 0; i < nLocal; i++)
        {
            threads[threadId]->join();
            topResults.insert(threadMultiResult[threadId].begin(), threadMultiResult[threadId].end());
            if (threadSingleResult[threadId].h < m_bestPacking.h)
            {
                m_bestPacking = threadSingleResult[threadId];
            }
            topResults.insert(threadSingleResult[threadId]);
            delete threads[threadId];
            threadId++;
        }

        for (int i = 0; i < nSA; i++)
        {
            threads[threadId]->join();
            topResults.insert(threadMultiResult[threadId].begin(), threadMultiResult[threadId].end());
            if (threadSingleResult[threadId].h < m_bestPacking.h)
            {
                m_bestPacking = threadSingleResult[threadId];
                weightSA++;
            }
            if (threadSingleResult[threadId].h < oldH)
            {
                weightSA++;
            }
            topResults.insert(threadSingleResult[threadId]);
            delete threads[threadId];
            threadId++;
        }

        for (int i = 0; i < nGenetic; i++)
        {
            threads[threadId]->join();
            topResults.insert(threadMultiResult[threadId].begin(), threadMultiResult[threadId].end());
            auto & bestOne = threadSingleResult[threadId];
            if (bestOne.h < m_bestPacking.h)
            {
                m_bestPacking = bestOne;
            }
            if (bestOne.h < oldH)
            {
                weightGenetic++;
            }
            delete threads[threadId];
            threadId++;
        }
        // 将所有线程随机导出的序列留给下一轮搜索
        randomNResult.clear();
        randomNResult.insert(randomNResult.begin(), topResults.begin(), topResults.end());

        randomNResult.push_back(m_bestPacking);
        // 这里对之前的结果进行排序，因为这两个算法都基于相对较好的结果进行
        std::sort(randomNResult.begin(), randomNResult.end(),
            [](const PackingList& lhs, const PackingList& rhs)
        {
            return lhs.h < rhs.h;
        }
        );

        // 更新下一次各种方法的线程数。注意，这里每个线程的权重至少有1，也就是采取了拉普拉斯平滑
        // 局部搜索并非是随机算法，它负责深度挖掘，固定4
        nLocal = std::min(4 , (int)randomNResult.size());
        nSA = std::max(1, (int)((double)(weightSA) / (double)(weightSA + weightGenetic) * (nThread - nLocal)));
        nGenetic = nThread - nLocal - nSA;

    }


	return m_bestPacking;
}

std::vector<Rectangle*>& Packing::getRectangles()
{
    return m_rectangles;
}
