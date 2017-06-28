#include "stdafx.h"
#include "Space.h"


Space::Space()
{
}

Space::Space(double x, double y, double h1, double h2, double width)
	: x(x), y(y), h1(h1), h2(h2), width(width)
{
}


Space::~Space()
{
}

bool Space::comparatorLowestLeftest(const Space & s1, const Space & s2)
{
	if (s1.y < s2.y)
	{
		return true;
	}
	else if (s1.y == s2.y)
	{
		if (s1.x < s2.x)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return false;
}
