#include "GameScene.h"
#include "GameOverScene.h"
#include "Player.h"

GameScene::GameScene(SceneManager* sceneManager, HWND hWnd)
	: m_sceneManager(sceneManager)
	, m_hWnd(hWnd)
	, m_world(nullptr)
{

}

GameScene::~GameScene()
{
	delete m_world;
}

void GameScene::Initialize()
{
	m_world = new GameWorld(m_sceneManager);
	m_world->Initialize();
}

void GameScene::Update(RECT& client, float deltaTime)
{
	m_world->Update(client, deltaTime);

	if (!m_world->GetPlayer()->IsActive())
		m_sceneManager->ChangeScene(new GameOverScene(m_sceneManager, m_hWnd));
}

void GameScene::Render(Renderer& renderer)
{
	m_world->Render(renderer);
}

void GameScene::HandleInput(float deltaTime)
{
	m_world->HandleInput(deltaTime);
}


