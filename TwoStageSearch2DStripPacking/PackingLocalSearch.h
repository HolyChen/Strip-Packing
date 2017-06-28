#pragma once
#include "PackingBase.h"

class PackingLocalSearch :
    public PackingBase
{
public:
    PackingLocalSearch(double sheetWidth, const std::vector<Rectangle*>& rectangles, const PackingList& bestPacking);
    ~PackingLocalSearch();

    // Í¨¹ý PackingBase ¼Ì³Ð
    virtual PackingList operator()() override;
};

