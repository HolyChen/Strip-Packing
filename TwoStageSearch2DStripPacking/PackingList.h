#pragma once

#include <vector>
#include <algorithm>

#include "Rectangle.h"

struct Assign
{
	double mX;
	double mY;
	double mWidth;
	double mHeight;
	Assign();
	Assign(double x, double y, double width, double height);
	~Assign();
};

typedef std::vector<Rectangle*> SearchList;

struct PackingList
{
public:
	std::vector<Assign> assigns;
	double h = 0.0;
    std::vector<Rectangle*> searchList;

public:
    unsigned long long hash() const
    {
        int size = searchList.size();
        int step = size / (int)(std::max(std::log(size), 1.0));
        unsigned long long sum = 0;
        for (int i = 0; i < searchList.size(); i += step)
        {
            sum += std::abs((i + 1) * (searchList[i]->id - i));
        }
        return ((size & 0xffff) << 48) | (sum);
    }
};

struct PackingListCmp {
    bool operator()(const PackingList& lhs, const PackingList& rhs) const {
        return lhs.hash() < rhs.hash();
    }
};
