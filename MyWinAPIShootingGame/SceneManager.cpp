#include "SceneManager.h"

SceneManager::SceneManager()
	: m_currentScene(nullptr)
	, m_nextScene(nullptr)
{

}

SceneManager::~SceneManager()
{
	delete m_currentScene;
	// nextScene은 nullptr인 상태
}

void SceneManager::ChangeScene(Scene* nextScene)
{
	m_nextScene = nextScene;
}

void SceneManager::Update(RECT& client, float deltaTime)
{
	if (m_nextScene)
	{
		delete m_currentScene;
		m_currentScene = m_nextScene;
		m_nextScene = nullptr;
		m_currentScene->Initialize();
	}

	if (m_currentScene)	m_currentScene->Update(client, deltaTime);
}

void SceneManager::Render(Renderer& renderer)
{
	if (m_currentScene)	m_currentScene->Render(renderer);
}

void SceneManager::HandleInput(float deltaTime)
{
	if (m_currentScene)	m_currentScene->HandleInput(deltaTime);
}


