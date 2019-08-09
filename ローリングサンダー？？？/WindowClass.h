#pragma once
#include <memory>
#define NOMINMAX
#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

//base on https://qiita.com/seekerkrt/items/2478c05c590c308146fd
//i allow to he use to the this project code.but in he's own risk.
//and he can drop this talk. i am alrady done...

class Window {
public:
	class EventHandler;
	typedef std::shared_ptr<EventHandler> SharedEventHandler;
public:
	Window();
	virtual ~Window();
public:	
	static const int MAX_LOADSTRING = 100;
	bool Create(HINSTANCE hInstance, const TCHAR (class_name)[],const TCHAR (app_name)[], int width, int height);
	bool IsCreated();   //�E�B���h�E�������i�����j����Ă��邩�m�F
	HWND GetHWND() { return hWnd; }
	bool UpdateWindow();
	bool ShowWindow(int nCmdShow);
	void SetTitle(const TCHAR title[]);
	WPARAM GetMessageLoop(UINT wMsgFilterMin, UINT wMsgFilterMax);  //�ėp����Ԃ̃��b�Z�[�W���[�v
	WPARAM PeekMessageLoop(UINT wMsgFilterMin, UINT wMsgFilterMax,/* UINT wRemoveMSG,*/ UINT NanoSleep);    //����p�r���b�Z�[�W���[�v


	bool AddStyle(LONG In) {
		LONG lStyle;
		lStyle = GetWindowLong(hWnd, GWL_STYLE);
		lStyle |= In;
		lStyle = SetWindowLong(hWnd, GWL_STYLE, lStyle);
		return true;
	}
	bool SubStyle(LONG In) {
		LONG lStyle;
		lStyle = GetWindowLong(hWnd, GWL_STYLE);
		lStyle &= ~In;
		lStyle = SetWindowLong(hWnd, GWL_STYLE, lStyle);
		return true;
	}

	bool SetClientArea(int W, int H) {
		RECT wrt = { 0, };
		RECT crt = { 0, };
		GetWindowRect(hWnd, &wrt);
		GetClientRect(hWnd, &crt);

		int BW = (wrt.right - wrt.left) - (crt.right-crt.left);
		int BH = (wrt.bottom - wrt.top) - (crt.bottom-crt.top);

		return TRUE == MoveWindow(hWnd, wrt.left, wrt.top, BW + W, BH + H, TRUE);
		
		
	}

	class EventHandler {
	public:
		virtual void InnerPeekMessage();//PeekMessageLoop�ŌĂ΂��
		virtual LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp); //�f�t�H���g�ł�DefWndProc���ĂԂ�WM_DESTORY���������邮�炢
		virtual ~EventHandler() {}
	};
	
	bool SetEventHandler(SharedEventHandler In);
	

public: //�J�X�^�}�C�Y���郁�\�b�h
	virtual void InnerPeekMessage();//PeekMessageLoop�ŌĂ΂��
	//virtual LRESULT LocalWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp); //�f�t�H���g�ł�DefWndProc���ĂԂ�WM_DESTORY���������邮�炢

private:
	static LRESULT CALLBACK BaseWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp); //���b�v���邽�߂̃g���b�N�Ȋ֐�
	void SetPointer(HWND hWnd); //window�I�u�W�F�N�g(this�|�C���^)�ƃE�B���h�E�n���h�����֘A�t���邽�߂̃g���b�N�Ȋ֐�

protected:
	volatile HWND hWnd = nullptr;
	SharedEventHandler EH = std::make_shared<EventHandler>();
};
