#include <thread>
#include <chrono>
#include "WindowClass.h"

//�R���X�g���N�^
Window::Window() {

}
//�f�X�g���N�^
Window::~Window() {

}

bool Window::Create(HINSTANCE hInstance,const TCHAR class_name[],const TCHAR app_name[], int width, int height) {
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(wcex));

	wcex.cbSize = sizeof(WNDCLASSEX);   //���̍\���̎��̂̃T�C�Y�Ȃ̂ŁB
	wcex.cbClsExtra = 0;    //�g���p�H�Ƃ肠�����g��Ȃ��̂�0
	wcex.cbWndExtra = 0;    //�g���p�H�Ƃ肠�����g��Ȃ��̂�0
	wcex.lpfnWndProc = Window::BaseWndProc;//���̃E�B���h�E�N���X�̃v���V�[�W�����w��B���̃\�[�X�R�[�h�ł͂�������
	wcex.hInstance = hInstance;
	wcex.lpszClassName = class_name;
	wcex.lpszMenuName = nullptr;    //���j���[�̖��O�B�g��Ȃ��Ȃ�Ƃ肠����NULL��OK
	wcex.style = CS_VREDRAW | CS_HREDRAW;
	wcex.hCursor = ::LoadCursor(hInstance, IDC_ARROW);      //�J�[�\���A�C�R��
	wcex.hIcon = ::LoadIcon(hInstance, IDI_APPLICATION);    //�v���O�����A�C�R��
	wcex.hIconSm = ::LoadIcon(hInstance, IDI_APPLICATION);  //�v���O�����A�C�R���i���j::�^�C�g���o�[�Ɏg�����H
	wcex.hbrBackground = GetStockBrush(GRAY_BRUSH);      //�N���C�A���g�̈�̓h��Ԃ��F�i�u���V�j
//���Ƃ��͓��ɁA���\�b�h�̈������󂯎���ăJ�X�^�}�C�Y�ł���悤�ɂ���̂��]�܂�������                                                 
			//ATOM��WNDCLASSEX�o�^
	if (!::RegisterClassEx(&wcex)) {
		return false;
	}
	//�E�B���h�E����
	HWND ret = ::CreateWindow(
		class_name,
		app_name,
		WS_OVERLAPPEDWINDOW,    //�E�B���h�E�X�^�C���B�Ƃ肠�����f�t�H���g�Ȋ�����
		CW_USEDEFAULT, CW_USEDEFAULT,   //�����ʒu�B�K���ɂ���Ă����B
		width, height,      //�E�B���h�E�T�C�Y
		nullptr,    //�e�E�B���h�E�̃n���h���B���ɂȂ���ō����NULL
		nullptr,    //���j���[�n���h���B���ɂȂ��̂ō����NULL
		hInstance,
		this    //�g���b�N�̊́BCreateParameter�ɐݒ�
	);
	//�E�B���h�E�����Ɏ��s�H
	if (ret == nullptr) {
		return false;
	}
	//���������̂�true��Ԃ��Ĕ�����
	return true;
}

bool Window::IsCreated()
{
	if (this->hWnd == nullptr) {
		return false;
	}
	else {
		return true;
	}
}

bool Window::UpdateWindow() {
	if (this->hWnd == nullptr) {
		return false;
	}
	::UpdateWindow(this->hWnd);
	return true;
}

bool Window::ShowWindow(int nCmdShow) {
	if (this->hWnd == nullptr) {
		return false;
	}
	::ShowWindow(this->hWnd, nCmdShow);
	return true;
}

void Window::SetTitle(const TCHAR title[MAX_LOADSTRING]) {
	::SetWindowText(hWnd, title);
}

WPARAM Window::GetMessageLoop(UINT wMsgFilterMin, UINT wMsgFilterMax) {
	MSG msg;
	while (::GetMessage(&msg, nullptr, wMsgFilterMin, wMsgFilterMax) > 0) {  //�u-1�v���Ԃ��Ă��邩������Ȃ��̂ł�������
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	this->hWnd = nullptr; //�R�R�ɋL�q���Ă����Ă݂�
	return msg.wParam;
}
WPARAM Window::PeekMessageLoop(UINT wMsgFilterMin, UINT wMsgFilterMax/*, UINT wRemoveMSG*/,UINT NanoSleep) {
	MSG msg;
	do {
		//����̏����̓v���V�[�W���ȂǂɔC���āc�c
		if (::PeekMessage(&msg, nullptr, wMsgFilterMin, wMsgFilterMax, PM_REMOVE)) {//if 2nd param not null to cant get WM_QUIT.
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		//�����łȂ������͎��O�ł���
		else {
			//���O�̏����ł���InnerPeekMessage()���Ăяo��
			this->InnerPeekMessage();
		}
		std::this_thread::sleep_for(std::chrono::nanoseconds(NanoSleep));
	} while (msg.message != WM_QUIT);//�C�x���g�h���u���������[�v�𔲂���܂Ŏ��s
	this->hWnd = nullptr; //�R�R�ɋL�q���Ă����Ă݂�
	return msg.wParam;
}

void Window::InnerPeekMessage() {
	//TODO:InnerPeekMessage()
	EH->InnerPeekMessage();
}
/** /
LRESULT Window::LocalWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
	case(WM_DESTROY):
		::PostQuitMessage(0);
		break;
	default:
		return ::DefWindowProc(hWnd, msg, wp, lp);
	}
	return 0;
}
/**/
LRESULT CALLBACK Window::BaseWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	//Window::SetPointer�ŃZ�b�g����this�|�C���^���擾
	Window* window = (Window*)(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
	//�擾�Ɏ��s���Ă�ꍇ
	if (window == nullptr) {
		//�����炭WM_CREATE�̍Œ��Ȃ̂�
		if (msg == WM_CREATE) {
			//CreateWindow�̃p�����[�^����擾����
			window = (Window*)(((LPCREATESTRUCT)lp)->lpCreateParams);
		}
		//���̏����Ŏ擾�ł��Ă�͂��Ȃ񂾂��ǁA�ꉞ�`�F�b�N���Ă���
		if (window != nullptr) {
			//window�I�u�W�F�N�g�ƃE�B���h�E�n���h�����֘A�t����
			window->SetPointer(hWnd);
		}
	}
	//�����擾�ł��ĂĂ�
	if (window != nullptr) {
		//���̃I�u�W�F�N�g�ɂ��E�B���h�E�v���V�[�W���������g���Ƃ������U�蕪����
		return window->EH->WndProc(hWnd, msg, wp, lp);
	}
	//�悭�킩��񂯂ǁA����ł��擾�ł����肵�ĂȂ��ꍇ�i�ȉ��̂悤�ɂ��������O����ē�����̂��悳����������
	else {
		return ::DefWindowProc(hWnd, msg, wp, lp);
	}
}
void Window::SetPointer(HWND hWnd) {
	::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	this->hWnd = hWnd;
}

bool Window::SetEventHandler(SharedEventHandler In) {
	this->EH = In;
	return true;
}
void Window::EventHandler::InnerPeekMessage() {
	//TODO:InnerPeekMessage()
	
}
LRESULT Window::EventHandler::WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
	case(WM_DESTROY):
		::PostQuitMessage(0);
		break;
	default:
		return ::DefWindowProc(hWnd, msg, wp, lp);
	}
	return 0;
}