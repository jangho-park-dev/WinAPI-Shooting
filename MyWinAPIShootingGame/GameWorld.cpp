#include "GameWorld.h"
#include "Player.h"
#include "Enemy.h"
#include "Collider.h"
#include "Background.h"
#include "GameOverScene.h"

GameWorld::GameWorld(SceneManager* sceneManager)
	: m_player(nullptr)
	, m_nCurrentWave(0)
	, m_fWaveTimer(0.f)
	, m_sceneManager(sceneManager)
	, m_nEnemyKilled(0)
	, m_fSurvivalTime(0.f)
	, m_bGameOver(false)
	, m_fFps(0.f)
{

}

GameWorld::~GameWorld()
{
	for (auto obj : m_objects)
		delete obj;
	m_objects.clear();
}

void GameWorld::Initialize()
{
	m_player = new Player(this);
	m_objects.push_back(new Background(0.f, 0.f, SpriteID::SPRITE_GAMEBACKGROUND));
	m_objects.push_back(new Background(0.f, -640.f, SpriteID::SPRITE_GAMEBACKGROUND));
	m_objects.push_back(m_player);

	m_waves = WaveLoader::GetWaves();
}

void GameWorld::AddObject(GameObject* object)
{
	m_spawnQueue.push(object);
}

void GameWorld::Update(RECT& client, float deltaTime)
{
	m_fSurvivalTime += deltaTime;

	UpdateWave(deltaTime);

	for (auto obj : m_objects)
	{
		if (obj->IsActive())
			obj->Update(client, deltaTime);
	}

	CheckCollisions();
	RemoveInactiveObjects();
	ProcessSpawnQueue();
	CheckWaveCleared();
	CheckRenderFPS(deltaTime);
}

void GameWorld::Render(Renderer& renderer)
{
	ObjectsSortByLayer();

	for (auto obj : m_objects)
	{
		if (obj->IsActive())
			obj->Render(renderer);
	}

	wchar_t buf[64];
	int nFps = static_cast<int>(m_fFps);
	swprintf_s(buf, L"FPS : %d", nFps);
	renderer.DrawString(380, 10, buf);
}

void GameWorld::CheckCollisions()
{
	// 유효한 충돌 쌍 정의 (플레이어-적, 플레이어-총알, 적-총알, 플레이어-아이템)
	static const std::pair<GameObjectType, GameObjectType> collisionPairs[] =
	{
		{ GameObjectType::PLAYER, GameObjectType::ENEMY },
		{ GameObjectType::PLAYER, GameObjectType::BULLET },
		{ GameObjectType::ENEMY, GameObjectType::BULLET },
		{ GameObjectType::PLAYER, GameObjectType::ITEM }
	};

	for (size_t i = 0; i < m_objects.size(); ++i)
	{
		for (size_t j = i + 1; j < m_objects.size(); ++j)
		{
			GameObject* alpha = m_objects[i];
			GameObject* beta = m_objects[j];

			if (!alpha->IsActive() || !beta->IsActive())			continue;
			if (!alpha->GetCollider() || !beta->GetCollider())	continue;

			bool validPair = false;

			for (auto& pair : collisionPairs)
			{
				if ((alpha->GetType() == pair.first && beta->GetType() == pair.second) ||
					(alpha->GetType() == pair.second && beta->GetType() == pair.first))
				{
					validPair = true;
					break;
				}
			}

			if (!validPair)	continue;

			if (alpha->GetCollider()->CheckCollision(*beta->GetCollider()))
			{
				alpha->OnCollision(*beta);
				beta->OnCollision(*alpha);
			}
		}
	}
}

void GameWorld::CheckRenderFPS(float deltaTime)
{
	static float countTime = 0.5f;
	countTime -= deltaTime;
	if (countTime <= 0.f)
	{
		m_fFps = deltaTime > 0.f ? 1.f / deltaTime : 0.f;
		countTime = 0.5f;
	}
}

void GameWorld::ObjectsSortByLayer()
{
	std::sort(m_objects.begin(), m_objects.end(),
		[](GameObject* alpha, GameObject* beta)
		{
			return alpha->GetLayer() < beta->GetLayer();
		}
	);
}

void GameWorld::RemoveInactiveObjects()
{
	auto it = std::remove_if(
		m_objects.begin(),
		m_objects.end(),
		[this](GameObject* obj)
		{
			if (!obj->IsActive())
			{
				if (obj->GetType() == GameObjectType::ENEMY)	++m_nEnemyKilled;

				delete obj;
				obj = nullptr;
				return true;   // 제거 대상
			}
			return false;
		}
	);

	m_objects.erase(it, m_objects.end());
}

void GameWorld::ProcessSpawnQueue()
{
	while (!m_spawnQueue.empty())
	{
		m_objects.push_back(m_spawnQueue.front());
		m_spawnQueue.pop();
	}
}

void GameWorld::HandleInput(float deltaTime)
{
	m_player->SetDirection(Keystates::LEFT,	KeyDown(VK_LEFT));
	m_player->SetDirection(Keystates::RIGHT,	KeyDown(VK_RIGHT));
	m_player->SetDirection(Keystates::UP,		KeyDown(VK_UP));
	m_player->SetDirection(Keystates::DOWN,	KeyDown(VK_DOWN));

	HandleFire(deltaTime);

	// S키를 누르면 이동 속도가 절반으로 감소, 떼면 원래대로
	if (KeyDown(0x53))	m_player->SetSpeed(m_player->GetBaseSpeed() / 2);
	if (KeyUp(0x53))		m_player->SetSpeed(m_player->GetBaseSpeed());
}

bool GameWorld::KeyDown(int keyCode)
{
	return GetAsyncKeyState(keyCode) & 0x8000 ? 1 : 0;
}

bool GameWorld::KeyUp(int keyCode)
{
	return GetAsyncKeyState(keyCode) & 0x8000 ? 0 : 1;
}

void GameWorld::HandleFire(float deltaTime)
{
	static float fireTimer = 0.f;
	fireTimer += deltaTime;

	if (KeyDown(0x41) && fireTimer > 0.1f)
	{
		ResourceManager::GetInstance().RPlaySound(SoundID::SOUND_PLAYER_SHOOT, 0.01f);

		int damage = m_player->GetDamage();
		int bulletNums = m_player->GetNumberOfBullets();
		for (int i = 0; i < bulletNums; ++i)
		{
			float offsetX = (i - bulletNums / 2.f + 0.5f) * 10.f;
			Bullet* bullet = new Bullet(
				m_player->GetX() + m_player->GetSrcWidth() / 2 - 4 + offsetX,
				m_player->GetY() - 18,
				600.f,
				damage,
				BulletType::PLAYERBULLET
			);
			bullet->SetDamage(m_player->GetDamage());
			m_objects.push_back(bullet);
		}

		fireTimer = 0.f;
	}
}

void GameWorld::UpdateWave(float deltaTime)
{
	if (m_nCurrentWave >= static_cast<int>(m_waves.size()))	return;

	m_fWaveTimer += deltaTime;
	
	for (auto& event : m_waves[m_nCurrentWave].spawnEvents)
	{
		if (!event.spawned && m_fWaveTimer >= event.spawnTime)
		{
			AddObject(new Enemy(
				event.x, event.y,
				event.speed,
				event.enemyType,
				this)
			);
			event.spawned = true;
		}
	}
}

void GameWorld::CheckWaveCleared()
{
	if (m_nCurrentWave >= static_cast<int>(m_waves.size()))	return;

	for (auto& event : m_waves[m_nCurrentWave].spawnEvents)
		if (!event.spawned)	return;
	
	for (auto obj : m_objects)
		if (obj->IsActive() && obj->GetType() == GameObjectType::ENEMY)	return;

	std::cout << "Wave " << m_nCurrentWave + 1 << " cleared!" << std::endl;
	++m_nCurrentWave;
	m_fWaveTimer = 0.f;

}