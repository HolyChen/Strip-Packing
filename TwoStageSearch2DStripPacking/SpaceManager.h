#pragma once
#include "Space.h"
#include "Rectangle.h"
#include <vector>
#include "PackingList.h"

#include <random>


class SpaceManager
{
private:
	std::vector<Space*> m_spaces;
	int m_sheetWidth;
	std::default_random_engine eg;
public:
	SpaceManager(int sheetWidth = 0);
	~SpaceManager();

	// �����ǰ�������еĿ��õĿռ䣬��ô�����������������Ϸ�δʹ�õĲ�����Ϊ���ÿռ�

	// ��һ������pack����index�ռ���
	Assign pack(int index, const Rectangle* const);
	// ɾ����index���ռ�
	void erase(int index);
	// ��ȡ���¡������Ŀռ������λ��
	int lowestLeftest();
	// ���ݿռ������λ�û�ȡ�ÿռ�
	Space *const get(int index) const;
};

