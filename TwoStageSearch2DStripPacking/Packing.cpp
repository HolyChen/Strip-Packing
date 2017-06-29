#include "stdafx.h"
#include <memory>
#include <algorithm>
#include <numeric>
#include <random>
#include <chrono>
#include <iostream>
#include <cassert>
#include <thread>

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

    m_bestSearchList = m_rectangles;
    m_bestPacking.searchList = m_bestSearchList;
    m_bestPacking.h = std::numeric_limits<double>::max();

    const int nThread = 14;
    PackingList threadResult[nThread];
    std::thread* threads[nThread];

    // �ֲ�����
    int nLocal = 1;
    // ģ���˻�
    int nSA = nThread * 0.3;
    // �Ŵ��㷨
    int nGenetic = nThread - nLocal - nSA;

    for (int i = 0; i < 6; i++)
    {

        int weightSA = 1;
        int weightGenetic = 1;

        int threadId = 0;

        for (int i = 0; i < nLocal; i++)
        {
            threads[threadId] = new std::thread([&](PackingList& result)
            {
                PackingLocalSearch(m_sheetWidth, m_rectangles, m_bestPacking)(result, std::chrono::seconds(10));
            }, std::ref(threadResult[threadId]));
            threadId++;
        }

        for (int i = 0; i < nSA; i++)
        {
            threads[threadId] = new std::thread([&](PackingList& result)
            {
                PackingSimulatedAnnealing(m_sheetWidth, m_rectangles, m_bestPacking)(result, std::chrono::seconds(10));
            }, std::ref(threadResult[threadId]));
            threadId++;
        }

        for (int i = 0; i < nGenetic; i++)
        {
            threads[threadId] = new std::thread([&](PackingList& result)
            {
                PackingGenetic(m_sheetWidth, m_rectangles, m_bestPacking)(result, std::chrono::seconds(10));
            }, std::ref(threadResult[threadId]));
            threadId++;
        }


        // ����������ж�ȡ���ݣ����ĳ������ȡ���˸��õ�Ч������ô��һ�θ������������̼߳���
        double oldH = m_bestPacking.h;
        threadId = 0;
        for (int i = 0; i < nLocal; i++)
        {
            threads[threadId]->join();
            if (threadResult[threadId].h < m_bestPacking.h)
            {
                m_bestPacking = threadResult[threadId];
            }
            delete threads[threadId];
            threadId++;
        }

        for (int i = 0; i < nSA; i++)
        {
            threads[threadId]->join();
            if (threadResult[threadId].h < m_bestPacking.h)
            {
                m_bestPacking = threadResult[threadId];
                weightSA++;
            }
            if (threadResult[threadId].h < oldH)
            {
                weightSA++;
            }
            delete threads[threadId];
            threadId++;
        }

        for (int i = 0; i < nGenetic; i++)
        {
            threads[threadId]->join();
            if (threadResult[threadId].h < m_bestPacking.h)
            {
                m_bestPacking = threadResult[threadId];
            }
            if (threadResult[threadId].h < oldH)
            {
                weightGenetic++;
            }
            delete threads[threadId];
            threadId++;
        }

        // ������һ�θ��ַ������߳�����ע�⣬����ÿ���̵߳�Ȩ��������1��Ҳ���ǲ�ȡ��������˹ƽ��
        // �ֲ���������������㷨������ʼ��Ϊ1��
        nLocal = 1;
        nSA = std::max(1, (int)((double)(weightSA) / (double)(weightSA + weightGenetic) * (nThread - nLocal)));
        nGenetic = nThread - nLocal - nSA;
    }


	return m_bestPacking;
}
