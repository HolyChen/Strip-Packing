#pragma once

#include <vector>

class Rectangle;

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
	std::vector<Assign> assgins;
	double h = 0.0;
    std::vector<Rectangle*> searchList;
};
