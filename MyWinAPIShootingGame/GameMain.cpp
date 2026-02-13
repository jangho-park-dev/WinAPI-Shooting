#include "GameMain.h"


GameMain::GameMain(HINSTANCE hInstance)
	: m_hInstance(hInstance),
	m_hWnd(nullptr),
	m_renderer(nullptr),
	m_resourceManager(nullptr),
	m_world(nullptr),
	m_isRunning(true)
{
}

GameMain::~GameMain()
{
	delete m_world;
	delete m_renderer;
	delete m_resourceManager;
}

bool GameMain::Initialize(int nCmdShow)
{
	RECT rect = { 0, 0, 480, 640 };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// 디버깅용 콘솔 창 할당
	if (AllocConsole())
	{
		FILE* pCout;
		freopen_s(&pCout, "CONOUT$", "w", stdout);
		freopen_s(&pCout, "CONOUT$", "w", stderr);
		std::clog.clear();
		std::cout.clear();
	}

	WNDCLASS wc = {};
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = GameMain::WndProc;
	wc.hInstance = m_hInstance;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = nullptr;
	wc.lpszClassName = L"Shooting Game";
	RegisterClass(&wc);

	m_hWnd = CreateWindow(
		L"Shooting Game",
		L"WinAPI Game",
		WS_OVERLAPPEDWINDOW,
		500, 100,
		rect.right - rect.left,
		rect.bottom - rect.top,
		nullptr, nullptr,
		m_hInstance, 
		this
	);

	if (!m_hWnd)
		return false;

	ShowWindow(m_hWnd, nCmdShow);
	UpdateWindow(m_hWnd);

	m_timer.Initialize();

	m_renderer = new Renderer(m_hWnd);
	m_resourceManager = new ResourceManager();
	m_world = new GameWorld();

	m_world->Initialize();

	return true;
}

void GameMain::Run()
{
	RECT client;
	GetClientRect(m_hWnd, &client);

	while (m_isRunning)
	{
		m_isRunning = HandleMessage();
		if (!m_isRunning)	break;

		m_timer.Tick();
		float deltaTime = m_timer.GetDeltaTime();

		HandleInput(deltaTime);

		m_renderer->BeginRender();
		m_world->Update(client, deltaTime);
		m_world->Render(*m_renderer);
		m_renderer->EndRender();
	}
}

bool GameMain::HandleMessage()
{
	MSG msg{};

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			return false;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return true;
}

void GameMain::HandleInput(float deltaTime)
{
	m_world->HandleInput(deltaTime);
}

LRESULT CALLBACK GameMain::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// game으로 GameMain에 접근하자
	GameMain* game = (GameMain*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	
	switch (msg)
	{
	case WM_NCCREATE:
	{
		CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
		SetWindowLongPtr(
			hWnd,
			GWLP_USERDATA,
			(LONG_PTR)cs->lpCreateParams
		);
		return TRUE;
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;
	}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}



