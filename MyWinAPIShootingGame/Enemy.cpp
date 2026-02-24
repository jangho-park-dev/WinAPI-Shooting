#include "Enemy.h"
#include "GameWorld.h"
#include "Player.h"

Enemy::Enemy(float x, float y, float speed, EnemyType type, GameWorld* gameWorld)
	: m_gameWorld(gameWorld)
	, m_enemyType(type)
	, m_state(EnemyState::SPAWN)
	, m_fSpawnTargetY(0.f)
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
		// 0.15초 간격으로 발사, 3발 발사 후 1.5초 휴식
		m_nBurstCount = 0;
		m_nBurstMax = 3;
		m_fBurstTimer = 0.f;
		m_fBurstInterval = 0.15f;		
		m_fRestTimer = 0.f;
		m_fRestInterval = 1.5f;
		m_bResting = false;
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

	switch (m_state)
	{
	case EnemyState::SPAWN:		EnemySpawnProcess(deltaTime);		break;
	case EnemyState::ATTACK:		EnemyAttackProcess(deltaTime);		break;
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

	// TODO : 피격 사운드
}


void Enemy::EnemySpawnProcess(float deltaTime)
{
	switch (m_enemyType)
	{
	case EnemyType::MONSTER:		MonsterSpawn(deltaTime);		break;
	case EnemyType::GOONS:			GoonsSpawn(deltaTime);		break;
	case EnemyType::MOTHERSHIP:	MothershipSpawn(deltaTime);	break;
	case EnemyType::DRAGON:		DragonSpawn(deltaTime);		break;
	}
}

void Enemy::EnemyAttackProcess(float deltaTime)
{
	switch (m_enemyType)
	{
	case EnemyType::MONSTER:		MonsterAttack(deltaTime);		break;
	case EnemyType::GOONS:			GoonsAttack(deltaTime);			break;
	case EnemyType::MOTHERSHIP:	MothershipAttack(deltaTime);	break;
	case EnemyType::DRAGON:		DragonAttack(deltaTime);		break;
	}
}

void Enemy::MonsterSpawn(float deltaTime)
{
	SetY(GetY() + GetSpeed() * deltaTime);

	if (GetY() >= m_fSpawnTargetY)
	{
		SetY(m_fSpawnTargetY);
		m_state = EnemyState::ATTACK;
	}
}

void Enemy::MonsterAttack(float deltaTime)
{
	if (m_bResting)
	{
		m_fRestTimer += deltaTime;
		if (m_fRestTimer >= m_fRestInterval)
		{
			m_fRestTimer  = 0.f;
			m_nBurstCount = 0;
			m_bResting = false;
		}
		return;
	}

	m_fBurstTimer += deltaTime;
	if (m_fBurstTimer >= m_fBurstInterval)
	{
		m_fBurstTimer = 0.f;
		MonsterFire();
		++m_nBurstCount;

		if (m_nBurstCount >= m_nBurstMax)	m_bResting = true;
	}
}

void Enemy::MonsterFire()
{
	float fLeftGunX = GetX() + 10.f;
	float fRightGunX = GetX() + GetWidth() - 10.f;
	float fGunY = GetY() + GetHeight() + 10.f;

	float targetX = m_gameWorld->GetPlayer()->GetX() + m_gameWorld->GetPlayer()->GetWidth() / 2.f;
	float targetY = m_gameWorld->GetPlayer()->GetY() + m_gameWorld->GetPlayer()->GetHeight() / 2.f;

	int damage = GetDamage();

	// 조준샷 + 왼쪽으로 퍼지는 3way
	BulletPattern::AimShot(
		m_gameWorld,
		fLeftGunX, fGunY,
		targetX, targetY,
		200.f,
		damage,
		BulletType::MONSTERBULLET
	);
	BulletPattern::NWayShot(
		m_gameWorld,
		fLeftGunX, fGunY,
		150.f,
		3,
		45.f,
		damage,
		BulletType::MONSTERBULLET
	);

	// 조준샷 + 오른쪽으로 퍼지는 3way
	BulletPattern::AimShot(
		m_gameWorld,
		fRightGunX, fGunY,
		targetX, targetY,
		200.f,
		damage,
		BulletType::MONSTERBULLET
	);
	BulletPattern::NWayShot(
		m_gameWorld,
		fRightGunX, fGunY,
		150.f,
		3,
		45.f,
		damage,
		BulletType::MONSTERBULLET
	);
}

void Enemy::GoonsSpawn(float deltaTime)
{

}

void Enemy::GoonsAttack(float deltaTime)
{

}

void Enemy::GoonsFire()
{

}

void Enemy::MothershipSpawn(float deltaTime)
{

}

void Enemy::MothershipAttack(float deltaTime)
{

}

void Enemy::MothershipFire()
{

}

void Enemy::DragonSpawn(float deltaTime)
{

}

void Enemy::DragonAttack(float deltaTime)
{

}

void Enemy::DragonFire()
{

}


