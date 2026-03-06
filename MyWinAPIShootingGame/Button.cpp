#include "Button.h"

Button::Button(SpriteID spriteID, int x, int y, int width, int height)
	: m_nX(x)
	, m_nY(y)
	, m_nWidth(width)
	, m_nHeight(height)
	, m_bHover(false)
{
	m_buttonSprite = ResourceManager::GetInstance().GetSprite(spriteID);
}

void Button::Render(Renderer& renderer)
{
	renderer.DrawSprite(
		*m_buttonSprite,
		m_nX, m_nY,
		0, 0,
		m_buttonSprite->GetWidth(), m_buttonSprite->GetHeight(),
		m_nWidth, m_nHeight
	);
}

// 마우스 클릭 여부 확인(WM_LBUTTONDOWN 없이 폴링)
bool Button::IsClicked() const
{
	if (!(GetAsyncKeyState(VK_LBUTTON) & 0x0001))	return false;

	POINT point;
	GetCursorPos(&point);
	// 스크린 좌표 -> 클라이언트 좌표 변환은 씬에서 처리
	return
		point.x >= m_nX && point.x <= m_nX + m_nWidth &&
		point.y >= m_nY && point.y <= m_nY + m_nHeight;
}

// 클라이언트 좌표 기준 클릭 확인
bool Button::IsClickedClient(HWND hWnd) const
{
	if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000))  return false;

	POINT point;
	GetCursorPos(&point);
	ScreenToClient(hWnd, &point);

	return
		point.x >= m_nX && point.x <= m_nX + m_nWidth &&
		point.y >= m_nY && point.y <= m_nY + m_nHeight;
}

