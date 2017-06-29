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

	// 首先检查宽度是否吻合
	if (rectWidth == cur->width)
	{
		// 当宽度吻合时的情况：

		// 1. 和左边高度一致，特殊情况为和两侧都一致；
		if (rectHeight == h1)
		{
			// 左边的直接延拓
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
				// 特殊情况，左右高度一致，打通左中右，同时也删除掉右边
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
		// 2. 和右边高度一致，特殊情况在1中考虑；
		else if (rectHeight == h2)
		{
			// 右侧的直接延拓
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
		// 3. 比最高的高；
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
		// 4. 在两侧高度之间；
		else if (rectHeight > std::min(h1, h2) && rectHeight < std::max(h1, h2))
		{
			cur->y += rectHeight;
			// 比左边的低，比右边的高，那么调整右边的h1
			if (h1 > h2)
			{
				if (right != nullptr)
				{
					right->h1 = cur->y - right->y;
				}
				cur->h1 = left->y - cur->y;
				cur->h2 = 0.0;
			}
			// 比左边高，比右边低，需要调整左边的h2
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
		// 5. 比最低的低。
		else
		{
			cur->y += rectHeight;
			cur->h1 -= rectHeight;
			cur->h2 -= rectHeight;
		}
	}
	// 矩形的宽度比空间的宽度小，这个时候需要修改空间位置，或者创建新的空间
	else
	{
		// 有以下几种情况

		// 1. 和左边高度一致
		if (rectHeight == h1)
		{
			// 将左边的延拓，将这个空间修改为pack之后的剩余空间
			if (left != nullptr)
			{
				left->width += rectWidth;
				left->h2 = 0.0;
				cur->x += rectWidth;
				cur->width -= rectWidth;
				cur->h1 = rectHeight;
			}
		}
		// 2. 和右边高度一致
		else if (rectHeight == h2)
		{
			// 将右边的向左延拓，将这个空间修改为pack之后的剩余空间
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
		// 3. 与左右都不一致，对于这种情况，把它随机放置
		else
		{
			// 放在左侧
			// if (true)
			if (std::bernoulli_distribution(0.5)(eg))
			{
				if (left != nullptr)
				{
					left->h2 = rectHeight - h1 > 0.0 ? rectHeight - h1 : 0.0;
				}
				// 新建一个空间放在cur右边
				m_spaces.insert(m_spaces.begin() + index + 1, new Space(cur->x + rectWidth, cur->y, rectHeight, cur->h2, cur->width - rectWidth));
				cur->h1 = h1 - rectHeight > 0.0 ? h1 - rectHeight : 0.0;
				cur->h2 = 0.0;
				cur->width = rectWidth;
				cur->y += rectHeight;
			}
			// 或右侧
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
				// 新建一个空间放在cur左边
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
		// 从逻辑上不会发生，但是万一发生了呢……
		std::cout << "用完了最后一个" << std::endl;
		m_spaces.push_back(new Space(0, 0, 0, 0, m_sheetWidth));
	}

	return packing;
}

void SpaceManager::erase(int index)
{
	auto cur = m_spaces.at(index);
	decltype(cur) left = nullptr, right = nullptr;
	// 标志位，0表示没有邻居，1表示左边的邻居较低，2表示右边的邻居较低，3表示左右一样高
	int flag = 0;

	// 左右两侧的邻居的高度差
	double deltaH = 0.0;

	// 检查左边的邻居
	if (index > 0)
	{
		left = m_spaces.at(index - 1);
		flag = 1;
		assert(left->y > cur->y);
	}
	// 检查右边的邻居
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
	case 0: 	// 没有邻居
		// TODO: 说明存在一块矩形的宽度大于材料板的宽度，这种情况下根本不可能完成裁剪任务
		// 删除掉cur
		delete cur;
		m_spaces.erase(m_spaces.begin() + index);
		assert(flag != 0);
		break;
	case 1:		// 左边的邻居较低
		left = m_spaces.at(index - 1);
		// 将左边邻居的宽度拓展，并且设置h2为cur的右边的邻居与左边邻居的高度差
		left->width += cur->width;
		left->h2 = deltaH;
		// 删除掉cur
		delete cur;
		m_spaces.erase(m_spaces.begin() + index);
		break;
	case 2:		// 右边的邻居较低
		// 将右边的邻居向左边扩展，并设置h1为cur左边邻居与右边邻居的高度差，同时修改x坐标
		right = m_spaces.at(index + 1);
		right->width += cur->width;
		right->x = cur->x;
		right->h1 = std::abs(deltaH);
        // 删除掉cur
        delete cur;
        m_spaces.erase(m_spaces.begin() + index);
		break;
	case 3:		// 两边的邻居一样高
		// 将左边的邻居与右边的邻居打通
		left->width += cur->width + right->width;
		left->h2 = right->h2;
		// 删除掉cur和右边邻居
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
