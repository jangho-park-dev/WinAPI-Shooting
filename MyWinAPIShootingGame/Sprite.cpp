#include "Sprite.h"

Sprite::Sprite()
{
	m_hBitmap = nullptr;
	m_nWidth = 0;
	m_nHeight = 0;
	m_bUseColorKey = false;
	m_colorKey = 0;
}

Sprite::Sprite(const wchar_t* path, bool useColorKey, COLORREF key)
	: m_bUseColorKey(useColorKey), m_colorKey(key)
{
	m_hBitmap = (HBITMAP)LoadImage(
		nullptr,
		path,
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE
	);

	if (m_hBitmap == NULL)
		std::cout << "FAILED to load image: " << path << std::endl;
	else
	{
		std::cout << "SUCCESS to load image: " << path << std::endl;
	}

	BITMAP bmp;
	GetObjectW(m_hBitmap, sizeof(BITMAP), &bmp);
	m_nWidth = bmp.bmWidth;
	m_nHeight = bmp.bmHeight;
}

Sprite::~Sprite()
{
	DeleteObject(m_hBitmap);
}

Sprite* Sprite::operator=(const Sprite* other)
{
	m_hBitmap = other->GetBitmap();
	m_nWidth = other->GetWidth();
	m_nHeight = other->GetHeight();
	m_bUseColorKey = other->UseColorKey();
	m_colorKey = other->GetColorKey();

	return this;
}

