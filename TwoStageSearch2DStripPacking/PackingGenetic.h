#pragma once
#include "PackingBase.h"

#include <chrono>

class PackingGenetic :
    public PackingBase
{
public:
    PackingGenetic(double sheetWidth, const std::vector<Rectangle*>& rectangles, const PackingList& bestPacking, std::chrono::microseconds runtime);
    PackingGenetic(double sheetWidth, const std::vector<Rectangle*>& rectangles, const std::vector<PackingList>& bestPackings, std::chrono::microseconds runtime);

    ~PackingGenetic();

    virtual void operator()(PackingList &result, std::vector<PackingList> &out);

private:
    // 选择
    void select();
    // 交叉
    void cross();
    // 变异
    void mutate();

    // 对两个个体进行交叉
    PackingList searchListCross(const PackingList& lho, const PackingList& rho);
    // 对一个个体进行变异
    PackingList searchListMutate(const PackingList srcList, long long seed);
    // 获取当前种群中最好的解
    void calcBestOne();

    // 二分查找恰好比key大的元素
    template<typename _Type>
    int searchForBiggerThan(const std::vector<_Type>& container, _Type key);
private:
    // 种群数量
    int mNumOfPop;
    // 种群数的一半
    int mNumOfHalfPop;

    // 当前种群
    std::vector<PackingList> mPopulation;

    // 这一代选出做父母的下标
    std::vector<std::pair<int, int> > mParents;

    // 下一代产生的临时个体
    std::vector<PackingList> mNextPopulation;

    // 当前代种群中最好的个体
    PackingList mBestPopulationInCurrentGeneration;

private:
    // 交配概率
    static const double PROPORTION_CROSS;
    // 变异概率
    static const double PROPORTION_MUTATE;
};
