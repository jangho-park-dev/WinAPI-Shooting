#pragma once
#include "Renderer.h"
#include "ResourceManager.h"

class Button
{
private:
	Sprite* m_buttonSprite;
	int m_nX, m_nY;
	int m_nWidth, m_nHeight;
	bool m_bHover;

public:
	Button(SpriteID spriteID, int x, int y, int width = 100, int height = 55);

	void Render(Renderer& renderer);
	bool IsClicked() const;
	bool IsClickedClient(HWND hWnd) const;

	void SetPosition(int x, int y) { m_nX = x; m_nY = y; }
	void SetSize(int width, int height) { m_nWidth = width; m_nHeight = height; }
	int GetX() const { return m_nX; }
	int GetY() const { return m_nY; }
	int GetWidth() const { return m_nWidth; }
	int GetHeight() const { return m_nHeight; }
};