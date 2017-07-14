#pragma once
#include "PackingBase.h"

class PackingLocalSearch :
    public PackingBase
{
public:
    PackingLocalSearch(double sheetWidth, const std::vector<Rectangle*>& rectangles, const PackingList& bestPacking, std::chrono::microseconds runtime);
    ~PackingLocalSearch();

    // ͨ�� PackingBase �̳�
    virtual void operator()(PackingList &result, std::vector<PackingList> &out) override;

private:
    void singleSearch(PackingList &result, std::vector<PackingList> &out);

private:
    // һ��������������У�����h��С��������
    std::vector<PackingList> mSearchQueue;

    // trueΪ�Ե����б����������falseΪ�Զ���б��������
    bool isSingleSearch = true;
};

