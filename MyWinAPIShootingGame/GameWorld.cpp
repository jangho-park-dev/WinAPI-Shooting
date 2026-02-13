#include "GameWorld.h"
#include "Collider.h"
#include "Background.h"

GameWorld::GameWorld()
	: m_player(nullptr)
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
	m_player = new Player();
	m_objects.push_back(new Background());
	m_objects.push_back(m_player);
}

void GameWorld::AddObject(GameObject* object)
{
	m_objects.push_back(object);
}

void GameWorld::Update(RECT& client, float deltaTime)
{
	for (auto obj : m_objects)
	{
		if (obj->IsActive())
			obj->Update(client, deltaTime);
	}

	CheckCollisions();
	RemoveInactiveObjects();
}

void GameWorld::Render(Renderer& renderer)
{
	for (auto obj : m_objects)
	{
		if (obj->IsActive())
			obj->Render(renderer);
	}
}

void GameWorld::CheckCollisions()
{
	for (size_t i = 0; i < m_objects.size(); ++i)
	{
		for (size_t j = i + 1; j < m_objects.size(); ++j)
		{
			GameObject* a = m_objects[i];
			GameObject* b = m_objects[j];

			if (!a->IsActive() || !b->IsActive())
				continue;

			if (!a->GetCollider() || !b->GetCollider())
				continue;

			if (a->GetCollider()->CheckCollision(*b->GetCollider()))
			{
				a->OnCollision(*b);
				b->OnCollision(*a);
			}
		}
	}
}

void GameWorld::RemoveInactiveObjects()
{
	auto it = m_objects.begin();
	while (it != m_objects.end())
	{
		if (!(*it)->IsActive())
		{
			delete* it;
			it = m_objects.erase(it);
		}
		else
			++it;
	}
}

void GameWorld::HandleInput(float deltaTime)
{
	m_player->SetDirection(LEFT, GetAsyncKeyState(VK_LEFT) & 0x8000);
	m_player->SetDirection(RIGHT, GetAsyncKeyState(VK_RIGHT) & 0x8000);
	m_player->SetDirection(UP, GetAsyncKeyState(VK_UP) & 0x8000);
	m_player->SetDirection(DOWN, GetAsyncKeyState(VK_DOWN) & 0x8000);

	HandleFire(deltaTime);
}

void GameWorld::HandleFire(float deltaTime)
{
	static float fireTimer = 0.f;
	fireTimer += deltaTime;

	if ((GetAsyncKeyState(0x41) & 0x8000) && fireTimer > 0.1f)
	{
		Bullet* bullet = new Bullet(
			m_player->GetX() + m_player->GetWidth() / 2 - 4,
			m_player->GetY() - 18,
			600.f,
			BulletType::PLAYERBULLET
		);
		bullet->SetDamage(m_player->GetDamage());
		m_objects.push_back(bullet);

		fireTimer = 0.f;
	}
}

