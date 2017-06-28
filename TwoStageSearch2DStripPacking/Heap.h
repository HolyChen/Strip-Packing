#pragma once

#include <memory>
#include <cstdlib>
#include <limits>

template<typename T, bool (*comparator)(const T&, const T&)>
class Heap
{
private:
	typedef T* _Pt;
public:
	Heap();
	~Heap();
	void push(const T&);
	void push(T&&);
	void push(T*);
	T* top();
	T* const top() const;
	std::auto_ptr<T> pop();

	int size() const;
	bool isEmpty() const;
private:
	void realloc(int newCapacity);
private:
	_Pt* m_dataSpace = nullptr;
	int m_size = 0;
	int m_capacity = 0;
	static const int DEFAULT_CAPCITY = 16;
};

template<typename T, bool(*comparator)(const T&, const T&)>
Heap<T, comparator>::Heap()
{
	realloc(DEFAULT_CAPCITY);
}

template<typename T, bool(*comparator)(const T&, const T&)>
Heap<T, comparator>::~Heap()
{
	if (m_dataSpace != nullptr)
	{
		for (int i = 0; i < m_size; i++)
		{
			if (m_dataSpace[i] != nullptr)
			{
				delete m_dataSpace[i];
				m_dataSpace[i] = nullptr;
			}
		}
		delete[] m_dataSpace;
		m_dataSpace = nullptr;
	}
}

template<typename T, bool(*comparator)(const T&, const T&)>
void Heap<T, comparator>::push(const T& data)
{
	push(new T(data));
}

template<typename T, bool(*comparator)(const T &, const T &)>
void Heap<T, comparator>::push(T && data)
{
	push(new T(data));
}

template<typename T, bool(*comparator)(const T &, const T &)>
void Heap<T, comparator>::push(T * data)
{
	realloc(m_size + 1);
	int i = m_size;
	while (i > 0 && comparator(*data, *m_dataSpace[(i + 1) / 2 - 1]))
	{
		m_dataSpace[i] = m_dataSpace[(i + 1) / 2 - 1];
		i = (i + 1) / 2 - 1;
	}
	m_dataSpace[i] = data;
	m_size++;
}

template<typename T, bool(*comparator)(const T&, const T&)>
T* Heap<T, comparator>::top()
{
	if (m_size > 0)
	{
		return m_dataSpace[0];
	}
	else
	{
		return nullptr;
	}
}

template<typename T, bool(*comparator)(const T&, const T&)>
T* const Heap<T, comparator>::top() const
{
	if (m_size > 0)
	{
		return static_cast<T* const>(m_dataSpace[0]);
	}
	else
	{
		return nullptr;
	}
}

template<typename T, bool(*comparator)(const T&, const T&)>
std::auto_ptr<T> Heap<T, comparator>::pop()
{
	if (m_size > 0)
	{
		std::auto_ptr<T> toResult(m_dataSpace[0]);

		// 调整堆
		// 首先将堆最后一个元素提取出来，逻辑上放到堆顶
		_Pt lastOne = m_dataSpace[--m_size];
		m_dataSpace[m_size] = nullptr;

		int i = 0, child = 1;
		while (child < m_size)
		{
			if (child + 1 < m_size && comparator(*m_dataSpace[child + 1], *m_dataSpace[child]))
			{
				child++;
			}
			if (comparator(*m_dataSpace[child], *lastOne))
			{
				m_dataSpace[i] = m_dataSpace[child];
				i = child;
				child = i * 2 + 1;
			}
			else
			{
				break;
			}
		}
		m_dataSpace[i] = lastOne;
		return toResult;
	}
	else
	{
		return std::auto_ptr<T>(nullptr);
	}
}

template<typename T, bool(*comparator)(const T &, const T &)>
inline int Heap<T, comparator>::size() const
{
	return m_size;
}

template<typename T, bool(*comparator)(const T &, const T &)>
inline bool Heap<T, comparator>::isEmpty() const
{
	return m_size == 0;
}

template<typename T, bool(*comparator)(const T&, const T&)>
void Heap<T, comparator>::realloc(int newCapacity)
{
	if (newCapacity < m_capacity || newCapacity < DEFAULT_CAPCITY)
	{
		return;
	}

	// 向上取整到第一个不小于newCapacity的2^n的数
	int pow2Capacity = newCapacity;
	pow2Capacity |= pow2Capacity >> 1;
	pow2Capacity |= pow2Capacity >> 2;
	pow2Capacity |= pow2Capacity >> 4;
	pow2Capacity |= pow2Capacity >> 8;
	pow2Capacity |= pow2Capacity >> 16;
	pow2Capacity += 1;

	if (pow2Capacity <= 0)
	{
		pow2Capacity = std::numeric_limits<int>::max() - 7;
	}

	_Pt* newSpace = new _Pt[pow2Capacity];
	memset(newSpace, 0, sizeof(_Pt) * pow2Capacity);
	if (m_dataSpace != nullptr)
	{
		for (int i = 0; i < m_size; i++)
		{
			newSpace[i] = m_dataSpace[i];
		}
		delete[] m_dataSpace;
	}
	m_dataSpace = newSpace;
	m_capacity = pow2Capacity;
}

