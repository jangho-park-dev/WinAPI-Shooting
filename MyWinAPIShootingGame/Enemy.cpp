#include "Enemy.h"
#include "GameWorld.h"

Enemy::Enemy(float x, float y, float speed, EnemyType type, GameWorld* gameWorld)
	: m_gameWorld(gameWorld)
	, m_enemyType(type)
	, m_state(EnemyState::SPAWN)
	, m_fTargetY(0.f)
	, m_fFireTimer(0.f)
	, m_fFireInterval(0.f)
{
	SetSprite();
	SetType(GameObjectType::ENEMY);
	SetLayer(GameObjectLayer::ENEMY_LAYER);
	SetActive(true);
	SetX(x);
	SetY(y);
	SetSpeed(speed);
	SetWH();
	SetHealth(300);
	SetDamage(10);
	SetCollider(new BoxCollider(this));
	

	if (m_enemyType == EnemyType::MONSTER)
	{
		m_fTargetY = 100.f;
		m_fFireInterval = 1.5f;
	}
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

	//std::cout << GetY() << std::endl;

	switch (m_state)
	{
	case EnemyState::SPAWN:
		SetY(GetY() + GetSpeed() * deltaTime);

		if (GetY() >= m_fTargetY)
		{
			SetY(m_fTargetY);
			m_state = EnemyState::ATTACK;
			m_fFireTimer = 0.f;
		}
		break;

	case EnemyState::ATTACK:
		m_fFireTimer += deltaTime;

		if (m_fFireTimer >= m_fFireInterval)
		{
			Fire();
			m_fFireTimer = 0.f;
		}
		break;
	}

}

void Enemy::Render(Renderer& renderer)
{
	if (!IsActive())	return;

	int drawX = static_cast<int>(GetX() + 0.5f);
	int drawY = static_cast<int>(GetY() + 0.5f);

	renderer.DrawSprite(
		*m_sprite,
		drawX, drawY
	);
}

void Enemy::OnCollision(GameObject& other)
{
	if (!IsActive())	return;

}

void Enemy::Fire()
{
	float fLeftGunX = GetX() + 10.f;
	float fRightGunX = GetX() + GetWidth() - 10.f;
	float gunY = GetY() + GetHeight() + 10.f;

	Bullet* leftBullet = new Bullet(fLeftGunX, gunY, 200.f, BulletType::MONSTERBULLET);
	Bullet* rightBullet = new Bullet(fRightGunX, gunY, 200.f, BulletType::MONSTERBULLET);

	leftBullet->SetDamage(GetDamage());
	rightBullet->SetDamage(GetDamage());

	m_gameWorld->AddObject(leftBullet);
	m_gameWorld->AddObject(rightBullet);
}

