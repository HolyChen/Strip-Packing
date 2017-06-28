#include "stdafx.h"
#include "PackingSimulatedAnnealing.h"

#include <algorithm>


PackingSimulatedAnnealing::PackingSimulatedAnnealing(
    double sheetWidth, const std::vector<Rectangle*>& rectangles, const PackingList& bestPacking)
    : PackingBase(sheetWidth, rectangles, bestPacking)
{
}


PackingSimulatedAnnealing::~PackingSimulatedAnnealing()
{
}

PackingList PackingSimulatedAnnealing::operator()()
{
    // 初始温度设置为所有的矩形的面积和除以材料板的宽度，即压紧时的高度
    double temperature = 0.0;
    {
        auto totalArea = 0.0;
        for (auto rect : m_rectangles)
        {
            totalArea += rect->getArea();
        }
        temperature = totalArea / m_sheetWidth;
    }
    // 记录h没有发生改变的次数，当h连续2次没有发生改变时，终止搜索
    int notChange = 0;
    // 每个温度下搜索的链长，为矩形个数的平方除以2，即一个搜索连通过swap两个元素获得邻域的总数
    int linkLength = m_nRect * m_nRect / 2;
    // 初始化随机数引擎
    std::default_random_engine rdeg(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    // 搜索链承接LS的后的best链
    auto curSearchList(m_bestSearchList);
    while (std::chrono::steady_clock::now() - m_beginTime < MAX_RUNTIME)
    {
        // 记录在这次搜索中是否找到更好的解
        bool better = false;
        for (int i = 0; i < linkLength; i++)
        {
            // 获得两个不相等的随机数
            std::uniform_int_distribution<int> distLeft(0, m_nRect - 2);
            int left = distLeft(rdeg);
            std::uniform_int_distribution<int> distRight(left + 1, m_nRect - 1);
            int right = distRight(rdeg);
            auto newSearchList(curSearchList);
            // 交换第i、j个元素位置
            {
                auto temp = newSearchList.at(left);
                newSearchList.at(left) = newSearchList.at(right);
                newSearchList.at(right) = temp;
            }
            auto currentPacking = heuristicPacking(newSearchList);
            // 如果这个新的序列的搜索结果好于原始序列，则替换原始序列为新序列，并更新全局最优序列
            if (currentPacking.h < m_bestPacking.h)
            {
                better = true;
                m_bestPacking = currentPacking;
                m_bestSearchList = newSearchList;
                curSearchList = newSearchList;
            }
            std::bernoulli_distribution isAccept(std::exp((m_bestPacking.h - currentPacking.h) / temperature));
            // 如果依照概率可以接受，那么替换当前队列
            if (isAccept(rdeg))
            {
                curSearchList = newSearchList;
            }
        }

        if (better)
        {
            notChange = 0;
        }
        else
        {
            notChange += 1;
        }
        // 衰退方式为每次变更为上次一的0.9
        temperature *= 0.9;
        // 多启动
        std::uniform_int_distribution<int> uIntD(0, 2);
        int type = uIntD(rdeg);
        switch (type)
        {
        case 0:         // 保持原样
            break;
        case 1:         // 转化为按边长排序的序列
            std::sort(curSearchList.begin(), curSearchList.end(),
                static_cast<bool(*)(const Rectangle::_PRectangle&, const Rectangle::_PRectangle&)>(Rectangle::comparatorPerimeter));
            break;
        case 2:         // 转化为按宽度排序的序列
            std::sort(curSearchList.begin(), curSearchList.end(),
                static_cast<bool(*)(const Rectangle::_PRectangle&, const Rectangle::_PRectangle&)>(Rectangle::compratorWidth));
            break;
        default:
            break;
        }
    }
    return m_bestPacking;
}
