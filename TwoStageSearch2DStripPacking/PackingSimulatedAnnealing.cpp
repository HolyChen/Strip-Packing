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
    // ��ʼ�¶�����Ϊ���еľ��ε�����ͳ��Բ��ϰ�Ŀ�ȣ���ѹ��ʱ�ĸ߶�
    double temperature = 0.0;
    {
        auto totalArea = 0.0;
        for (auto rect : m_rectangles)
        {
            totalArea += rect->getArea();
        }
        temperature = totalArea / m_sheetWidth;
    }
    // ��¼hû�з����ı�Ĵ�������h����2��û�з����ı�ʱ����ֹ����
    int notChange = 0;
    // ÿ���¶���������������Ϊ���θ�����ƽ������2����һ��������ͨ��swap����Ԫ�ػ�����������
    int linkLength = m_nRect * m_nRect / 2;
    // ��ʼ�����������
    std::default_random_engine rdeg(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    // �������н�LS�ĺ��best��
    auto curSearchList(m_bestSearchList);
    while (std::chrono::steady_clock::now() - m_beginTime < MAX_RUNTIME)
    {
        // ��¼������������Ƿ��ҵ����õĽ�
        bool better = false;
        for (int i = 0; i < linkLength; i++)
        {
            // �����������ȵ������
            std::uniform_int_distribution<int> distLeft(0, m_nRect - 2);
            int left = distLeft(rdeg);
            std::uniform_int_distribution<int> distRight(left + 1, m_nRect - 1);
            int right = distRight(rdeg);
            auto newSearchList(curSearchList);
            // ������i��j��Ԫ��λ��
            {
                auto temp = newSearchList.at(left);
                newSearchList.at(left) = newSearchList.at(right);
                newSearchList.at(right) = temp;
            }
            auto currentPacking = heuristicPacking(newSearchList);
            // �������µ����е������������ԭʼ���У����滻ԭʼ����Ϊ�����У�������ȫ����������
            if (currentPacking.h < m_bestPacking.h)
            {
                better = true;
                m_bestPacking = currentPacking;
                m_bestSearchList = newSearchList;
                curSearchList = newSearchList;
            }
            std::bernoulli_distribution isAccept(std::exp((m_bestPacking.h - currentPacking.h) / temperature));
            // ������ո��ʿ��Խ��ܣ���ô�滻��ǰ����
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
        // ˥�˷�ʽΪÿ�α��Ϊ�ϴ�һ��0.9
        temperature *= 0.9;
        // ������
        std::uniform_int_distribution<int> uIntD(0, 2);
        int type = uIntD(rdeg);
        switch (type)
        {
        case 0:         // ����ԭ��
            break;
        case 1:         // ת��Ϊ���߳����������
            std::sort(curSearchList.begin(), curSearchList.end(),
                static_cast<bool(*)(const Rectangle::_PRectangle&, const Rectangle::_PRectangle&)>(Rectangle::comparatorPerimeter));
            break;
        case 2:         // ת��Ϊ��������������
            std::sort(curSearchList.begin(), curSearchList.end(),
                static_cast<bool(*)(const Rectangle::_PRectangle&, const Rectangle::_PRectangle&)>(Rectangle::compratorWidth));
            break;
        default:
            break;
        }
    }
    return m_bestPacking;
}
