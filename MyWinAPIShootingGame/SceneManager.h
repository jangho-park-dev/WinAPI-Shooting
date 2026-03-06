#pragma once
#include "Scene.h"

class SceneManager
{
private:
	Scene* m_currentScene;
	Scene* m_nextScene;

public:
	SceneManager();
	~SceneManager();

	void ChangeScene(Scene* nextScene);
	void Update(RECT& client, float deltaTime);
	void Render(Renderer& renderer);
	void HandleInput(float deltaTime);

	Scene* GetCurrentScene() const { return m_currentScene; }
};

