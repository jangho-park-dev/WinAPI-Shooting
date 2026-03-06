#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include "GameWorld.h"

class GameScene : public Scene
{
private:
	SceneManager* m_sceneManager;
	HWND				m_hWnd;
	GameWorld*		m_world;

public:
	GameScene(SceneManager* sceneManager, HWND hWnd);
	~GameScene();

	void Initialize() override;
	void Update(RECT& client, float deltaTime) override;
	void Render(Renderer& renderer) override;
	void HandleInput(float deltaTime) override;
};

