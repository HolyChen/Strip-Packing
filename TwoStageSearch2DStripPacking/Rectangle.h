#pragma once
class Rectangle
{
public:
	typedef Rectangle* _PRectangle;
public:
    const int id;
	double width = 0;
	double height = 0;

	double getPerimeter() const { return 2 * (width + height); }
	double getArea() const { return width * height; };

    Rectangle() : id(0) { }
	Rectangle(double width, double height, int id) : width(width), height(height), id(id) {}
	~Rectangle() {}

	static double getPerimeter(const Rectangle& rect) { return rect.width + rect.height; }
	static double getPerimeter(const Rectangle* const rect) { return rect->width + rect->height; }
	static double getArea(const Rectangle& rect) { return rect.width * rect.height; }
	static double getArea(const Rectangle* const rect) { return rect->width * rect->height; }


	// ��������ȽϺ���

	// �����ܳ�����
	static bool comparatorPerimeter(const Rectangle& x1, const Rectangle& x2)
	{
		return (x1.width + x1.height) < (x2.width + x2.height);
	}

	// ���տ�Ƚ���
	static bool compratorWidth(const Rectangle& x1, const Rectangle& x2)
	{
		return (x1.width) < (x2.width);
	}

	// ��������ȽϺ�����ָ��汾
	// �����ܳ�����
	static bool comparatorPerimeter(const _PRectangle& x1, const _PRectangle& x2)
	{
		return (x1->width + x1->height) < (x2->width + x2->height);
	}

	// ���տ�Ƚ���
	static bool compratorWidth(const _PRectangle& x1, const _PRectangle& x2)
	{
		return (x1->width) < (x2->width);
	}
};

