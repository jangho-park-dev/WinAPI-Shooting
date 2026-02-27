#include "Renderer.h"

Renderer::Renderer(HWND hWnd)
	: m_hWnd(hWnd), m_hdc(nullptr),
	m_hMemDC(nullptr), m_hBackBitmap(nullptr), m_hOldBitmap(nullptr)
{
	HDC hdc = GetDC(hWnd);

	GetClientRect(hWnd, &m_clientRect);

	m_hMemDC = CreateCompatibleDC(hdc);

	m_hBackBitmap = CreateCompatibleBitmap(
		hdc,
		m_clientRect.right,
		m_clientRect.bottom
	);

	m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBackBitmap);

	ReleaseDC(hWnd, hdc);
}

Renderer::~Renderer()
{
	if (m_hMemDC)
	{
		SelectObject(m_hMemDC, m_hOldBitmap);
		DeleteObject(m_hBackBitmap);
		DeleteDC(m_hMemDC);
	}
}

void Renderer::DrawSprite(
	const Sprite& sprite,				// 스프라이트 정보
	int x, int y,								// 화면에 그릴 시작 위치
	int srcX, int srcY,					// 스프라이트 시트에서 가져올 시작 위치
	int width, int height,				// 그릴 영역 크기
	int destWidth, int destHeight	// 화면에 그릴 크기(확대/축소)
)
{
	if (width < 0) width = sprite.GetWidth();
	if (height < 0) height = sprite.GetHeight();
	
	// 기본값은 원본 크기
	if (destWidth < 0)  destWidth = width;
	if (destHeight < 0) destHeight = height;

	// 메모리 DC(m_hMemDC)와 호환되는 임시 DC 생성
	// 비트맵을 선택해서 소스 DC로 사용하기 위함
	HDC hSrcDC = CreateCompatibleDC(m_hMemDC);

	// 스프라이트가 가진 HBITMAP을 방금 만든 소스 DC에 선택
	// 기존에 선택돼 있던 비트맵은 old에 저장
	HBITMAP old = (HBITMAP)SelectObject(hSrcDC, sprite.GetBitmap());

	// 확대/축소 시 픽셀을 단순 복사하여 처리하도록 설정
	SetStretchBltMode(m_hMemDC, COLORONCOLOR);

	// 스프라이트가 컬러키 투명도를 사용하는 경우
	if (sprite.UseColorKey())
	{
		// 지정한 색(color key)을 투명하게 처리하여 복사
		TransparentBlt(
			m_hMemDC,						// 목적지 DC(백버퍼)
			x, y,									// 화면에 그릴 시작 위치
			destWidth, destHeight,		// 목적지 크기(확대/축소)
			hSrcDC,								// 소스 DC(스프라이트 비트맵)
			srcX, srcY,							// 스프라이트 시트에서 가져올 시작 위치
			width, height,						// 원본 크기
			sprite.GetColorKey()			// 투명하게 처리할 색
		);
	}
	else
	{
		// 단순 복사하여 그리기(투명도 없음)
		StretchBlt(
			m_hMemDC,						// 목적지 DC(백버퍼)
			x, y,									// 화면에 그릴 시작 위치
			destWidth, destHeight,		// 목적지 크기(확대/축소)
			hSrcDC,								// 소스 DC(스프라이트 비트맵)
			srcX, srcY,							// 스프라이트 시트에서 가져올 시작 위치
			width, height,						// 원본 크기
			SRCCOPY							// 복사 방식(단순 복사 연산)
		);
	}

	// 소스 DC에 원래 선택돼 있던 비트맵 복원
	SelectObject(hSrcDC, old);

	// 소스 DC 삭제
	DeleteDC(hSrcDC);
}

void Renderer::BeginRender()
{
	// 백버퍼 검은색으로 밀기
	PatBlt(
		m_hMemDC,
		0, 0,
		m_clientRect.right,
		m_clientRect.bottom,
		BLACKNESS
	);
}

void Renderer::EndRender()
{
	m_hdc = GetDC(m_hWnd);

	// 백버퍼 내용을 현재 버퍼로(m_hMemDC -> m_hdc)
	BitBlt(
		m_hdc,
		0, 0,
		m_clientRect.right,
		m_clientRect.bottom,
		m_hMemDC,
		0, 0,
		SRCCOPY
	);

	ReleaseDC(m_hWnd, m_hdc);
}
