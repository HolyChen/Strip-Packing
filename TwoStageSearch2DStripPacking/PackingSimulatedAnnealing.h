#pragma once
#include "PackingBase.h"

class PackingSimulatedAnnealing :
    public PackingBase
{
public:
    PackingSimulatedAnnealing(double sheetWidth, const std::vector<Rectangle*>& rectangles, const PackingList& bestPacking, std::chrono::microseconds runtime = std::chrono::seconds(10));
    ~PackingSimulatedAnnealing();

    // Í¨¹ý PackingBase ¼Ì³Ð
    virtual void operator()(PackingList &result, std::vector<PackingList> &out) override;
};

