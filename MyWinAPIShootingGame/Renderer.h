#pragma once
#include <windows.h>
#include <wingdi.h>
#include "Sprite.h"
#pragma comment(lib, "Msimg32.lib")

struct MyRectF
{
	float x;
	float y;
	float width;
	float height;
};


class Renderer
{
private:
	HWND m_hWnd;
	HDC m_hdc;					// 화면 DC
	HDC m_hMemDC;			// 백버퍼 DC
	HBITMAP m_hBackBitmap;
	HBITMAP m_hOldBitmap;
	RECT m_clientRect;

public:
	Renderer(HWND hwnd);
	~Renderer();

	void BeginRender();	// 프레임 시작
	void EndRender();		// 프레임 종료
	void DrawSprite(
		const Sprite& sprite,
		int x, int y,
		int srcX = 0, int srcY = 0,
		int width = -1, int height = -1
	);

};

