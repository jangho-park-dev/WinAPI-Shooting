#pragma once

#include "resource.h"
#include "framework.h"
#include "Timer.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameWorld.h"
#include "Player.h"

class GameMain
{
private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;

	Timer m_timer;
	Renderer* m_renderer;
	ResourceManager* m_resourceManager;
	GameWorld* m_world;

	bool m_isRunning;

public:
	GameMain(HINSTANCE hInstance);
	~GameMain();

	bool Initialize(int nCmdShow);
	void Run();
	bool HandleMessage();
	void HandleInput(float deltaTime);
	
	
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
