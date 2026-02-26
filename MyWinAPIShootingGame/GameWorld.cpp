#include "GameWorld.h"
#include "Player.h"
#include "Enemy.h"
#include "Collider.h"
#include "Background.h"

GameWorld::GameWorld()
	: m_player(nullptr)
{
	m_nCurrentWave = 0;
	m_fWaveTimer = 0.f;
}

GameWorld::~GameWorld()
{
	for (auto obj : m_objects)
		delete obj;
	m_objects.clear();
}

void GameWorld::Initialize()
{
	m_player = new Player();
	m_objects.push_back(new Background());
	m_objects.push_back(m_player);

	m_waves = WaveLoader::GetWaves();
}

void GameWorld::AddObject(GameObject* object)
{
	m_spawnQueue.push(object);
}

void GameWorld::Update(RECT& client, float deltaTime)
{
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
}

void GameWorld::Render(Renderer& renderer)
{
	ObjectsSortByLayer();

	for (auto obj : m_objects)
	{
		if (obj->IsActive())
			obj->Render(renderer);
	}
}

void GameWorld::CheckCollisions()
{
	// 유효한 충돌 쌍 정의 (플레이어-적, 플레이어-총알, 적-총알)
	static const std::pair<GameObjectType, GameObjectType> collisionPairs[] =
	{
		{ GameObjectType::PLAYER, GameObjectType::ENEMY },
		{ GameObjectType::PLAYER, GameObjectType::BULLET },
		{ GameObjectType::ENEMY, GameObjectType::BULLET }
	};

	for (size_t i = 0; i < m_objects.size(); ++i)
	{
		for (size_t j = i + 1; j < m_objects.size(); ++j)
		{
			GameObject* a = m_objects[i];
			GameObject* b = m_objects[j];

			if (!a->IsActive() || !b->IsActive())			continue;
			if (!a->GetCollider() || !b->GetCollider())	continue;

			bool validPair = false;

			for (auto& pair : collisionPairs)
			{
				if ((a->GetType() == pair.first && b->GetType() == pair.second) ||
					(a->GetType() == pair.second && b->GetType() == pair.first))
				{
					validPair = true;
					break;
				}
			}

			if (!validPair)	continue;

			if (a->GetCollider()->CheckCollision(*b->GetCollider()))
			{
				a->OnCollision(*b);
				b->OnCollision(*a);
			}
		}
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
		[](GameObject* obj)
		{
			if (!obj->IsActive())
			{
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
	m_player->SetDirection(Keystates::LEFT, KeyDown(VK_LEFT));
	m_player->SetDirection(Keystates::RIGHT, KeyDown(VK_RIGHT));
	m_player->SetDirection(Keystates::UP, KeyDown(VK_UP));
	m_player->SetDirection(Keystates::DOWN, KeyDown(VK_DOWN));

	HandleFire(deltaTime);

	// S키를 누르면 이동 속도가 절반으로 감소, 떼면 원래대로
	if (KeyDown(0x53))	m_player->SetSpeed(150.f);
	if (KeyUp(0x53))		m_player->SetSpeed(300.f);
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
	int damage = m_player->GetDamage();

	if (KeyDown(0x41) && fireTimer > 0.1f)
	{
		Bullet* bullet = new Bullet(
			m_player->GetX() + m_player->GetWidth() / 2 - 4,
			m_player->GetY() - 18,
			600.f,
			damage,
			BulletType::PLAYERBULLET
		);
		bullet->SetDamage(m_player->GetDamage());
		m_objects.push_back(bullet);

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
			Enemy* enemy = new Enemy(
				event.x, event.y, event.speed,
				event.enemyType, this
			);
			AddObject(enemy);
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