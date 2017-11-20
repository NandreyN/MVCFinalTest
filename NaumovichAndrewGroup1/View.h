#pragma once
#include <windows.h>
#include <string>
#include <map>
#include <memory>
#include "Controller.h"

#pragma region 

#pragma region Defines

#pragma region Inputs
#define IDC_PUSHBACKINPUT 1002
#define IDC_PUSHFRONTINPUT 1003
#define IDC_INITINPUT 1008
#pragma endregion Inputs

#pragma region Buttons
#define IDC_POPBACKBUTTON 1006
#define IDC_POPFRONTBUTTON 1007
#define IDC_PUSHBACKBUTTON 1004
#define IDC_PUSHFRONTBUTTON 1005
#define IDC_INITBUTTON 1009
#define IDC_CLEARBUTTON 1011
#define IDC_SUMBUTTON 1012
#pragma endregion Buttons

#define IDD_DIALOG 101
#define IDC_CONTENT 1001
#define IDC_ERROR 1010
#pragma endregion



/*
View class manipulates only output to user
via special methods
*/
class View
{
private:
	HWND _handle;
	std::map<std::string, int> _commandDictionary;
	std::shared_ptr<Controller> _controllerPtr;
	void update(const ActionResult&);
	string getInput(HWND hwnd, int elementID) const;
	void UIUpdate(std::string commandStr, int controlID, std::string data);
public:
	LRESULT mainProc(HWND hwmd, UINT message, WPARAM wParam, LPARAM lParam);
	BOOL dlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	View(HINSTANCE hinstance, HINSTANCE prevHinstance, LPSTR lpCmdLine, int nCmdShow);
	int run();
};

inline int View::run()
{
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

map<HWND, View*> windowMap;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	std::map<HWND, View *>::iterator it = windowMap.find(hWnd);
	if (it != windowMap.end())
		return it->second->mainProc(hWnd, message, wParam, lParam);
	return DefWindowProc(hWnd, message, wParam, lParam);;
}

LRESULT CALLBACK DlgCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	std::map<HWND, View *>::iterator it = windowMap.find(hWnd);
	if (it != windowMap.end())
		return it->second->dlgProc(hWnd, message, wParam, lParam);
	return 0;
}

LRESULT View::mainProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND dlgHWND;
	switch (message)
	{
	case WM_CREATE:
		dlgHWND = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG), hwnd, (DLGPROC)DlgCallback);
		windowMap[dlgHWND] = this;
		ShowWindow(dlgHWND, SW_SHOW);
		SendMessage(dlgHWND, WM_INITDIALOG, 0, 0);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return FALSE;
}

inline void View::update(const ActionResult &ar)
{
	if (ar.Update) UIUpdate("SetWindowText", IDC_CONTENT, ar.Data.data());

	if (ar.Error) UIUpdate("SetWindowText", IDC_ERROR, ar.ErrorMessage.data());
}

string View::getInput(HWND hwnd, int elementID) const
{
	char buffer[1024];
	GetWindowText(GetDlgItem(hwnd, elementID), buffer, 1024);
	return string(buffer);
}

BOOL View::dlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static ActionResult ar;
	static ActionResult srcAr;
	switch (message)
	{
	case WM_INITDIALOG:
		_handle = hwnd;
		_controllerPtr = std::shared_ptr<Controller>(new Controller());
		ar = _controllerPtr->GetModelState();
		SetWindowText(GetDlgItem(_handle, IDC_CONTENT), ar.Data.data());
		SetWindowText(GetDlgItem(_handle, IDC_ERROR), "");
		break;
	case WM_COMMAND:
		if (HIWORD(wParam) == EN_SETFOCUS && LOWORD(wParam) == IDC_INITINPUT)
		{
			SetWindowText(GetDlgItem(_handle, IDC_INITINPUT), "");
		}

		if (HIWORD(wParam) == EN_SETFOCUS && LOWORD(wParam) == IDC_PUSHBACKINPUT)
		{
			SetWindowText(GetDlgItem(_handle, IDC_PUSHBACKINPUT), "");
		}

		if (HIWORD(wParam) == EN_SETFOCUS && LOWORD(wParam) == IDC_PUSHFRONTINPUT)
		{
			SetWindowText(GetDlgItem(_handle, IDC_PUSHFRONTINPUT), "");
		}

		switch (LOWORD(wParam))
		{
		case IDC_INITBUTTON:
		{
			ar = _controllerPtr->OnInitButtonClick(getInput(_handle, IDC_INITINPUT));
			UIUpdate("DisableWindow", IDC_INITINPUT, "");
			UIUpdate("DisableWindow", IDC_INITBUTTON, "");
			break;
		}

		case IDC_PUSHBACKBUTTON:
		{
			ar = _controllerPtr->OnPushBackButtonClick(getInput(_handle, IDC_PUSHBACKINPUT));
			break;
		}

		case IDC_PUSHFRONTBUTTON:
		{
			ar = _controllerPtr->OnPushFrontButtonClick(getInput(_handle, IDC_PUSHFRONTINPUT));
			break;
		}
		
		case IDC_POPBACKBUTTON:
		{
			ar = _controllerPtr->OnPopBackButtonClick();
			break;
		}

		case IDC_POPFRONTBUTTON:
		{
			ar = _controllerPtr->OnPopFrontButtonClick();
			break;
		}

		case IDC_CLEARBUTTON:
		{
			ar = _controllerPtr->OnClearButtonClick();
			break;
		}

		case IDOK:
		{
			SendMessage(GetParent(_handle), WM_CLOSE, 0, 0);
			EndDialog(_handle, 0);
			break;
		}

		case IDC_SUMBUTTON:
		{
			srcAr = _controllerPtr->OnVisitorButtonClick();
			MessageBox(NULL, srcAr.Data.data(), "Sum", MB_OK);
			break;
		}

		}
		update(ar);
		break;
	}


	return FALSE;
}

inline View::View(HINSTANCE hinstance, HINSTANCE prevHinstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wndclass;
	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hinstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = "UICONTAINER";
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wndclass))
	{
		throw "";
	}

	HWND hwnd;
	hwnd = CreateWindow(
		(LPCSTR)"UICONTAINER",
		(LPCSTR)"UICONTAINER",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		NULL,
		NULL,
		hinstance,
		NULL);


	if (!hwnd || hwnd == NULL)
	{
		throw "Exception";
	}
	windowMap[hwnd] = this;
	SendMessage(hwnd, WM_CREATE, 0, 0);

	_commandDictionary["SetWindowText"] = 0;
	_commandDictionary["DisableWindow"] = 1;
	_commandDictionary["EnableWindow"] = 2;
	_commandDictionary["UpdateDequeStateMessage"] = 3;
	_commandDictionary["MessageBox"] = 4;
}

inline void View::UIUpdate(std::string commandStr, int controlID, std::string data)
{
	if (_commandDictionary.find(commandStr) == _commandDictionary.end()) return;
	switch (_commandDictionary[commandStr])
	{
	case 0:
		SetWindowText(GetDlgItem(_handle, controlID), data.data());
		break;
	case 1:
		EnableWindow(GetDlgItem(_handle, controlID), false);
		break;
	case 2:
		EnableWindow(GetDlgItem(_handle, controlID), true);
		break;
	case 3:
		UIUpdate("SetWindowText", IDC_CONTENT, data);
		break;
	case 4:
		MessageBox(_handle, data.data(), "Info", MB_OK);
		break;
	}
}