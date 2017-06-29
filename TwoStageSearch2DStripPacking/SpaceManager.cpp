#include "stdafx.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include "SpaceManager.h"
#include <assert.h>
#include <chrono>

SpaceManager::SpaceManager(int sheetWidth)
	: m_sheetWidth(sheetWidth), eg(std::chrono::high_resolution_clock::now().time_since_epoch().count())
{
	m_spaces.push_back(new Space(0, 0, 0, 0, m_sheetWidth));
}


SpaceManager::~SpaceManager()
{
	for (auto &rect : m_spaces)
	{
		if (rect != nullptr)
		{
			delete rect;
		}
	}
}

Assign SpaceManager::pack(int index, const Rectangle * const rectangle)
{
    assert(rectangle->width <= m_spaces.at(index)->width);

    auto cur = m_spaces.at(index);
	const auto h1 = cur->h1, h2 = cur->h2;

	const double rectHeight = rectangle->height, rectWidth = rectangle->width;

	auto left = index > 0 ? m_spaces.at(index - 1) : nullptr;
	auto right = index < m_spaces.size() - 1 ? m_spaces.at(index + 1) : nullptr;

	Assign packing(cur->x, cur->y, rectWidth, rectHeight);

	// ���ȼ�����Ƿ��Ǻ�
	if (rectWidth == cur->width)
	{
		// ������Ǻ�ʱ�������

		// 1. ����߸߶�һ�£��������Ϊ�����඼һ�£�
		if (rectHeight == h1)
		{
			// ��ߵ�ֱ������
			if (left != nullptr)
			{
				left->width += cur->width;
				if (right != nullptr)
				{
					left->h2 = right->y - left->y > 0.0 ? right->y - left->y : 0.0;
				}
				else
				{
					left->h2 = 0.0;
				}
				// ������������Ҹ߶�һ�£���ͨ�����ң�ͬʱҲɾ�����ұ�
				if (h1 == h2)
				{
					left->width +=  right->width;
					left->h2 = right->h2;
					delete cur;
					delete right;
					m_spaces.erase(m_spaces.begin() + index, m_spaces.begin() + index + 2);
				}
				else
				{
					if (right != nullptr)
					{
						right->h1 = rectHeight - h2 > 0.0 ? rectHeight - h2 : 0.0;
					}

					delete cur;
					m_spaces.erase(m_spaces.begin() + index);
				}
			}
		}
		// 2. ���ұ߸߶�һ�£����������1�п��ǣ�
		else if (rectHeight == h2)
		{
			// �Ҳ��ֱ������
			if (right != nullptr)
			{
				right->width += cur->width;
				if (left != nullptr)
				{
					right->h1 = left->y - right->y > 0.0 ? left->y - right->y : 0.0;
				}
				else
				{
					0.0;
				}
				right->x = cur->x;

				if (left != nullptr)
				{
					left->h2 = rectHeight - h1 > 0.0 ? rectHeight - h1 : 0.0;
				}

				delete cur;
				m_spaces.erase(m_spaces.begin() + index);
			}
		}
		// 3. ����ߵĸߣ�
		else if (rectHeight > h1 && rectHeight > h2)
		{
			cur->y += rectHeight;
			cur->h1 = cur->h2 = 0.0;
			if (left != nullptr)
			{
				left->h2 = cur->y - left->y;
			}
			if (right != nullptr)
			{
				right->h1 = cur->y - right->y;
			}
		}
		// 4. ������߶�֮�䣻
		else if (rectHeight > std::min(h1, h2) && rectHeight < std::max(h1, h2))
		{
			cur->y += rectHeight;
			// ����ߵĵͣ����ұߵĸߣ���ô�����ұߵ�h1
			if (h1 > h2)
			{
				if (right != nullptr)
				{
					right->h1 = cur->y - right->y;
				}
				cur->h1 = left->y - cur->y;
				cur->h2 = 0.0;
			}
			// ����߸ߣ����ұߵͣ���Ҫ������ߵ�h2
			else
			{
				if (left != nullptr)
				{
					left->h2 = cur->y - left->y;
				}
				cur->h2 = right->y - cur->y;
				cur->h1 = 0;
			}
		}
		// 5. ����͵ĵ͡�
		else
		{
			cur->y += rectHeight;
			cur->h1 -= rectHeight;
			cur->h2 -= rectHeight;
		}
	}
	// ���εĿ�ȱȿռ�Ŀ��С�����ʱ����Ҫ�޸Ŀռ�λ�ã����ߴ����µĿռ�
	else
	{
		// �����¼������

		// 1. ����߸߶�һ��
		if (rectHeight == h1)
		{
			// ����ߵ����أ�������ռ��޸�Ϊpack֮���ʣ��ռ�
			if (left != nullptr)
			{
				left->width += rectWidth;
				left->h2 = 0.0;
				cur->x += rectWidth;
				cur->width -= rectWidth;
				cur->h1 = rectHeight;
			}
		}
		// 2. ���ұ߸߶�һ��
		else if (rectHeight == h2)
		{
			// ���ұߵ��������أ�������ռ��޸�Ϊpack֮���ʣ��ռ�
			if (right != nullptr)
			{
				packing.mX = right->x - rectWidth;

				right->width += rectWidth;
				right->h1 = 0.0;
				right->x -= rectWidth;

				cur->width -= rectWidth;
				cur->h2 = rectHeight;
			}
		}
		// 3. �����Ҷ���һ�£�������������������������
		else
		{
			// �������
			// if (true)
			if (std::bernoulli_distribution(0.5)(eg))
			{
				if (left != nullptr)
				{
					left->h2 = rectHeight - h1 > 0.0 ? rectHeight - h1 : 0.0;
				}
				// �½�һ���ռ����cur�ұ�
				m_spaces.insert(m_spaces.begin() + index + 1, new Space(cur->x + rectWidth, cur->y, rectHeight, cur->h2, cur->width - rectWidth));
				cur->h1 = h1 - rectHeight > 0.0 ? h1 - rectHeight : 0.0;
				cur->h2 = 0.0;
				cur->width = rectWidth;
				cur->y += rectHeight;
			}
			// ���Ҳ�
			else
			{
				if (right != nullptr)
				{
					right->h1 = rectHeight - h2 > 0.0 ? rectHeight - h2 : 0.0;
					packing.mX = right->x - rectWidth;
				}
				else
				{
					packing.mX = m_sheetWidth - rectWidth;
				}
				// �½�һ���ռ����cur���
				m_spaces.insert(m_spaces.begin() + index, new Space(cur->x, cur->y, cur->h1, rectHeight, cur->width - rectWidth));
				cur->h1 = 0.0;
				cur->h2 = h2 - rectHeight > 0.0 ? h2 - rectHeight : 0.0;
				cur->x += cur->width - rectWidth;
				cur->y += rectHeight;
				cur->width = rectWidth;
			}
		}
	}

	if (m_spaces.size() == 0)
	{
		// ���߼��ϲ��ᷢ����������һ�������ء���
		std::cout << "���������һ��" << std::endl;
		m_spaces.push_back(new Space(0, 0, 0, 0, m_sheetWidth));
	}

	return packing;
}

void SpaceManager::erase(int index)
{
	auto cur = m_spaces.at(index);
	decltype(cur) left = nullptr, right = nullptr;
	// ��־λ��0��ʾû���ھӣ�1��ʾ��ߵ��ھӽϵͣ�2��ʾ�ұߵ��ھӽϵͣ�3��ʾ����һ����
	int flag = 0;

	// ����������ھӵĸ߶Ȳ�
	double deltaH = 0.0;

	// �����ߵ��ھ�
	if (index > 0)
	{
		left = m_spaces.at(index - 1);
		flag = 1;
		assert(left->y > cur->y);
	}
	// ����ұߵ��ھ�
	if (index < m_spaces.size() - 1)
	{
		right = m_spaces.at(index + 1);
		if (index > 0)
		{
			assert(right->y > cur->y);
			deltaH = right->y - left->y;
			if (deltaH < 0)
			{
				left = right;
				flag = 2;
			}
			else if (deltaH == 0)
			{
				flag = 3;
			}
		}
		else
		{
			flag = 2;
            left = right;
		}
	}
	switch (flag)
	{
	case 0: 	// û���ھ�
		// TODO: ˵������һ����εĿ�ȴ��ڲ��ϰ�Ŀ�ȣ���������¸�����������ɲü�����
		// ɾ����cur
		delete cur;
		m_spaces.erase(m_spaces.begin() + index);
		assert(flag != 0);
		break;
	case 1:		// ��ߵ��ھӽϵ�
		left = m_spaces.at(index - 1);
		// ������ھӵĿ����չ����������h2Ϊcur���ұߵ��ھ�������ھӵĸ߶Ȳ�
		left->width += cur->width;
		left->h2 = deltaH;
		// ɾ����cur
		delete cur;
		m_spaces.erase(m_spaces.begin() + index);
		break;
	case 2:		// �ұߵ��ھӽϵ�
		// ���ұߵ��ھ��������չ��������h1Ϊcur����ھ����ұ��ھӵĸ߶Ȳͬʱ�޸�x����
		right = m_spaces.at(index + 1);
		right->width += cur->width;
		right->x = cur->x;
		right->h1 = std::abs(deltaH);
        // ɾ����cur
        delete cur;
        m_spaces.erase(m_spaces.begin() + index);
		break;
	case 3:		// ���ߵ��ھ�һ����
		// ����ߵ��ھ����ұߵ��ھӴ�ͨ
		left->width += cur->width + right->width;
		left->h2 = right->h2;
		// ɾ����cur���ұ��ھ�
		delete cur;
		delete m_spaces.at(index + 1);
		m_spaces.erase(m_spaces.begin() + index, m_spaces.begin() + index + 2);
        break;
	default:
		break;
	}
}

int SpaceManager::lowestLeftest()
{
	auto llest = m_spaces.cbegin();
	for (auto iter = m_spaces.cbegin() + 1; iter != m_spaces.cend(); iter++)
	{
		if (Space::comparatorLowestLeftest(**iter, **llest))
		{
			llest = iter;
		}
	}
	return llest - m_spaces.cbegin();
}

Space * const SpaceManager::get(int index) const
{
	if (index >= 0)
	{
		return m_spaces.at(index);
	}
	else
	{
		return nullptr;
	}
}
