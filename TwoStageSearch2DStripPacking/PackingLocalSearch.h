#pragma once
#include "PackingBase.h"

class PackingLocalSearch :
    public PackingBase
{
public:
    PackingLocalSearch(double sheetWidth, const std::vector<Rectangle*>& rectangles, const PackingList& bestPacking);
    ~PackingLocalSearch();

    // ͨ�� PackingBase �̳�
    virtual PackingList operator()() override;
};

