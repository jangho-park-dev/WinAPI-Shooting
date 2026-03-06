#include "GameOverScene.h"
#include "TitleScene.h"
#include "GameScene.h"

static const int BTN_Y = 570;
static const int BTN_HOME_X = 260;
static const int BTN_REST_X = 370;
static const int BTN_W = 100;
static const int BTN_H = 55;

GameOverScene::GameOverScene(SceneManager* sceneManager, HWND hWnd)
	: m_sceneManager(sceneManager)
	, m_hWnd(hWnd)
	, m_background(nullptr)
	, m_buttonHome(nullptr)
	, m_buttonRestart(nullptr)
{

}

GameOverScene::~GameOverScene()
{
	delete m_background;
	delete m_buttonHome;
	delete m_buttonRestart;
}

void GameOverScene::Initialize()
{
	// TODO : 게임오버 BGM

	m_background = new Background(SpriteID::SPRITE_GAMEOVERBACKGROUND);
	m_buttonHome = new Button(SpriteID::SPRITE_BTN_HOME, BTN_HOME_X, BTN_Y, BTN_W, BTN_H);
	m_buttonRestart = new Button(SpriteID::SPRITE_BTN_RESTART, BTN_REST_X, BTN_Y, BTN_W, BTN_H);
}

void GameOverScene::Update(RECT& client, float deltaTime)
{

}

void GameOverScene::Render(Renderer& renderer)
{
	// 게임 오버 스프라이트 렌더링
	m_background->Render(renderer);
	m_buttonHome->Render(renderer);
	m_buttonRestart->Render(renderer);
}

void GameOverScene::HandleInput(float deltaTime)
{
	if (m_buttonHome->IsClickedClient(m_hWnd))
		m_sceneManager->ChangeScene(new TitleScene(m_sceneManager, m_hWnd));

	if (m_buttonRestart->IsClickedClient(m_hWnd))
		m_sceneManager->ChangeScene(new GameScene(m_sceneManager, m_hWnd));
}