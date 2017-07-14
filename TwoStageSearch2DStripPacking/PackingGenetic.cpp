#include "stdafx.h"
#include "PackingGenetic.h"

#include <algorithm>
#include <memory>
#include <set>

// �������
const double PackingGenetic::PROPORTION_CROSS = 0.3;
// �������
const double PackingGenetic::PROPORTION_MUTATE = 0.1;

PackingGenetic::PackingGenetic(double sheetWidth, const std::vector<Rectangle*>& rectangles, const PackingList& bestPacking, std::chrono::microseconds runtime)
    : PackingBase(sheetWidth, rectangles, bestPacking, runtime)
{
    // ��ʼ����Ⱥ
    // ����Ⱥ��������Ϊ��ռ�õ��ڴ治����1GB�������������ھ�������4��
    mNumOfHalfPop = std::max(std::min((int)(500'000'000 / ((sizeof(Rectangle*) + sizeof(Assign)) * m_nRect + 64)), 2 * m_nRect), 1);
    mNumOfPop = mNumOfHalfPop * 2;

    // ��ʼ����Ⱥ�ĸ���
    // ��Ⱥ��1/4�ĸ��壨������һ��������Ϊ֮ǰ�Ѿ��ҵ�������������У�ʣ���3/4��ʹ�öԸ�����������е�������������г�ʼ��
    int numOfQuarterPop = std::max(1, mNumOfPop / 4);
    for (int i = 0; i < numOfQuarterPop && std::chrono::steady_clock::now() - m_beginTime < m_maxRunTime; i++)
    {
        mPopulation.push_back(m_bestPacking);
    }
    for (int i = numOfQuarterPop; i < mNumOfPop && std::chrono::steady_clock::now() - m_beginTime < m_maxRunTime; i++)
    {
        mPopulation.push_back(searchListMutate(m_bestPacking, std::chrono::high_resolution_clock().now().time_since_epoch().count()));
    }
}

PackingGenetic::PackingGenetic(double sheetWidth, const std::vector<Rectangle*>& rectangles, const std::vector<PackingList>& bestPackings, std::chrono::microseconds runtime)
    : PackingBase(sheetWidth, rectangles, bestPackings.front(), runtime)
{
    // ��ʼ����Ⱥ
    // ����Ⱥ��������Ϊ��ռ�õ��ڴ治����1GB�������������ھ�������4��
    mNumOfHalfPop = std::max(std::min((int)(500'000'000 / ((sizeof(Rectangle*) + sizeof(Assign)) * m_nRect + 64)), 2 * m_nRect), 1);
    mNumOfPop = mNumOfHalfPop * 2;

    // ��ʼ����Ⱥ�ĸ���
    // ���ȼ���1�����е�bestPackings�ĸ��壬ʣ�ಿ��������Ϊ��0.3�ĸ���������Ⱥ�е�һ�������������
    for (int i = 0; i < 1; i++)
    {
        mPopulation.insert(mPopulation.begin(), bestPackings.cbegin(), bestPackings.cbegin() + std::min(mNumOfPop, (int)bestPackings.size()));
    }

    int nLeft = mNumOfPop - std::min(mNumOfPop, (int)bestPackings.size());

    std::default_random_engine eg(std::chrono::high_resolution_clock().now().time_since_epoch().count());
    std::bernoulli_distribution dis(0.3);

    for (int i = 0; i < nLeft && std::chrono::steady_clock::now() - m_beginTime < m_maxRunTime; i++)
    {
        if (dis(eg))
        {
            mPopulation.push_back(searchListMutate(bestPackings[0], std::chrono::high_resolution_clock().now().time_since_epoch().count()));
        }
        else
        {
            mPopulation.push_back(bestPackings[0]);
        }
    }
}


PackingGenetic::~PackingGenetic()
{
}

void PackingGenetic::operator()(PackingList &result, std::vector<PackingList> &out)
{
    // ������Ⱥ����õĸ���
    calcBestOne();
    while (std::chrono::steady_clock::now() - m_beginTime < m_maxRunTime)
    {
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        select();
        if (std::chrono::steady_clock::now() - m_beginTime >= m_maxRunTime)
        {
            calcBestOne();
            break;
        }
        cross();
        if (std::chrono::steady_clock::now() - m_beginTime >= m_maxRunTime)
        {
            calcBestOne();
            break;
        }
        mutate();
        if (std::chrono::steady_clock::now() - m_beginTime >= m_maxRunTime)
        {
            calcBestOne();
            break;
        }
        calcBestOne();
    }

    // ���ｫ�����(mPopulation.size()/4)���������е���������һ������ʹ��
    int nToResult = std::max(1, int(mPopulation.size() / 4));
    
    result = m_bestPacking;

    out.insert(out.begin(), mPopulation.begin(), mPopulation.begin() + nToResult);
}

void PackingGenetic::select()
{
    // ѡ��һЩ���壬���н������
    // ������׼��
    // ·������L�뵱ǰ��Ⱥ����̵�·��Lmin��������Ĳ�ֵ+1�������·������Lmin��
    // ��  Lmin / (L - Lmin + 1)
    // ��ѡȡ�ĸ�����ò�����׼������
    mParents.clear();

    // 1. �����������۽�Ȩֵ
    double pattern = mBestPopulationInCurrentGeneration.h;
    double accmulate = 0.0;
    std::vector<double> accumulateWeights;
    for (auto packingList : mPopulation)
    {
        if (std::chrono::steady_clock::now() - m_beginTime >= m_maxRunTime)
        {
            return;
        }
        accmulate += (pattern / (packingList.h - pattern + 1));
        accumulateWeights.push_back(accmulate);
    }


    std::default_random_engine engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> distribution(0.0, accmulate);

    // 2. ����mNumOfPop / 2��ѡ��ÿ���������ǵ�Ȩ��ѡ��һ�Ը���
    for (int i = 0; i < mNumOfHalfPop && std::chrono::steady_clock::now() - m_beginTime < m_maxRunTime; i++)
    {
        int father = searchForBiggerThan<double>(accumulateWeights, distribution(engine));
        int mother = searchForBiggerThan<double>(accumulateWeights, distribution(engine));
        mParents.push_back(std::pair<int, int>(father, mother));
    }
}

void PackingGenetic::cross()
{
    // ���˽ڵ��ˣ��ֵ��˹���Ů����ļ��ڣ������ɹ���ֻ��PROPORTION_CROSS��
    // �������ʧ���ˣ�ֱ�ӱ���Ӵ���Ϊ����Ӧ���Ҷ������ߣ�ÿ�Թ���Ů�������������ӣ�
    // һ�˸���������ȫ���˹��ھ�

    mNextPopulation.clear();

    std::default_random_engine engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    for (auto parent : mParents)
    {
        if (std::chrono::steady_clock::now() - m_beginTime >= m_maxRunTime)
        {
            calcBestOne();
            return;
        }
        const auto& p1 = mPopulation.at(parent.first), p2 = mPopulation.at(parent.second);
        // ����ɣ��ɹ��������ӣ�ʧ���˱����
        if (distribution(engine) <= PROPORTION_CROSS)
        {
            mNextPopulation.push_back(searchListCross(p1, p2));
            mNextPopulation.push_back(searchListCross(p2, p1));
        }
        else
        {
            mNextPopulation.push_back(p1);
            mNextPopulation.push_back(p2);
        }
    }

}

void PackingGenetic::mutate()
{
    // ���䣿��ѧҩƷ������ֻ�ǳ��ߵ�Ӳ�㣬��֮������Ҫ�����ˡ�
    // ��������ʳƷ��ȫ�ĸ��ƣ��������������ͣ�ֻ��PROPORTION_MUTATE��ô��

    std::vector<PackingList> newGerneration;

    std::default_random_engine engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> distribution(0.0, 1.0);


    for (auto child : mNextPopulation)
    {
        if (std::chrono::steady_clock::now() - m_beginTime >= m_maxRunTime)
        {
            return;
        }
        if (distribution(engine) < PROPORTION_MUTATE)
        {
            newGerneration.push_back(searchListMutate(child, std::chrono::high_resolution_clock().now().time_since_epoch().count()));
        }
        else
        {
            newGerneration.push_back(child);
        }
    }

    mPopulation = std::move(newGerneration);
}

PackingList PackingGenetic::searchListCross(const PackingList& lho, const PackingList& rho)
{
    // �����㷨���£�
    // ѡȡһ������[i, j]������һ���м�Ⱦɫ��Z
    // ʹZ��ͬ��B��ȥ��A������[i, j]֮������нڵ��Ⱦɫ�壬����Ϊn - j + i - 1
    // ���²�����Ⱦɫ��C��[0, i]��Z�е�[0, i]���Ӧ��[j, n - 1]��ʣ�ಿ��[i + 1, n - j + i - 2]���Ӧ,
    // Ȼ��A��[i, j]���ֲ����C��[i, j]�Ĳ���

    int numNodes = m_nRect;

    int left = std::rand() % (numNodes - 1);
    int right = std::rand() % (numNodes - left - 1) + left + 1;

    std::unique_ptr<bool[]> isRemove(new bool[numNodes]);
    std::memset(isRemove.get(), 0, sizeof(bool) * numNodes);

    SearchList child(m_nRect);

    for (int i = left; i <= right; i++)
    {
        isRemove[lho.searchList[i]->id] = true;
    }

    // [0, i)
    int posB = 0, posC = 0;
    for (; posB < numNodes && posC < left; posB++)
    {
        if (!isRemove[rho.searchList[posB]->id])
        {
            child[posC++] = rho.searchList[posB];
        }
    }

    // [i, j]
    for (; posC <= right; posC++)
    {
        child[posC] = lho.searchList[posC];
    }

    // [j, end]
    for (; posB < numNodes && posC < numNodes; posB++)
    {
        if (!isRemove[rho.searchList[posB]->id])
        {
            child[posC++] = rho.searchList[posB];
        }
    }

    return heuristicPacking(child);
}

PackingList PackingGenetic::searchListMutate(const PackingList srcList, long long seed)
{
    SearchList newSearchList(srcList.searchList.size());

    std::srand(seed);

    // ���ѡȡһ���������У������ƶ�����ǰ��
    int randl = std::rand() % m_nRect;
    int rand2 = std::rand() % m_nRect;

    int left = std::min(randl, rand2);
    int right = std::max(randl, rand2);

    int length = right - left;

    for (int i = 0; i < length; i++)
    {
        newSearchList.at(i) = srcList.searchList.at(left + i);
    }

    // ��ʣ��δ���ƵĲ��ֲ������µ����������
    int index = length;
    for (int i = 0; i < left; i++)
    {
        newSearchList.at(index++) = srcList.searchList.at(i);
    }
    for (int i = right; i < m_nRect; i++)
    {
        newSearchList.at(index++) = srcList.searchList.at(i);
    }

    //// ���ѡȡһ���������У�������ת
    //int randl = std::rand() % m_nRect;
    //int rand2 = std::rand() % m_nRect;

    //int left = std::min(randl, rand2);
    //int right = std::max(randl, rand2);

    //int length = right - left + 1;

    //newSearchList = srcList.searchList;

    //for (int i = 0; i < length; i++)
    //{
    //    newSearchList.at(left + i) = srcList.searchList.at(right - i);
    //}

    // ������µ��б��Packing����
    return heuristicPacking(newSearchList);
}

void PackingGenetic::calcBestOne()
{
    // ������һ������õĸ���չʾ����
    double minHeight = std::numeric_limits<double>::max();
    PackingList &bestList = mPopulation.at(0);
    for (auto pakcingList : mPopulation)
    {
        if (pakcingList.h < minHeight)
        {
            minHeight = pakcingList.h;
            bestList = pakcingList;
        }
    }
    mBestPopulationInCurrentGeneration = bestList;
    if (mBestPopulationInCurrentGeneration.h < m_bestPacking.h)
    {
        m_bestPacking = mBestPopulationInCurrentGeneration;
    }
}


// ���ֲ���ǡ�ñ�key���Ԫ��
template<typename _Type>
int PackingGenetic::searchForBiggerThan(const std::vector<_Type>& container, _Type key)
{
    int size = container.size();
    int left = 0, right = size - 1;
    int mid;
    int res = -1;
    while (left <= right)
    {
        mid = (left + right) / 2;
        _Type cur = container.at(mid);
        if (cur >= key)
        {
            right = mid - 1;
            res = mid;
        }
        else
        {
            left = mid + 1;
        }
    }
    return res;
}