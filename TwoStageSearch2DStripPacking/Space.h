#pragma once
class Space
{
public:
	// ���½ǵĺ�������
	double x;
	double y;
	// ��߱ڵĸ߶�
	double h1;
	// �ұ߱ڵĸ߶�
	double h2;
	// ���
	double width;

	Space();
	Space(double x, double y, double h1, double h2, double width);
	~Space();

	static bool comparatorLowestLeftest(const Space& s1, const Space& s2);
};

