#pragma once
#include <memory>
#include <string>
using namespace std;
class ActionResult
{
public:
	bool Update;
	string Data;
	string ErrorMessage;
	bool Error;

	ActionResult();
	ActionResult(bool,const string&, bool = false, const string& = "");
};