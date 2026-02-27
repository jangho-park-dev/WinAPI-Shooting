#pragma once
#include "framework.h"

class Sprite
{
private:
	HBITMAP m_hBitmap;
	int m_nWidth;
	int m_nHeight;

	bool m_bUseColorKey;
	COLORREF m_colorKey;

	float m_fSpriteSizeMultiplier;

public:
	Sprite();
	Sprite(const wchar_t* path, bool useColorKey = false, COLORREF key = RGB(255, 0, 255));
	~Sprite();

	HBITMAP GetBitmap() const { return m_hBitmap; }
	int GetWidth() const { return m_nWidth; }
	int GetHeight() const { return m_nHeight; }
	bool UseColorKey() const { return m_bUseColorKey; }
	COLORREF GetColorKey() const { return m_colorKey; }
	float GetSpriteSizeMultiplier() const { return m_fSpriteSizeMultiplier; }

	void SetSpriteSizeMultiplier(float multiplier) { m_fSpriteSizeMultiplier = multiplier; }

	Sprite* operator=(const Sprite* other);
};
