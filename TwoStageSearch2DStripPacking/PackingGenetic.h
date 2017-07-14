#pragma once
#include "PackingBase.h"

#include <chrono>

class PackingGenetic :
    public PackingBase
{
public:
    PackingGenetic(double sheetWidth, const std::vector<Rectangle*>& rectangles, const PackingList& bestPacking, std::chrono::microseconds runtime);
    PackingGenetic(double sheetWidth, const std::vector<Rectangle*>& rectangles, const std::vector<PackingList>& bestPackings, std::chrono::microseconds runtime);

    ~PackingGenetic();

    virtual void operator()(PackingList &result, std::vector<PackingList> &out);

private:
    // ѡ��
    void select();
    // ����
    void cross();
    // ����
    void mutate();

    // ������������н���
    PackingList searchListCross(const PackingList& lho, const PackingList& rho);
    // ��һ��������б���
    PackingList searchListMutate(const PackingList srcList, long long seed);
    // ��ȡ��ǰ��Ⱥ����õĽ�
    void calcBestOne();

    // ���ֲ���ǡ�ñ�key���Ԫ��
    template<typename _Type>
    int searchForBiggerThan(const std::vector<_Type>& container, _Type key);
private:
    // ��Ⱥ����
    int mNumOfPop;
    // ��Ⱥ����һ��
    int mNumOfHalfPop;

    // ��ǰ��Ⱥ
    std::vector<PackingList> mPopulation;

    // ��һ��ѡ������ĸ���±�
    std::vector<std::pair<int, int> > mParents;

    // ��һ����������ʱ����
    std::vector<PackingList> mNextPopulation;

    // ��ǰ����Ⱥ����õĸ���
    PackingList mBestPopulationInCurrentGeneration;

private:
    // �������
    static const double PROPORTION_CROSS;
    // �������
    static const double PROPORTION_MUTATE;
};
