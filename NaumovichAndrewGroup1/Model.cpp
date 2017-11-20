#include "Model.h"
#include <sstream>
#include "ToStringVisitor.h"
vector<double> Model::parseString(const string & s) const
{
	vector<double> vect;
	istringstream oss(s);
	while (!oss.eof())
	{
		double elem;
		oss >> elem;
		vect.push_back(elem);
	}
	return vect;
}

bool Model::isStringNormal(const string & s) const
{
	if (s.empty()) return false;
	for (char c : s)
		if (!isdigit(c) && c != ' ') return false;
	return true;
}

Model::Model()
{
	_list = Container<double>();
}

Model::Model(const Container<double> & c)
{
	_list = c;
}

void Model::setContainer(const Container<double> & c)
{
	_list = c;
}

void Model::setContainer(const string & str)
{
	_list = Container<double>();
	push_back_string(str);
}

void Model::clear()
{
	_list.clear();
}

void Model::push_back_string(const string & str)
{
	auto vect = parseString(str);
	for (auto c : vect)
		_list.push_back(c);
}

void Model::push_front_string(const string & str)
{
	auto vect = parseString(str);
	for (auto c : vect)
		_list.push_front(c);
}

int Model::pop_front()
{
	return _list.pop_front();
}

int Model::pop_back()
{
	return _list.pop_back();
}


string Model::toString()
{
	if (_list.size() == 0) return "";
	string s = "";
	ToStringVisitor<double> vis;
	_list.accept(vis);
	s = vis.getString();
	return s + "; Content : " + _list.contentToString();
}

Model::~Model()
{
}
