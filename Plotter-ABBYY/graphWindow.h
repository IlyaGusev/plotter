﻿#pragma once
#include <Windows.h>
#include <gdiplus.h>

#include "GP.h"

struct Polygon4Wrap {
	Gdiplus::PointF poly[4];
};

class GraphWindow {
public:
	GraphWindow( int width, int height, const wchar_t* formulaPath, bool is2D = false );

	static bool RegisterClass(HINSTANCE hInstance);

	bool Create(HINSTANCE hInstance, int nCmdShow);

	void Show();

	HWND GetHandle();

protected:
	void OnKeyDown( WPARAM wParam );
	void OnMouseWheel( WPARAM wParam );
	void OnMouseMove( WPARAM wParam, int x, int y );
	void OnLButtonDown( int xMousePos, int yMousePos );

	void OnPaint();
	void OnClose();
	void OnDestroy();

private:
	const static wchar_t* nameClassWindow;
	const static wchar_t* nameWindow;

	int prevMousePosX;
	int prevMousePosY;

	int windowWidth;
	int windowHeight;

	int cmdShow;
	HWND handle;
	
	GP graphInPoints;

	void drawGraph(HDC dc);
	void drawAxes(HDC dc);
	// red for max and blue for min by default
	void fillWithGradient(HDC dc, Gdiplus::Color maxColor = Gdiplus::Color(128, 255, 0, 0), Gdiplus::Color minColor = Gdiplus::Color(128, 0, 0, 255));

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

	void getMaxMinZAndRelativeGridKnots( double& min, double& max, int& xMin, int& yMin, int& xMax, int& yMax );
	void getAllPolygonsOfGrid(std::vector< Polygon4Wrap > &polygons, std::vector< std::vector < std::pair<double, double> > > &points);
};

