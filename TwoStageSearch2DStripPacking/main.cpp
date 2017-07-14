// TwoStageSearch2DStripPacking.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include "Rectangle.h"
#include "Packing.h"

#include <string>
#include <array>
#include <numeric>
#include <utility>

#include <iostream>
#include <fstream>

using fsecons = std::chrono::duration<float, std::chrono::seconds::period>;

// ��1������Ϊ�������ݵ�����
// ��2������Ϊ���������ļ���·��
// ��3������Ϊ����ļ���Ŀ¼��·��
// ��4������Ϊͳ���������·��
int main(int argc, char* argv[])
{
    if (argc < 5)
    {
        return 1;
    }

    std::string caseName(argv[1]);
    std::ifstream input;

    input.open(argv[2]);


    double stripWidth, lowerBound;
    int n;

    input >> stripWidth >> lowerBound >> n;

    Packing p(stripWidth, n, lowerBound);

    for (int i = 0; i < n; i++)
    {
        double width, height;
        input >> width >> height;
        p.pushRectangle(Rectangle(width, height, i));
    }

    input.close();

    const int times = 10;
    double hs[times];

    for (int seed = 0; seed < times; seed++)
    {
        auto begin = std::chrono::steady_clock::now();
        auto packingList = p.isa();
        auto duration = fsecons(std::chrono::steady_clock::now() - begin);

        std::cout << caseName + " seed " + std::to_string(seed + 1) + " in " + std::to_string(times) + " completed" << std::endl;
        std::ofstream output;
        output.open(std::string(argv[3]) + "/" + caseName + "_seed" + std::to_string(seed + 1) + ".pack");
        output << "h: " << packingList.h << std::endl;
        output << "time: " << (duration).count() << std::endl;
        output << stripWidth << " " << lowerBound << std::endl;
        output << n << std::endl;

        for (const auto & rect : p.getRectangles())
        {
            output << rect->width << " " << rect->height << std::endl;
        }

        output << packingList.assigns.size() << std::endl;

        for (int i = 0; i < packingList.assigns.size(); i++)
        {
            const auto& assign = packingList.assigns[i];
            output << assign.mWidth << " " << assign.mHeight << " " << assign.mX << " " << assign.mY << " "
                << assign.mX + assign.mWidth << " " << assign.mY + assign.mHeight << " " << i + 1 << std::endl;
        }

        output << 0 << std::endl;

        output.close();

        hs[seed] = packingList.h;

    }

    std::ofstream statisticResult;
    statisticResult.open(argv[4], std::ios::app);

    statisticResult << caseName << " " << n << " " << stripWidth << " " << lowerBound << " " << times << " ";
    for (int i = 0; i < times; i++)
    {
        statisticResult << hs[i] << " ";
    }
    statisticResult << std::endl;
    statisticResult.close();

    try
    {


    }
    catch (...)
    {
        return 1;
    }

    
    return 0;
}

