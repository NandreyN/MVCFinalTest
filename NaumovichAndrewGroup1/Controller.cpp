#include "Controller.h"
#include <map> 
bool isStringOK(const string& s)
{
	if (s.empty()) return false;
	for (char c : s)
		if (!isdigit(c) && c != ' ' && c!='-' && c!='.') return false;
	return true;
}

string Controller::trim(const string & str)
{
	size_t first = str.find_first_not_of(' ');
	if (string::npos == first)
	{
		return str;
	}
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, (last - first + 1));

}

Controller::Controller()
{
	_modelPtr = shared_ptr<Model>(new Model(Container<double>({1,2,3})));
}

ActionResult Controller::GetModelState() const
{
	return ActionResult(true, _modelPtr->toString());
}

ActionResult Controller::OnPushBackButtonClick(const string & str)
{
	if (!isStringOK(str)) return ActionResult(false, "");
	_modelPtr->push_back_string(str);
	return ActionResult(true,_modelPtr->toString());
}

ActionResult Controller::OnPushFrontButtonClick(const string & str)
{
	if (!isStringOK(str)) return ActionResult(false, "");
	_modelPtr->push_front_string(str);
	return ActionResult(true, _modelPtr->toString());
}

ActionResult Controller::OnClearButtonClick()
{
	_modelPtr->clear();
	return ActionResult(true, _modelPtr->toString());
}

ActionResult Controller::OnInitButtonClick(const string & inp)
{
	if (!isStringOK(inp))
		return ActionResult(false,"");
	_modelPtr->setContainer(trim(inp));

	return ActionResult(true, _modelPtr->toString());
}

ActionResult Controller::OnPopBackButtonClick()
{
	try
	{
		_modelPtr->pop_back();
	}
	catch (std::out_of_range ex)
	{
		return  ActionResult(true, _modelPtr->toString(), true, "Empty Deque");
	}
	return ActionResult(true, _modelPtr->toString());
}

ActionResult Controller::OnPopFrontButtonClick()
{
	try
	{
		_modelPtr->pop_front();
	}
	catch (std::out_of_range ex)
	{
		return  ActionResult(true, _modelPtr->toString(), true, "Empty Deque");
	}
	return ActionResult(true, _modelPtr->toString());
}

ActionResult Controller::OnVisitorButtonClick()
{
	return ActionResult(false , "");
}
