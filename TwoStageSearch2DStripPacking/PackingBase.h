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
    // ��������������
    virtual void operator()(PackingList &result, std::vector<PackingList> &out) = 0;
protected:
    // ����ʽPacking�㷨
    // ���룺Packing����
    // ���������Ҫ�Ĳ��ϰ�ĳ���
    // Pre����localSearch����simulatedAnnealing�㷨���ã����ұ�֤ÿһ�����ü��ľ��εĿ�ȶ������ڲ��ϰ�Ŀ��
    // Post����
    PackingList heuristicPacking(const std::vector<Rectangle*>& searchList) const;

    // ��ȡ�������б�����С�Ŀռ�Ŀ��
    double getMinWidth(const std::list<Rectangle*>& searchList) const;

    // ��ȡʣ�������б��У�������ߵġ���������ռ�space�ľ��Σ����������б����Ƴ�
    // judgeΪ���ֺ���
    Rectangle* popHighestScoreRectangle(std::list<Rectangle*>& searchList, const Space* const space, int(*judge)(const Rectangle* const, const Space* const)) const;

    // ��ȡʣ�������б��У��������Ҳ�����spaceWidth�ľ���
    Rectangle* popMaxWidthRectangle(std::list<Rectangle*>& searchList, double spaceWidth) const;

    // �ŵ¸���ʦ�����ַ�ʽ
    static int judgeZhang(const Rectangle* const, const Space* const);
    // ���� 'A simple randomized algorithm for two-dimensional strip packing' �����ַ�ʽ
    static int judegYang(const Rectangle* const, const Space* const);

protected:
    double m_sheetWidth;
    PackingList m_bestPacking;
    std::vector<Rectangle*> m_bestSearchList;
    // �ܹ�Ҫ�ü��ľ�������
    int m_nRect;
    // ����Ҫ�ü��ľ���
    std::vector<Rectangle*> m_rectangles;

    // �㷨����ʱ��
    std::chrono::steady_clock::time_point m_beginTime;
    // �������ʱ��
    std::chrono::microseconds m_maxRunTime;
};
