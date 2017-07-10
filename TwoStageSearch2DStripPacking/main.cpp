// TwoStageSearch2DStripPacking.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "Rectangle.h"
#include "Packing.h"

#include <string>
#include <array>
#include <numeric>

#include <fstream>

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        return 1;
    }

    try
    {

    }
    catch (std::exception &e)
    {
        return 1;
    }

    std::ifstream input(argv[1]);
    std::string caseName(argv[1]);
    caseName = caseName.substr(0, caseName.length() - 4);

    std::ofstream output(caseName + std::string("_result.out"));

    double stripeWidth, lowerBound;
    int n;


	//std::ofstream outFile("resultLog.txt", std::ios::out);

	//auto std_cout = std::cout.rdbuf();

	//std::cout.rdbuf(outFile.rdbuf());

	int round;

	std::cin >> round;

	for (auto k = 0; k < round; k++)
	{
		std::cout << "Case " << k + 1 << std::endl;
		double stripWidth;
		int n;
		std::cin >> stripWidth >> n;
		std::cout << stripWidth << " " << n << std::endl;
		std::cout << std::endl;

		const int times = 5;
		std::array<double, times> result;

		Packing p(stripWidth, n);

		double area = 0.0;

		for (int i = 0; i < n; i++)
		{
			double width, heigth;
			std::cin >> heigth >> width;
			std::cout << heigth << " " << width << std::endl;
			p.pushRectangle(Rectangle(width, heigth, i));
			area += width * heigth;
		}

		std::cout << std::endl;

		for (int i = 0; i < times; i++)
		{
			auto packingList = p.isa();

			std::cout << "Rects' Area:  " << area << "\t ";
			std::cout << "Strip's Ares: " << packingList.h * stripWidth << "\t ";
			std::cout << "Strip's Height: " << packingList.h << std::endl;
			// if (packingList.h * stripWidth == area)
			{
                auto deltaArea = packingList.h * stripWidth - area;
                if (deltaArea < 0)
                {
                    std::cout << "------------- Wrong Packing: -------------" << std::endl;
                }
                else if (deltaArea > 0)
                {
                    std::cout << "------------- Not Best Packing: -------------" << std::endl;
                }
                else
                {
                    std::cout << "------------- Best Packing: -------------" << std::endl;
                }
				std::cout << n << " " << stripWidth << " " << packingList.h << std::endl;
				for (auto&& packing : packingList.assigns)
				{
					/*std::cout << "(" << packing.mX << ", " << packing.mY << ")"
						<< " <-- " << "(" << packing.mWidth << ", " << packing.mHeight << ")" << std::endl;*/
					std::cout << packing.mX << " " << packing.mY << " " << packing.mWidth << " " << packing.mHeight << std::endl;
				}
			}
			if (packingList.h * stripWidth == area)
			{
				break;
			}
			result[i] = packingList.h;
		}

		std::cout << "Mean: " << std::accumulate(result.begin(), result.end(), 0.0) / (double)times << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
	}
/*
	std::cout.rdbuf(std_cout);

	outFile.close();*/
    return 0;
}

