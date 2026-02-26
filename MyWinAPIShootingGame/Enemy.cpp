#include "Enemy.h"
#include "GameWorld.h"
#include "Player.h"

Enemy::Enemy(float x, float y, float speed, EnemyType type, GameWorld* gameWorld)
	: m_gameWorld(gameWorld)
	, m_enemyType(type)
	, m_state(EnemyState::SPAWN)
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
	SetCollider(new BoxCollider(this));
	SetBehavior();

	m_nSrcX = 0;
}

void Enemy::SetBehavior()
{
	switch (m_enemyType)
	{
	case EnemyType::MONSTER:
	{
		// 0.15초 간격으로 발사, 3발 발사 후 2초 휴식
		m_behavior.nBurstCount = 0;
		m_behavior.nBurstMax = 3;
		m_behavior.fBurstTimer = 0.f;
		m_behavior.fBurstInterval = 0.15f;
		m_behavior.fRestTimer = 0.f;
		m_behavior.fRestInterval = 2.f;
		m_behavior.bResting = false;

		// 회전샷 초기 각도
		m_behavior.fRotateAngle = 30.f;
		// 목표 Y지점
		m_behavior.fSpawnTargetY = 0.f;
		// 사인 곡선 이동 타이머
		m_behavior.fSineMoveTimer = 0.f;
		// 사인 곡선 이동 진폭
		m_behavior.fSineMoveAmplitude = 0.f;
		// 데미지
		SetDamage(10);
	}
	break;
	case EnemyType::GOONS:
	{
		// 0.3초 간격으로 발사, 1발 발사 후 0.3초 휴식
		m_behavior.nBurstCount = 0;
		m_behavior.nBurstMax = 1;
		m_behavior.fBurstTimer = 0.f;
		m_behavior.fBurstInterval = 0.3f;
		m_behavior.fRestTimer = 0.f;
		m_behavior.fRestInterval = 0.3f;
		m_behavior.bResting = false;
		// 회전샷 초기 각도
		m_behavior.fRotateAngle = 30.f;
		// 목표 Y지점
		m_behavior.fSpawnTargetY = 700.f;
		// 사인 곡선 이동 타이머
		m_behavior.fSineMoveTimer = 0.f;
		// 사인 곡선 이동 진폭
		m_behavior.fSineMoveAmplitude = 100.f;
		// 데미지
		SetDamage(5);
		// Goons는 스프라이트 시트에서 랜덤한 X 좌표를 사용
		m_nSrcX = (rand() % 3) * 18;
	}
	break;
	case EnemyType::MOTHERSHIP:
	{
		// 0.2초 간격으로 발사, 5발 발사 후 3초 휴식
		m_behavior.nBurstCount = 0;
		m_behavior.nBurstMax = 5;
		m_behavior.fBurstTimer = 0.f;
		m_behavior.fBurstInterval = 0.2f;
		m_behavior.fRestTimer = 0.f;
		m_behavior.fRestInterval = 3.f;
		m_behavior.bResting = false;
		// 회전샷 초기 각도
		m_behavior.fRotateAngle = 30.f;
		// 목표 Y지점
		m_behavior.fSpawnTargetY = 0.f;
		// 사인 곡선 이동 타이머
		m_behavior.fSineMoveTimer = 0.f;
		// 사인 곡선 이동 진폭
		m_behavior.fSineMoveAmplitude = 0.f;
		SetDamage(10);
	}
	break;
	case EnemyType::DRAGON:
	{
		// 0.1초 간격으로 발사, 10발 발사 후 5초 휴식
		m_behavior.nBurstCount = 0;
		m_behavior.nBurstMax = 10;
		m_behavior.fBurstTimer = 0.f;
		m_behavior.fBurstInterval = 0.1f;
		m_behavior.fRestTimer = 0.f;
		m_behavior.fRestInterval = 5.f;
		m_behavior.bResting = false;
		// 회전샷 초기 각도
		m_behavior.fRotateAngle = 30.f;
		// 목표 Y지점
		m_behavior.fSpawnTargetY = 100.f;
		// 사인 곡선 이동 타이머
		m_behavior.fSineMoveTimer = 0.f;
		// 사인 곡선 이동 진폭
		m_behavior.fSineMoveAmplitude = 0.f;
		SetDamage(10);
	}
	break;
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

	if (m_enemyType != EnemyType::GOONS)
		renderer.DrawSprite(
			*m_sprite, 
			drawX, drawY
		);
	else
	{
		renderer.DrawSprite(
			*m_sprite,
			drawX, drawY,
			m_nSrcX, 0,
			GetWidth(), GetHeight()
		);
	}
		
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

	if (GetY() >= m_behavior.fSpawnTargetY)
	{
		SetY(m_behavior.fSpawnTargetY);
		m_state = EnemyState::ATTACK;
	}
}

void Enemy::MonsterAttack(float deltaTime)
{
	if (m_behavior.bResting)
	{
		m_behavior.fRestTimer += deltaTime;
		if (m_behavior.fRestTimer >= m_behavior.fRestInterval)
		{
			m_behavior.fRestTimer  = 0.f;
			m_behavior.nBurstCount = 0;
			m_behavior.bResting = false;
		}
		return;
	}

	m_behavior.fBurstTimer += deltaTime;
	if (m_behavior.fBurstTimer >= m_behavior.fBurstInterval)
	{
		m_behavior.fBurstTimer = 0.f;
		MonsterFire();
		++m_behavior.nBurstCount;

		if (m_behavior.nBurstCount >= m_behavior.nBurstMax)	m_behavior.bResting = true;
	}
}

void Enemy::MonsterFire()
{
	float fLeftGunX = GetX() + 12.f;
	float fRightGunX = GetX() + GetWidth() - 18.f;
	float fGunY = GetY() + GetHeight() + 10.f;

	float targetX = m_gameWorld->GetPlayer()->GetX() + m_gameWorld->GetPlayer()->GetWidth() / 2.f;
	float targetY = m_gameWorld->GetPlayer()->GetY() + m_gameWorld->GetPlayer()->GetHeight() / 2.f;

	int damage = GetDamage();

	// 조준샷 + 왼쪽으로 퍼지는 3way
	BulletPattern::AimShot(
		m_gameWorld,
		fLeftGunX, fGunY,
		targetX, targetY,
		300.f,
		damage,
		BulletType::MONSTERBULLET
	);
	BulletPattern::NWayShot(
		m_gameWorld,
		fLeftGunX, fGunY,
		150.f,
		3,
		30.f,
		damage,
		BulletType::MONSTERBULLET
	);

	// 조준샷 + 오른쪽으로 퍼지는 3way
	BulletPattern::AimShot(
		m_gameWorld,
		fRightGunX, fGunY,
		targetX, targetY,
		300.f,
		damage,
		BulletType::MONSTERBULLET
	);
	BulletPattern::NWayShot(
		m_gameWorld,
		fRightGunX, fGunY,
		150.f,
		3,
		30.f,
		damage,
		BulletType::MONSTERBULLET
	);
}

void Enemy::SineMove(float deltaTime)
{
	m_behavior.fSineMoveTimer += deltaTime * 3.f;
	SetX(GetX() + cosf(m_behavior.fSineMoveTimer) * m_behavior.fSineMoveAmplitude * deltaTime);
	SetY(GetY() + GetSpeed() * deltaTime);
}

void Enemy::GoonsSpawn(float deltaTime)
{
	if (GetY() < 0)	SineMove(deltaTime);
	else					m_state = EnemyState::ATTACK;
}

void Enemy::GoonsAttack(float deltaTime)
{
	SineMove(deltaTime);
	if (GetY() >= m_behavior.fSpawnTargetY)	SetActive(false);


	if (m_behavior.bResting)
	{
		m_behavior.fRestTimer += deltaTime;
		if (m_behavior.fRestTimer >= m_behavior.fRestInterval)
		{
			m_behavior.fRestTimer = 0.f;
			m_behavior.nBurstCount = 0;
			m_behavior.bResting = false;
		}
		return;
	}

	m_behavior.fBurstTimer += deltaTime;
	if (m_behavior.fBurstTimer >= m_behavior.fBurstInterval)
	{
		m_behavior.fBurstTimer = 0.f;
		GoonsFire();
		++m_behavior.nBurstCount;

		if (m_behavior.nBurstCount >= m_behavior.nBurstMax)	m_behavior.bResting = true;
	}
}

void Enemy::GoonsFire()
{
	float fGunX = GetX() + 4.f;
	float fGunY = GetY() + GetHeight() + 10.f;

	float targetX = m_gameWorld->GetPlayer()->GetX() + m_gameWorld->GetPlayer()->GetWidth() / 2.f;
	float targetY = m_gameWorld->GetPlayer()->GetY() + m_gameWorld->GetPlayer()->GetHeight() / 2.f;

	int damage = GetDamage();
	
	// 조준샷
	BulletPattern::AimShot(
		m_gameWorld,
		fGunX, fGunY,
		targetX, targetY,
		150.f,
		damage,
		BulletType::GOONSBULLET
	);
}

void Enemy::MothershipSpawn(float deltaTime)
{
	SetY(GetY() + GetSpeed() * deltaTime);
	if (GetY() >= m_behavior.fSpawnTargetY)
	{
		SetY(m_behavior.fSpawnTargetY);
		m_state = EnemyState::ATTACK;
	}
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


