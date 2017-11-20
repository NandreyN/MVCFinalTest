#include "ActionResult.h"

ActionResult::ActionResult()
{
	Update = false;
	Data = "";
	Error = false;
	ErrorMessage = "";
}

ActionResult::ActionResult(bool f, const string& s, bool error, const string& errMsg)
{
	Update = f;

	if (Update)
		Data = s;

	Error = error;
	ErrorMessage = errMsg;
}