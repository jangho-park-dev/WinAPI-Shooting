#include "TitleScene.h"
#include "GameScene.h"

static const int BTN_Y = 550;
static const int BTN_START_X = 200;
static const int BTN_EXIT_X = 350;
static const int BTN_W = 150;
static const int BTN_H = 80;

TitleScene::TitleScene(SceneManager* sceneManager, HWND hWnd)
	: m_sceneManager(sceneManager)
	, m_hWnd(hWnd)
	, m_background(nullptr)
	, m_buttonStart(nullptr)
	, m_buttonExit(nullptr)
{

}

TitleScene::~TitleScene()
{
	delete m_background;
	delete m_buttonStart;
	delete m_buttonExit;
}

void TitleScene::Initialize()
{
	// TODO : title bgm
	//ResourceManager::GetInstance().RPlaySound(SoundID::SOUND_TITLE_BGM, true);
	m_background = new Background(SpriteID::SPRITE_TITLEBACKGROUND);
	m_buttonStart = new Button(SpriteID::SPRITE_BTN_START, BTN_START_X, BTN_Y, BTN_W, BTN_H);
	m_buttonExit = new Button(SpriteID::SPRITE_BTN_EXIT, BTN_EXIT_X, BTN_Y, BTN_W, BTN_H);
}

void TitleScene::Update(RECT& client, float deltaTime)
{
	
}

void TitleScene::Render(Renderer& renderer)
{
	m_background->Render(renderer);
	m_buttonStart->Render(renderer);
	m_buttonExit->Render(renderer);
}

void TitleScene::HandleInput(float deltaTime)
{
	if (m_buttonStart->IsClickedClient(m_hWnd))
		m_sceneManager->ChangeScene(new GameScene(m_sceneManager, m_hWnd));

	if (m_buttonExit->IsClickedClient(m_hWnd))	PostQuitMessage(0);
}
