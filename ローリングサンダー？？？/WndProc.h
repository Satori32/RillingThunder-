#pragma once
#include <cstddef>
#include <vector>
#include <thread>
#include <algorithm>
#include <tuple>
#include <random>

#include <Windows.h>
#include "WindowClass.h"

//#include "Rolling.h"

class RollingThunderProc : public Window::EventHandler {

	typedef std::tuple<double, double> Data;//Angle,DeltaAngle

	LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {

		switch (msg) {

		case WM_CREATE: {
			HWnd = hWnd;
			Data Dt{ ui(mt),ui(mt) };
			D.push_back(Dt);
			
			RECT rt;
			GetClientRect(hWnd, &rt);	

			PX=(rt.right - rt.left) / 2;
			PY=(rt.bottom - rt.top) / 2;
		}
			break;
		case WM_LBUTTONDOWN: {
			RECT rt = { 0, };
			Data Dt{ ui(mt),ui(mt)};
			D.push_back(Dt);
		}
			break;
		case WM_RBUTTONDOWN:
			if (D.size())D.pop_back();
			break;
		case WM_MOUSEMOVE:
			PX = GET_X_LPARAM(lp);
			PY = GET_Y_LPARAM(lp);
			break;

		case WM_PAINT:
		{			
			RECT rt;
			GetClientRect(hWnd, &rt);

			PAINTSTRUCT PS;			
			
			double X = (rt.right - rt.left) / 2;
			double Y = (rt.bottom - rt.top) / 2;
			double L = (std::min)(X, Y)/3;
			
			X = (std::max<double>)(0, ((std::min)(X*2, PX)));
			Y = (std::max<double>)(0, ((std::min)(Y*2, PY)));

			HDC hDc = BeginPaint(hWnd, &PS);			
			HPEN Pen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
			HPEN OldPen = SelectPen(hDc, Pen);
			float F = 1;
			double LP = 0.8;
			MoveToEx(hDc, X,Y, nullptr);
			TCHAR S[] = _T("Move Mouse on Window. Push LButton to Add. RButton to Sub.");
			TextOut(hDc, 0, 0, S, std::size(S)-1);
			for (std::size_t i = 0; i < D.size(); i++) {
				std::get<0>(D[i])+= std::get<1>(D[i]);
				Angle = F*std::fmod(std::get<0>(D[i]), 360.0);
				std::tie(X, Y) = Rot(Angle, L, X, Y);
				L *= LP;
				F *= -1;

				LineTo(hDc, X, Y);
				
			}

			SelectPen(hDc, OldPen);
			DeletePen(Pen);
			EndPaint(hWnd, &PS);			
			
			/** /

			if(D.size()==0)	return DefWindowProc(hWnd, msg, wp, lp);
			RECT rt;

			GetClientRect(hWnd, &rt);

			PAINTSTRUCT PS;
			
			HPEN Pen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
			
			HDC hDc = BeginPaint(hWnd, &PS);
			HPEN OldPen = SelectPen(hDc, Pen);
			MoveToEx(hDc, ( rt.right-rt.left) / 2, (rt.bottom - rt.top) / 2, nullptr);

			for (std::size_t i = 1; i < D.size(); i++) {
				LineTo(hDc, std::get<0>(D[i]), std::get<1>(D[i]));
			}


			SelectPen(hDc, OldPen);
			DeletePen(Pen);
			EndPaint(hWnd, &PS);


		/**/
		}
			break;

		case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;
		default:
			return DefWindowProc(hWnd, msg, wp, lp);
			break;
		}

		return 0;
	}
public:
	void InnerPeekMessage() {
		InvalidateRect(HWnd, nullptr, TRUE);
		std::this_thread::sleep_for(std::chrono::nanoseconds((1000*1000*1000)/60));
	}
protected:
	std::tuple<double, double> Rot(double Angle, double L, double X, double Y) {
		double Rad = 3.141592 / 180.0;
		double x = std::sin(Rad * Angle) * L+X;
		double y = std::cos(Rad * Angle) * L+Y;

		return{ x,y };
	}
protected:

	HWND HWnd = nullptr;

	std::random_device rd;
	std::mt19937 mt{rd()};
	std::uniform_real_distribution<double> ui{ 0.0, 7.0 };

	std::vector<Data> D;

	double Angle = 0;
	double PX = 0;
	double PY = 0;
};