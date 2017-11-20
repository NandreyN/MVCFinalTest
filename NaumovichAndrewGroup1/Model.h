#pragma once
#include <vector>
#include "Container.h"

using namespace std;
class Model
{
	vector<double> parseString(const string&) const;
	bool isStringNormal(const string&) const;
	Container<double> _list;
public:
	Model();
	Model(const Container<double>&);
	void setContainer(const Container<double>&);
	void setContainer(const string&);


	void clear();
	void push_back_string(const string&);
	void push_front_string(const string&);
	int pop_front();
	int pop_back();

	string toString();
	~Model();
};

