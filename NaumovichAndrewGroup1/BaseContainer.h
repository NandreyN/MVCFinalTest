#pragma once
#include "BaseVisitor.h"
template <class Type>
class BaseContainer
{
public:
	virtual void accept(BaseVisitor<Type>&) = 0;
};