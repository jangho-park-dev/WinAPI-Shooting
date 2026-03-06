#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include "Background.h"
#include "Button.h"

class GameOverScene : public Scene
{
private:
	SceneManager*	m_sceneManager;
	HWND				m_hWnd;
	Background*		m_background;
	Button*				m_buttonHome;
	Button*				m_buttonRestart;
	
public:
	GameOverScene(SceneManager* sceneManager, HWND hWnd);
	~GameOverScene();

	void Initialize() override;
	void Update(RECT& client, float deltaTime) override;
	void Render(Renderer& renderer) override;
	void HandleInput(float deltaTime) override;
};

