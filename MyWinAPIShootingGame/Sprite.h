#pragma once
#include "framework.h"

class Sprite
{
public:
	Sprite();
	Sprite(const wchar_t* path, bool useColorKey = false, COLORREF key = RGB(255, 0, 255));
	~Sprite();

	HBITMAP GetBitmap() const { return m_hBitmap; }
	int GetWidth() const { return m_nWidth; }
	int GetHeight() const { return m_nHeight; }

	bool UseColorKey() const { return m_bUseColorKey; }
	COLORREF GetColorKey() const { return m_colorKey; }

	Sprite* operator=(const Sprite* other);

private:
	HBITMAP m_hBitmap;
	int m_nWidth;
	int m_nHeight;

	bool m_bUseColorKey;
	COLORREF m_colorKey;
};
