#pragma once
class Space
{
public:
	// 左下角的横纵坐标
	double x;
	double y;
	// 左边壁的高度
	double h1;
	// 右边壁的高度
	double h2;
	// 宽度
	double width;

	Space();
	Space(double x, double y, double h1, double h2, double width);
	~Space();

	static bool comparatorLowestLeftest(const Space& s1, const Space& s2);
};

