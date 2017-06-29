#pragma once

#include <list>
#include <chrono>

#include "Rectangle.h"
#include "SpaceManager.h"

class PackingBase
{
public:
    PackingBase(double sheetWidth, const std::vector<Rectangle*>& rectangles, const PackingList& bestPacking, std::chrono::microseconds runtime = std::chrono::seconds(10));
    ~PackingBase();
    // 搜索方法，函子
    virtual void operator()(PackingList &result, std::vector<PackingList> &out) = 0;
protected:
    // 启发式Packing算法
    // 输入：Packing序列
    // 输出：所需要的材料板的长度
    // Pre：由localSearch或者simulatedAnnealing算法调用，并且保证每一个待裁剪的矩形的宽度都不大于材料板的宽度
    // Post：无
    PackingList heuristicPacking(const std::vector<Rectangle*>& searchList) const;

    // 获取待搜索列表中最小的空间的宽度
    double getMinWidth(const std::list<Rectangle*>& searchList) const;

    // 获取剩余搜索列表中，评分最高的、可以填进空间space的矩形，并把它从列表中移除
    // judge为评分函数
    Rectangle* popHighestScoreRectangle(std::list<Rectangle*>& searchList, const Space* const space, int(*judge)(const Rectangle* const, const Space* const)) const;

    // 获取剩余搜索列表中，宽度最大且不超过spaceWidth的矩形
    Rectangle* popMaxWidthRectangle(std::list<Rectangle*>& searchList, double spaceWidth) const;

    // 张德富老师的评分方式
    static int judgeZhang(const Rectangle* const, const Space* const);
    // 论文 'A simple randomized algorithm for two-dimensional strip packing' 的评分方式
    static int judegYang(const Rectangle* const, const Space* const);

protected:
    double m_sheetWidth;
    PackingList m_bestPacking;
    std::vector<Rectangle*> m_bestSearchList;
    // 总共要裁剪的矩形数量
    int m_nRect;
    // 所有要裁剪的矩形
    std::vector<Rectangle*> m_rectangles;

    // 算法启动时间
    std::chrono::steady_clock::time_point m_beginTime;
    // 最大运行时间
    std::chrono::microseconds m_maxRunTime;
};
