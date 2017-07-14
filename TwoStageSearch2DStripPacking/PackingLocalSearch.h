#pragma once
#include "PackingBase.h"

class PackingLocalSearch :
    public PackingBase
{
public:
    PackingLocalSearch(double sheetWidth, const std::vector<Rectangle*>& rectangles, const PackingList& bestPacking, std::chrono::microseconds runtime);
    ~PackingLocalSearch();

    // 通过 PackingBase 继承
    virtual void operator()(PackingList &result, std::vector<PackingList> &out) override;

private:
    void singleSearch(PackingList &result, std::vector<PackingList> &out);

private:
    // 一个有序的搜索队列，按照h从小到大排列
    std::vector<PackingList> mSearchQueue;

    // true为对单个列表进行搜索，false为对多个列表进行搜索
    bool isSingleSearch = true;
};

