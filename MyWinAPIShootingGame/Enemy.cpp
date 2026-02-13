#include "Enemy.h"

Enemy::Enemy(float x, float y, float speed, EnemyType type)
	: m_enemyType(type)
{
	SetSprite();
	SetType(GameObjectType::ENEMY);
	SetLayer(GameObjectLayer::ENEMY_LAYER);
	SetActive(true);
	SetX(x);
	SetY(y);
	SetSpeed(speed);
	SetWH();
	SetHealth(30);
	SetDamage(10);
	SetCollider(new BoxCollider(this));
}

void Enemy::SetSprite()
{
	switch (m_enemyType)
	{
	case EnemyType::MONSTER:
		m_sprite = ResourceManager::GetInstance().GetSprite(SpriteID::SPRITE_MONSTER);
		break;
	case EnemyType::MOTHERSHIP:
		m_sprite = ResourceManager::GetInstance().GetSprite(SpriteID::SPRITE_MOTHERSHIP);
		break;
	case EnemyType::DRAGON:
		m_sprite = ResourceManager::GetInstance().GetSprite(SpriteID::SPRITE_DRAGON);
		break;
	case EnemyType::GOONS:
		m_sprite = ResourceManager::GetInstance().GetSprite(SpriteID::SPRITE_GOONS);
		break;
	}
}

void Enemy::SetWH()
{
	switch (m_enemyType)
	{
	case EnemyType::MONSTER:
		SetWidth(m_sprite->GetWidth());
		SetHeight(m_sprite->GetHeight());
		break;
	case EnemyType::MOTHERSHIP:
		SetWidth(240);
		SetHeight(128);
		break;
	case EnemyType::DRAGON:
		SetWidth(m_sprite->GetWidth());
		SetHeight(m_sprite->GetHeight());
		break;
	case EnemyType::GOONS:
		SetWidth(18);
		SetHeight(18);
		break;
	}
}

void Enemy::Update(RECT& client, float deltaTime)
{
	if (!IsActive())	return;

}

void Enemy::Render(Renderer& renderer)
{
	if (!IsActive())	return;

}

void Enemy::OnCollision(GameObject& other)
{
	if (!IsActive())	return;

}

