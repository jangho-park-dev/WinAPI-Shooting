#include "GameOverScene.h"
#include "TitleScene.h"
#include "GameScene.h"

static const int BTN_Y = 550;
static const int BTN_HOME_X = 200;
static const int BTN_REST_X = 350;
static const int BTN_W = 150;
static const int BTN_H = 80;

static const int STATS_X = 40;
static const int STATS_Y = 340;
static const int STATS_LINE = 36;	// 줄 간격

GameOverScene::GameOverScene(SceneManager* sceneManager, HWND hWnd, GameStats stats)
	: m_sceneManager(sceneManager)
	, m_hWnd(hWnd)
	, m_background(nullptr)
	, m_buttonHome(nullptr)
	, m_buttonRestart(nullptr)
	, m_stats(stats)
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
	ResourceManager::GetInstance().RPlaySound(SoundID::SOUND_GAMEOVER_BGM, 0.05f, true);

	m_background = new Background(0.f, 0.f, SpriteID::SPRITE_GAMEOVERBACKGROUND);
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

	// 결과 텍스트
	wchar_t buf[64];
	swprintf_s(buf, L"Wave Cleared : %d", m_stats.nWaveCleared);
	renderer.DrawString(STATS_X, STATS_Y, buf);

	swprintf_s(buf, L"Enemies Killed : %d", m_stats.nEnemyKilled);
	renderer.DrawString(STATS_X, STATS_Y + STATS_LINE, buf);

	int min = static_cast<int>(m_stats.fSurvivalTime) / 60;
	int sec = static_cast<int>(m_stats.fSurvivalTime) % 60;
	swprintf_s(buf, L"Survival Time : %02d:%02d", min, sec);
	renderer.DrawString(STATS_X, STATS_Y + STATS_LINE * 2, buf);
}

void GameOverScene::HandleInput(float deltaTime)
{
	if (m_buttonHome->IsClickedClient(m_hWnd))
	{
		ResourceManager::GetInstance().RStopSound(SoundID::SOUND_GAMEOVER_BGM);
		m_sceneManager->ChangeScene(new TitleScene(m_sceneManager, m_hWnd));
		return;
	}

	if (m_buttonRestart->IsClickedClient(m_hWnd))
	{
		ResourceManager::GetInstance().RStopSound(SoundID::SOUND_GAMEOVER_BGM);
		m_sceneManager->ChangeScene(new GameScene(m_sceneManager, m_hWnd));
		return;
	}
}