#include "stdafx.h"
#include "PackingGenetic.h"

#include <algorithm>
#include <memory>
#include <set>

// 交配概率
const double PackingGenetic::PROPORTION_CROSS = 0.3;
// 变异概率
const double PackingGenetic::PROPORTION_MUTATE = 0.1;

PackingGenetic::PackingGenetic(double sheetWidth, const std::vector<Rectangle*>& rectangles, const PackingList& bestPacking, std::chrono::microseconds runtime)
    : PackingBase(sheetWidth, rectangles, bestPacking, runtime)
{
    // 初始化种群
    // 将种群数量设置为总占用的内存不超过1GB，且总数不大于矩形数量4倍
    mNumOfHalfPop = std::max(std::min((int)(500'000'000 / ((sizeof(Rectangle*) + sizeof(Assign)) * m_nRect + 64)), 2 * m_nRect), 1);
    mNumOfPop = mNumOfHalfPop * 2;

    // 初始化种群的个体
    // 种群的1/4的个体（不少于一个）设置为之前已经找到的最佳搜索序列，剩余的3/4则使用对改最佳搜索序列的随机变异结果进行初始化
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
    // 初始化种群
    // 将种群数量设置为总占用的内存不超过1GB，且总数不大于矩形数量4倍
    mNumOfHalfPop = std::max(std::min((int)(500'000'000 / ((sizeof(Rectangle*) + sizeof(Assign)) * m_nRect + 64)), 2 * m_nRect), 1);
    mNumOfPop = mNumOfHalfPop * 2;

    // 初始化种群的个体
    // 首先加入1次所有的bestPackings的个体，剩余部分则设置为以0.3的概率生成种群中第一个序列随机变异
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
    // 更新种群中最好的个体
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

    // 这里将随机的(mPopulation.size()/4)个搜索序列导出，供下一次搜索使用
    int nToResult = std::max(1, int(mPopulation.size() / 4));
    
    result = m_bestPacking;

    out.insert(out.begin(), mPopulation.begin(), mPopulation.begin() + nToResult);
}

void PackingGenetic::select()
{
    // 选出一些个体，进行交叉操作
    // 测量标准：
    // 路径长度L与当前种群中最短的路径Lmin相减，所的差值+1，被最短路径长度Lmin除
    // 即  Lmin / (L - Lmin + 1)
    // 被选取的概率与该测量标准成正比
    mParents.clear();

    // 1. 计算各个解的累进权值
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

    // 2. 进行mNumOfPop / 2轮选择，每次依照他们的权重选择一对个体
    for (int i = 0; i < mNumOfHalfPop && std::chrono::steady_clock::now() - m_beginTime < m_maxRunTime; i++)
    {
        int father = searchForBiggerThan<double>(accumulateWeights, distribution(engine));
        int mother = searchForBiggerThan<double>(accumulateWeights, distribution(engine));
        mParents.push_back(std::pair<int, int>(father, mother));
    }
}

void PackingGenetic::cross()
{
    // 情人节到了，又到了狗男女交配的季节，不过成功率只有PROPORTION_CROSS，
    // 如果交配失败了，直接变成子代。为了响应国家二孩政策，每对狗男女必须生两个孩子，
    // 一人该生不生，全村人工授精

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
        // 交配吧！成功了生儿子，失败了变儿子
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
    // 辐射？化学药品？或者只是长者的硬点，总之孩子们要变异了。
    // 不过随着食品安全的改善，变异率有所降低，只有PROPORTION_MUTATE这么大

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
    // 交叉算法如下：
    // 选取一个区间[i, j]，构造一个中间染色体Z
    // 使Z等同于B中去掉A中区间[i, j]之间的所有节点的染色体，长度为n - j + i - 1
    // 让新产生的染色体C的[0, i]与Z中的[0, i]相对应，[j, n - 1]与剩余部分[i + 1, n - j + i - 2]相对应,
    // 然后将A的[i, j]部分插入的C的[i, j]的部分

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

    // 随机选取一段搜索序列，把它移动到最前方
    int randl = std::rand() % m_nRect;
    int rand2 = std::rand() % m_nRect;

    int left = std::min(randl, rand2);
    int right = std::max(randl, rand2);

    int length = right - left;

    for (int i = 0; i < length; i++)
    {
        newSearchList.at(i) = srcList.searchList.at(left + i);
    }

    // 把剩余未复制的部分补充在新的搜索链表后方
    int index = length;
    for (int i = 0; i < left; i++)
    {
        newSearchList.at(index++) = srcList.searchList.at(i);
    }
    for (int i = right; i < m_nRect; i++)
    {
        newSearchList.at(index++) = srcList.searchList.at(i);
    }

    //// 随机选取一段搜索序列，把它翻转
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

    // 对这个新的列表解Packing问题
    return heuristicPacking(newSearchList);
}

void PackingGenetic::calcBestOne()
{
    // 将最终一代的最好的个体展示出来
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


// 二分查找恰好比key大的元素
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