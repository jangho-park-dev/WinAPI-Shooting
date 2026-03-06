#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include "Background.h"
#include "Button.h"


class TitleScene : public Scene
{
private:
	SceneManager* m_sceneManager;
	HWND				m_hWnd;
	Background*		m_background;
	Button*				m_buttonStart;
	Button*				m_buttonExit;

public:
	TitleScene(SceneManager* sceneManager, HWND hWnd);
	~TitleScene();

	void Initialize() override;
	void Update(RECT& client, float deltaTime) override;
	void Render(Renderer& renderer) override;
	void HandleInput(float deltaTime) override;
};

