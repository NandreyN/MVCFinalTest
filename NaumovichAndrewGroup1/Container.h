#pragma once
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include "BaseContainer.h"

using namespace std;

template<class Type>
class Container : public BaseContainer<Type>
{
	struct Node
	{
		Node()
		{
			value = 0;
			next = nullptr;
			prev = nullptr;
		}
		Node(const Type& t)
		{
			value = t;
			next = nullptr;
			prev = nullptr;
		}
		Type value;
		Node* next;
		Node* prev;
	};


public:
	class DequeConstIterator : public std::iterator<std::random_access_iterator_tag, Node*>
	{
		Node * nd;
		DequeConstIterator(Node* n) : nd(n) {};
	public:
		friend Container<Type>;
		DequeConstIterator() { nd = nullptr; };
		DequeConstIterator(const DequeConstIterator& it) :nd(it.nd) {};
		bool operator==(const DequeConstIterator& rhs) { return nd == rhs.nd; };
		bool operator!=(const DequeConstIterator& rhs) { return nd != rhs.nd; };

		const Type& operator*() const { return nd->value; };
		const Type& operator->() const { return nd->value; };
		DequeConstIterator& operator++() { DequeConstIterator cpy = *this; nd = nd->next; return cpy; }
		DequeConstIterator& operator++(int) { nd = nd->next; return *this; }

	};

	Container();
	Container(std::initializer_list<Type>); // ok
	Container(Container&&); // ok
	Container(const Container&);// ok
	Container(const string&); // ok
	~Container(); // ok

	Container& operator=(Container&&); // ok
	Container& operator=(const Container&);// ok
	Container& operator+=(const Container&);// ok

	Container operator+(const Container&);//ok

	template<class U>
	friend ostream& operator<<(ostream& o, const Container<U>&); // ok

	template<class U>
	friend istream& operator >> (istream& i, Container<U>&); // ok

	string getInternalInfo() const;

	bool operator==(const Container&) const;//ok
	bool operator!=(const Container&) const;//ok

	DequeConstIterator begin() const; // ok
	DequeConstIterator end() const; // ok

	void push_back(const Type&); // ok
	void push_front(const Type&); // ok
	Type pop_front();//ok
	Type pop_back();//ok
	Type front() const;//ok
	Type back() const;//ok

	void clear();//ok
	size_t size() const; // ok
	bool isEmpty() const; // ok
	string contentToString() const;
	Type getMin();
	Type getMax();
	void accept(BaseVisitor<Type>&) override;
private:

	Node* _head;
	Node* _tail;
	int  _length;
};

template<class T>
class DIterator
{
	Container<T>& ld;
	typename Container<T>::DequeConstIterator iter;
	int _size;
public:
	friend Container<T>;
	DIterator(Container<T>& ld) :ld(ld), _size(0)
	{
		iter = ld.begin();
	};

	T first()
	{
		return *ld.begin();
	};

	T next()
	{
		++_size;
		if (_size >= ld.size())
			return NULL;
		++iter;
		return *iter;
	};

	bool isDone()
	{
		return _size < ld.size();
	};

	T currentItem()
	{
		return *iter;
	};
};


template <class Type>
Container<Type>::Container() : _length(0)
{
}

template <class Type>
Container<Type>::Container(std::initializer_list<Type> ilist) : _length(ilist.size())
{
	_head = new Node();
	Node* headCopy = _head;

	for (auto t : ilist)
	{
		headCopy->value = t;

		Node* newHead = new Node();
		headCopy->next = newHead;
		newHead->prev = headCopy;

		headCopy = headCopy->next;
	}
	_tail = headCopy;
}

template <class Type>
Container<Type>::Container(Container&& ld)
{
	*this = operator=(ld);
}

template <class Type>
Container<Type>::Container(const Container& ld)
{
	*this = operator=(ld);
}

template <class Type>
Container<Type>::Container(const string& s)
{
	istringstream iss(s);
	vector<Type> values;

	Type t{};
	while (!iss.eof())
	{
		iss >> t;
		values.push_back(t);
	}
	_length = 0;
	for (auto val : values)
		push_back(val);
}

template <class Type>
Container<Type>::~Container()
{
	if (!isEmpty()) clear();
}

template <class Type>
Container<Type>& Container<Type>::operator=(Container&& ld)
{
	if (*this != ld)
	{
		clear();
		for (auto it = ld.begin(); it != ld.end(); ++it)
			push_back(*it);
		ld.clear();
	}
	return *this;
}

template <class Type>
Container<Type>& Container<Type>::operator=(const Container& ld)
{
	clear();
	_head = new Node();
	Node* headCopy = _head;
	for (auto it = ld.begin(); it != ld.end(); ++it)
	{
		headCopy->value = *it;
		Node* newHead = new Node();
		headCopy->next = newHead;
		newHead->prev = headCopy;

		headCopy = headCopy->next;
	}
	_tail = headCopy; // >????
	_length = ld.size();
	return *this;
}

template <class Type>
Container<Type>& Container<Type>::operator+=(const Container& ld)
{
	for (auto it = ld.begin(); it != ld.end(); ++it)
		push_back(*it);
	return *this;
}

template <class Type>
Container<Type> Container<Type>::operator+(const Container& ld)
{
	return *this += ld;
}

template <class Type>
string Container<Type>::getInternalInfo() const
{
	return "Length = " + to_string(_length) + "\r\n";
}

template <class Type>
bool Container<Type>::operator==(const Container& ld) const
{
	if (size() != ld.size()) return false;

	auto ldBgn = ld.begin();
	auto thisBegin = begin();

	while (ldBgn != ld.end() || thisBegin != end())
	{
		if (*ldBgn != *thisBegin) return false;
		++ldBgn;
		++thisBegin;
	}
	return true;
}

template <class Type>
bool Container<Type>::operator!=(const Container& ld) const
{
	return !operator==(ld);
}

template <class Type>
typename Container<Type>::DequeConstIterator  Container<Type>::begin() const
{
	return DequeConstIterator(_head);
}

template <class Type>
typename Container<Type>::DequeConstIterator Container<Type>::end() const
{
	return DequeConstIterator(_tail);
}



template <class Type>
void Container<Type>::push_back(const Type& val)
{
	if (isEmpty())
	{
		_head = new Node(val);
		_length = 1;
		_tail = new Node;
		_head->next = _tail;
		_tail->prev = _head;
		return;
	}

	Node* lastValid = _tail->prev;
	Node* emptyNode = _tail;

	Node* actualnode = new Node(val);
	actualnode->next = emptyNode;
	actualnode->prev = lastValid;

	lastValid->next = actualnode;

	_tail = emptyNode;
	_tail->prev = actualnode;
	++_length;
}

template <class Type>
void Container<Type>::push_front(const Type& val)
{
	if (isEmpty())
	{
		_head = new Node(val);
		_length = 1;
		_tail = new Node;
		_head->next = _tail;
		_tail->prev = _head;
		return;
	}

	Node* newHead = new Node(val);
	newHead->next = _head;
	_head->prev = newHead;
	_head = newHead;
	++_length;
}

template <class Type>
Type Container<Type>::pop_front()
{
	if (isEmpty()) throw std::out_of_range("Deque is empty");
	Node* oldHead = _head;
	_head = _head->next;
	delete oldHead;
	_head->prev = nullptr;
	--_length;
	return *begin();
}

template <class Type>
Type Container<Type>::pop_back()
{
	if (isEmpty()) throw std::out_of_range("Deque is empty");
	Node* oldTail = _tail;
	_tail = _tail->prev;
	_tail->next = nullptr;
	delete oldTail;
	--_length;
	return *end();
}

template <class Type>
Type Container<Type>::front() const
{
	if (isEmpty()) throw std::out_of_range("empty deque");
	return *begin();
}

template <class Type>
Type Container<Type>::back() const
{
	if (isEmpty()) throw std::out_of_range("empty deque");
	return _tail->prev->value;
}


template <class Type>
void Container<Type>::clear()
{
	if (isEmpty()) return;

	do
	{
		Node* src = _head->next;
		delete _head;
		_head = src;
	} while (_head != _tail);

	delete _head;
	_length = 0;
}

template <class Type>
size_t Container<Type>::size() const
{
	return _length;
}

template <class Type>
bool Container<Type>::isEmpty() const
{
	return size() == 0 ? true : false;
}

template<class Type>
string Container<Type>::contentToString() const
{
	ostringstream oss;
	oss << *this;
	return oss.str();
}

template<class Type>
Type Container<Type>::getMin()
{
	MinVisitor<Type> vis;
	this->accept(vis);
	return vis.GetData();
}

template<class Type>
Type Container<Type>::getMax()
{
	MaxVisitor<Type> maxVisitor;
	this->accept(maxVisitor);
	return maxVisitor.GetData();
}

template <class Type>
void Container<Type>::accept(BaseVisitor<Type>& vis)
{
	vis.visit(*this);
}

template<class U>
ostream& operator<<(ostream& o, const Container<U>& ld)
{
	for (auto it = ld.begin(); it != ld.end(); ++it)
		o << *it << " ";
	return o;
}

template<class U>
istream& operator >> (istream& i, Container<U>& ld)
{
	string s;
	getline(i, s);
	ld = (s != "") ? Container<U>(s) : Container<U>();
	return i;
}
