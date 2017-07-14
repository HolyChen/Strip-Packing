#pragma once
#include "PackingBase.h"

class PackingSimulatedAnnealing :
    public PackingBase
{
public:
    PackingSimulatedAnnealing(double sheetWidth, const std::vector<Rectangle*>& rectangles, const PackingList& bestPacking, std::chrono::microseconds runtime);
    ~PackingSimulatedAnnealing();

    // ͨ�� PackingBase �̳�
    virtual void operator()(PackingList &result, std::vector<PackingList> &out) override;
};

