#pragma once
//#include "BaseVisitor.h"
#include "Container.h"
#include <string>
#include "MinMaxVisitor.h"
using namespace std;

template <class T>
class ToStringVisitor : public BaseVisitor<T>
{
private:
	string result;
public:
	ToStringVisitor() { result = ""; };
	string getString() const { return result; };
	void visit(Container<T>& container)  override
	{
		std::pair<T, T> numeric(container.getMin(), container.getMax());

		result = std::to_string(numeric.first) + " " + std::to_string(numeric.second);
	}
};