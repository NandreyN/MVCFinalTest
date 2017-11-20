#pragma once
template <class T>
class Container;

template <class T>
class BaseVisitor
{
	public:
	virtual void visit(Container<T>&) = 0;

	virtual ~BaseVisitor() = default;
};