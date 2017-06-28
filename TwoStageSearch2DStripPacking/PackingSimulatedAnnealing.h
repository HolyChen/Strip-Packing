#pragma once
#include "PackingBase.h"

class PackingSimulatedAnnealing :
    public PackingBase
{
public:
    PackingSimulatedAnnealing(double sheetWidth, const std::vector<Rectangle*>& rectangles, const PackingList& bestPacking);
    ~PackingSimulatedAnnealing();

    // Í¨¹ý PackingBase ¼Ì³Ð
    virtual PackingList operator()() override;
};

