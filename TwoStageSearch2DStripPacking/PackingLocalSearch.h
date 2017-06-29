#pragma once
#include "PackingBase.h"

class PackingLocalSearch :
    public PackingBase
{
public:
    PackingLocalSearch(double sheetWidth, const std::vector<Rectangle*>& rectangles, const PackingList& bestPacking);
    ~PackingLocalSearch();

    // Í¨¹ý PackingBase ¼Ì³Ð
    virtual void operator()(PackingList &result, std::chrono::microseconds runtime = std::chrono::seconds(10)) override;
};

