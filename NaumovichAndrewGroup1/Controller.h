#include <windows.h>
#include <memory>
#include "ActionResult.h"
#include "Model.h"
using namespace std;

class Controller
{
	std::shared_ptr<Model> _modelPtr;

	template<class T>
	string initListToString(const std::initializer_list<T>& lst);

	string trim(const string&);
public:
	Controller();

	ActionResult GetModelState()const;
	ActionResult OnPushBackButtonClick(const string& str);
	ActionResult OnPushFrontButtonClick(const string& str);

	ActionResult OnClearButtonClick();
	ActionResult OnInitButtonClick(const string& inp);

	ActionResult OnPopBackButtonClick();
	ActionResult OnPopFrontButtonClick();

	ActionResult OnVisitorButtonClick();
};

template <class T>
string Controller::initListToString(const std::initializer_list<T>& lst)
{
	ostringstream oss("");
	for (const auto& elem : lst)
		oss << elem << " ";
	return oss.str();
}
