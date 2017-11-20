#include "Container.h"
#include <algorithm>
#include <stdexcept>
#include <vector>
using namespace std;

bool Container::isEmpty() const
{
	return size() == 0 ? true : false;;
}

int Container::size() const
{
	return (_tailIndex == -1 || _headIndex == -1) ? 0 : _tailIndex - _headIndex + 1;
}

void Container::clear()
{
	delete[] _dataPtr;
	_dataPtr = new int[_currentCapacity];
	_headIndex = _currentCapacity / 2;
	_tailIndex = _headIndex - 1;
}

Container & Container::operator=(const Container & c)
{
	this->clear();
	Iterator it = c.makeIterator();
	delete[] _dataPtr;

	_dataPtr = new int[c._currentCapacity];
	_headIndex = c._headIndex;
	_tailIndex = _headIndex - 1;
	while (!it.isDone())
	{
		push_back(it.Current());
		it.Next();
	}
	_currentCapacity = c._currentCapacity;
	return *this;
}

Container::Container() : _currentCapacity(6)
{
	_dataPtr = new int[_currentCapacity];
	configureInd();
}

Container::Container(const Container & c)
{
	*this = operator=(c);
}

Container::Container(const std::initializer_list<int>& list)
{
	_currentCapacity = list.size();
	int ratio = 2;
	_headIndex = ((_currentCapacity * ratio) / 2) - (_currentCapacity / 2);
	_tailIndex = _headIndex + size();
	_currentCapacity *= ratio;

	_dataPtr = new int[_currentCapacity];
	configureInd();
	for (auto c : list)
		push_back(c);
}


Container::~Container()
{
	delete[] _dataPtr;
}

void Container::configureInd()
{
	_headIndex = _currentCapacity / 2;
	_tailIndex = _headIndex - 1;
}

void Container::accept(BaseVisitor &visitor)
{
	visitor.visit(*this);
}

Iterator Container::makeIterator() const
{
	return Iterator(this);
}

int Container::pop_back()
{
	if (isEmpty())
		throw std::out_of_range("Empty container");
	return _dataPtr[_tailIndex--];
}

int Container::pop_front()
{
	if (isEmpty())
		throw std::out_of_range("Empty container");
	return _dataPtr[_headIndex++];
}

void Container::enlarge()
{
	int ratio = 2;
	int newHeadInd = ((_currentCapacity * ratio) / 2) - (size() / 2);
	int newTailInd = newHeadInd + size() - 1;

	_currentCapacity *= ratio;

	vector<int> buffer;
	Iterator it = makeIterator();
	while (!it.isDone())
	{
		buffer.push_back(it.Current());
		it.Next();
	}

	delete[] _dataPtr;
	_dataPtr = new int[_currentCapacity];
	//int*  newPtr = new int[_currentCapacity];
	
	for (int i = 0; i < size(); i++)
		_dataPtr[newHeadInd + i] = buffer[i];
	//copy(_dataPtr.get(), _dataPtr.get() + size(), newPtr.get() + newHeadInd);

	_headIndex = newHeadInd;
	_tailIndex = newTailInd;
}

void Container::push_back(int elem)
{
	if (_tailIndex >= _currentCapacity - 1)
		enlarge();
	_tailIndex++;
	_dataPtr[_tailIndex] = elem;
}

void Container::push_front(int elem)
{
	if (_headIndex == 0)
		enlarge();

	--_headIndex;
	_dataPtr[_headIndex] = elem;
}
