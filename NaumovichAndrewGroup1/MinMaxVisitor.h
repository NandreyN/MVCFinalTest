#pragma once
#include "BaseVisitor.h"
#include <utility>
#include "Container.h"
#include <vector>
#include <algorithm>
template <class T>
class MinVisitor : public BaseVisitor<T>
{
	T _min;
public:
	T GetData() { return _min; }
	void visit(Container<T>& c) override
	{
		DIterator<T> iter(c);
		vector<T> content;

		while (iter.isDone())
		{
			content.push_back(iter.currentItem());
			iter.next();
		}
		if (content.empty()) return;
		_min = *min_element(content.begin(), content.end());
	}
};

template<class T>
class MaxVisitor : public BaseVisitor<T>
{
	T _max;
public:
	T GetData() { return _max; }
	void visit(Container<T>& c) override
	{
		DIterator<T> iter(c);
		vector<T> content;

		while (iter.isDone())
		{
			content.push_back(iter.currentItem());
			iter.next();
		}
		if (content.empty()) return;
		_max = *max_element(content.begin(), content.end());
	}
};