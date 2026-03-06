#include "Enemy.h"
#include "GameWorld.h"
#include "Player.h"
#include "Item.h"

Enemy::Enemy(float x, float y, float speed, EnemyType type, GameWorld* gameWorld)
	: m_gameWorld(gameWorld)
	, m_enemyType(type)
	, m_state(EnemyState::SPAWN)
	, m_fMoveDir(1.f)
{
	SetSprite();
	SetType(GameObjectType::ENEMY);
	SetLayer(GameObjectLayer::ENEMY_LAYER);
	SetActive(true);
	SetX(x);
	SetY(y);
	SetSpeed(speed);
	SetWH();
	SetCollider(new BoxCollider(this));

	m_nSrcX = 0;
	m_nSrcY = 0;

	SetBehavior();
}

void Enemy::SetBehavior()
{
	switch (m_enemyType)
	{
	case EnemyType::MONSTER:
	{
		m_behavior.nBurstCount = 0;
		m_behavior.nBurstMax = 10;
		m_behavior.fBurstTimer = 0.f;
		m_behavior.fBurstInterval = 0.2f;
		m_behavior.fRestTimer = 0.f;
		m_behavior.fRestInterval = 2.f;
		m_behavior.bResting = false;
		m_behavior.fRotateAngle = 30.f;
		m_behavior.fSpawnTargetY = static_cast<float>(rand() % 300);
		m_behavior.fSineMoveTimer = 0.f;
		m_behavior.fSineMoveAmplitude = 0.f;
		m_behavior.nCycleCount = 0;
		m_behavior.bCharging = false;
		m_behavior.fChargeTimer = 0.f;
		m_behavior.fChargeInterval = 0.f;
		m_behavior.nLaserBurstCount = 0;
		m_behavior.nLaserBurstMax = 0;
		m_behavior.fLaserBurstTimer = 0.f;
		m_behavior.fLaserBurstInterval = 0.f;
		SetDamage(10);
		SetHealth(300 * (m_gameWorld->GetCurrentWave() + 1));
	}
	break;
	case EnemyType::GOONS:
	{
		m_behavior.nBurstCount = 0;
		m_behavior.nBurstMax = 1;
		m_behavior.fBurstTimer = 0.f;
		m_behavior.fBurstInterval = 0.3f;
		m_behavior.fRestTimer = 0.f;
		m_behavior.fRestInterval = 1.f;
		m_behavior.bResting = false;
		m_behavior.fRotateAngle = 30.f;
		m_behavior.fSpawnTargetY = 700.f;
		m_behavior.fSineMoveTimer = 0.f;
		m_behavior.fSineMoveAmplitude = 100.f;
		m_behavior.nCycleCount = 0;
		m_behavior.bCharging = false;
		m_behavior.fChargeTimer = 0.f;
		m_behavior.fChargeInterval = 0.f;
		m_behavior.nLaserBurstCount = 0;
		m_behavior.nLaserBurstMax = 0;
		m_behavior.fLaserBurstTimer = 0.f;
		m_behavior.fLaserBurstInterval = 0.f;
		SetDamage(5);
		m_nSrcX = (rand() % 3) * 18;
		SetHealth(30 * (m_gameWorld->GetCurrentWave() + 1));
	}
	break;
	case EnemyType::MOTHERSHIP:
	{
		m_behavior.nBurstCount = 0;
		m_behavior.nBurstMax = 7;
		m_behavior.fBurstTimer = 0.f;
		m_behavior.fBurstInterval = 0.2f;
		m_behavior.fRestTimer = 0.f;
		m_behavior.fRestInterval = 0.5f;
		m_behavior.bResting = false;
		m_behavior.fRotateAngle = 30.f;
		m_behavior.fSpawnTargetY = 0.f;
		m_behavior.fSineMoveTimer = 0.f;
		m_behavior.fSineMoveAmplitude = 0.f;
		m_behavior.nCycleCount = 0;
		m_behavior.bCharging = false;
		m_behavior.fChargeTimer = 0.f;
		m_behavior.fChargeInterval = 0.f;
		m_behavior.nLaserBurstCount = 0;
		m_behavior.nLaserBurstMax = 0;
		m_behavior.fLaserBurstTimer = 0.f;
		m_behavior.fLaserBurstInterval = 0.f;
		SetDamage(10);
		SetHealth(10000);
	}
	break;
	case EnemyType::DRAGON:
	{
		m_behavior.nBurstCount = 0;
		m_behavior.nBurstMax = 10;
		m_behavior.fBurstTimer = 0.f;
		m_behavior.fBurstInterval = 0.15f;
		m_behavior.fRestTimer = 0.f;
		m_behavior.fRestInterval = 2.f;
		m_behavior.bResting = false;
		m_behavior.fRotateAngle = 0.f;
		m_behavior.fSpawnTargetY = 80.f;
		m_behavior.fSineMoveTimer = 0.f;
		m_behavior.fSineMoveAmplitude = 0.f;
		m_behavior.nCycleCount = 0;
		// 레이저 차징
		m_behavior.bCharging = false;
		m_behavior.fChargeTimer = 0.f;
		m_behavior.fChargeInterval = 0.8f;
		m_behavior.nLaserBurstCount = 0;
		m_behavior.nLaserBurstMax = 5;
		m_behavior.fLaserBurstTimer = 0.f;
		m_behavior.fLaserBurstInterval = 0.05f;
		SetDamage(15);
		SetHealth(15000);
	}
	break;
	}
}

void Enemy::SetSprite()
{
	switch (m_enemyType)
	{
	case EnemyType::MONSTER:
		m_enemySprite = ResourceManager::GetInstance().GetSprite(SpriteID::SPRITE_MONSTER);
		m_enemySprite->SetSpriteSizeMultiplier(1.f);
		break;
	case EnemyType::MOTHERSHIP:
		m_enemySprite = ResourceManager::GetInstance().GetSprite(SpriteID::SPRITE_MOTHERSHIP);
		m_enemySprite->SetSpriteSizeMultiplier(1.f);
		break;
	case EnemyType::DRAGON:
		m_enemySprite = ResourceManager::GetInstance().GetSprite(SpriteID::SPRITE_DRAGON);
		m_enemySprite->SetSpriteSizeMultiplier(1.f);
		break;
	case EnemyType::GOONS:
		m_enemySprite = ResourceManager::GetInstance().GetSprite(SpriteID::SPRITE_GOONS);
		m_enemySprite->SetSpriteSizeMultiplier(1.2f);
		break;
	}
}

void Enemy::SetWH()
{
	switch (m_enemyType)
	{
	case EnemyType::MONSTER:
		SetSrcWidth(m_enemySprite->GetWidth());
		SetSrcHeight(m_enemySprite->GetHeight());
		break;
	case EnemyType::MOTHERSHIP:
		SetSrcWidth(240);
		SetSrcHeight(128);
		break;
	case EnemyType::DRAGON:
		SetSrcWidth(m_enemySprite->GetWidth());
		SetSrcHeight(m_enemySprite->GetHeight());
		break;
	case EnemyType::GOONS:
		SetSrcWidth(18);
		SetSrcHeight(18);
		break;
	}

	SetRenderSize(
		static_cast<int>(GetSrcWidth() * m_enemySprite->GetSpriteSizeMultiplier()),
		static_cast<int>(GetSrcHeight() * m_enemySprite->GetSpriteSizeMultiplier())
	);
}

void Enemy::Update(RECT& client, float deltaTime)
{
	if (!IsActive())	return;

	switch (m_state)
	{
	case EnemyState::SPAWN:		EnemySpawnProcess(deltaTime);	break;
	case EnemyState::ATTACK:	EnemyAttackProcess(deltaTime);	break;
	}
}

void Enemy::Render(Renderer& renderer)
{
	if (!IsActive())	return;

	int drawX = static_cast<int>(GetX() + 0.5f);
	int drawY = static_cast<int>(GetY() + 0.5f);

	if (m_enemyType == EnemyType::GOONS ||
		m_enemyType == EnemyType::MOTHERSHIP)
		renderer.DrawSprite(
			*m_enemySprite,
			drawX, drawY,
			m_nSrcX, m_nSrcY,
			GetSrcWidth(), GetSrcHeight(),
			GetRenderWidth(), GetRenderHeight()
		);
	else
		renderer.DrawSprite(*m_enemySprite, drawX, drawY);

	// 드래곤 차징 중 조준선 렌더링
	if (m_enemyType == EnemyType::DRAGON && m_behavior.bCharging)
	{
		int fromX = static_cast<int>(GetX() + GetRenderWidth() / 2.f);
		int fromY = static_cast<int>(GetY() + GetRenderHeight() / 2.f);
		int toX = static_cast<int>(m_gameWorld->GetPlayer()->GetX() + m_gameWorld->GetPlayer()->GetRenderWidth() / 2.f);
		int toY = static_cast<int>(m_gameWorld->GetPlayer()->GetY() + m_gameWorld->GetPlayer()->GetRenderHeight() / 2.f);

		renderer.DrawLine(fromX, fromY, toX, toY, RGB(255, 0, 0));
	}
}

void Enemy::OnCollision(GameObject& other)
{
	if (!IsActive())	return;

	SetMothershipSpriteByHealth();

	if (other.GetType() == GameObjectType::BULLET &&
		dynamic_cast<Bullet&>(other).GetBulletType() == BulletType::PLAYERBULLET)
	{
		ResourceManager::GetInstance().RPlaySound(SoundID::SOUND_ENEMY_HIT, 0.01f);
		m_gameWorld->AddObject(new Effect(
			other.GetX() - (32 - other.GetSrcWidth() / 2),		// 32 -> 이펙트 크기 절반 하드코딩
			other.GetY() - (32 - other.GetSrcHeight() / 2),
			BulletType::PLAYERBULLET
		));
	}
}

void Enemy::SetMothershipSpriteByHealth()
{
	if (m_enemyType != EnemyType::MOTHERSHIP)	return;

	if (8000 < GetHealth() && GetHealth() <= 10000)
	{
		m_nSrcX = 0;
		m_nSrcY = 0;
	}
	else if (6000 < GetHealth() && GetHealth() <= 8000)
	{
		m_nSrcX = 240;
		m_nSrcY = 0;
	}
	else if (4000 < GetHealth() && GetHealth() <= 6000)
	{
		m_nSrcX = 480;
		m_nSrcY = 0;
	}
	else if (2000 < GetHealth() && GetHealth() <= 4000)
	{
		m_nSrcX = 0;
		m_nSrcY = 128;
	}
	else if (0 < GetHealth() && GetHealth() <= 2000)
	{
		m_nSrcX = 240;
		m_nSrcY = 128;
	}
}

void Enemy::OnDeath()
{
	ResourceManager::GetInstance().RPlaySound(SoundID::SOUND_ENEMY_DEATH, 0.02f);

	ItemType dropType = ItemType::SPEEDUP;
	switch (rand() % 4)
	{
	case 0:	dropType = ItemType::SPEEDUP;	break;
	case 1:	dropType = ItemType::POWERUP;	break;
	case 2:	dropType = ItemType::BULLETUP;	break;
	case 3:	dropType = ItemType::HEALTH;	break;
	}
	if (rand() % 100 < 30)
		m_gameWorld->AddObject(new Item(
			GetX() + GetRenderWidth() / 2.f - 4.f,
			GetY() + GetRenderHeight() / 2.f - 4.f,
			100.f, dropType)
		);
}

void Enemy::EnemySpawnProcess(float deltaTime)
{
	switch (m_enemyType)
	{
	case EnemyType::MONSTER:	MonsterSpawn(deltaTime);		break;
	case EnemyType::GOONS:		GoonsSpawn(deltaTime);			break;
	case EnemyType::MOTHERSHIP:	MothershipSpawn(deltaTime);		break;
	case EnemyType::DRAGON:		DragonSpawn(deltaTime);			break;
	}
}

void Enemy::EnemyAttackProcess(float deltaTime)
{
	switch (m_enemyType)
	{
	case EnemyType::MONSTER:	MonsterAttack(deltaTime);		break;
	case EnemyType::GOONS:		GoonsAttack(deltaTime);			break;
	case EnemyType::MOTHERSHIP:	MothershipAttack(deltaTime);	break;
	case EnemyType::DRAGON:		DragonAttack(deltaTime);		break;
	}
}

// ===================== Monster =====================

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
		MonsterFire();
		++m_behavior.nBurstCount;

		if (m_behavior.nBurstCount >= m_behavior.nBurstMax)	m_behavior.bResting = true;
	}
}

void Enemy::MonsterFire()
{
	BulletPattern::RotateShot(
		m_gameWorld,
		GetX() + GetRenderWidth() / 2, GetY() + GetRenderHeight() / 2,
		200.f,
		20,
		360.f,
		GetDamage(),
		BulletType::MONSTERBULLET,
		m_behavior.fRotateAngle
	);
	m_behavior.fRotateAngle += 5.f;
}

// ===================== Goons =====================

void Enemy::SineMove(float deltaTime)
{
	m_behavior.fSineMoveTimer += deltaTime * 3.f;
	SetX(GetX() + cosf(m_behavior.fSineMoveTimer) * m_behavior.fSineMoveAmplitude * deltaTime);
	SetY(GetY() + GetSpeed() * deltaTime);
}

void Enemy::GoonsSpawn(float deltaTime)
{
	if (GetY() < 0)	SineMove(deltaTime);
	else			m_state = EnemyState::ATTACK;
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
	float fGunY = GetY() + GetRenderHeight() + 10.f;

	float targetX = m_gameWorld->GetPlayer()->GetX() + m_gameWorld->GetPlayer()->GetRenderWidth() / 2.f;
	float targetY = m_gameWorld->GetPlayer()->GetY() + m_gameWorld->GetPlayer()->GetRenderHeight() / 2.f;

	BulletPattern::AimShot(
		m_gameWorld,
		fGunX, fGunY,
		targetX, targetY,
		150.f,
		GetDamage(),
		BulletType::GOONSBULLET
	);
}

// ===================== Mothership =====================

void Enemy::MothershipSpawn(float deltaTime)
{
	SetY(GetY() + GetSpeed() * deltaTime);
	if (GetY() >= m_behavior.fSpawnTargetY)
	{
		SetY(m_behavior.fSpawnTargetY);
		m_state = EnemyState::ATTACK;
	}
}

void Enemy::MothershipMove(float deltaTime, float screenWidth)
{
	float moveSpeed = 80.f;
	if (GetHealth() <= 2000)		moveSpeed = 200.f;
	else if (GetHealth() <= 4000)	moveSpeed = 130.f;

	SetX(GetX() + moveSpeed * m_fMoveDir * deltaTime);

	if (GetX() <= 0.f)
	{
		SetX(0.f);
		m_fMoveDir = 1.f;
	}
	else if (GetX() + GetRenderWidth() >= screenWidth)
	{
		SetX(screenWidth - static_cast<float>(GetRenderWidth()));
		m_fMoveDir = -1.f;
	}
}

void Enemy::MothershipAttack(float deltaTime)
{
	MothershipMove(deltaTime, 480.f);

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

		if (m_behavior.nCycleCount % 2 == 0)
			MothershipAimShot();
		else
			MothershipNWayShot();

		++m_behavior.nBurstCount;
		if (m_behavior.nBurstCount >= m_behavior.nBurstMax)
		{
			m_behavior.bResting = true;
			++m_behavior.nCycleCount;
		}
	}
}

void Enemy::MothershipAimShot()
{
	static std::vector<std::pair<float, float>> msArmsOffset =
	{
		{ 33.f,  61.f },
		{ 64.f,  81.f },
		{ 94.f,  98.f },
		{ 146.f, 97.f },
		{ 177.f, 79.f },
		{ 207.f, 60.f }
	};

	float targetX = m_gameWorld->GetPlayer()->GetX() + m_gameWorld->GetPlayer()->GetRenderWidth() / 2.f;
	float targetY = m_gameWorld->GetPlayer()->GetY() + m_gameWorld->GetPlayer()->GetRenderHeight() / 2.f;

	float bulletSpeed = 700.f;
	if (GetHealth() <= 2000)		bulletSpeed = 1000.f;
	else if (GetHealth() <= 4000)	bulletSpeed = 850.f;

	for (auto& arm : msArmsOffset)
	{
		BulletPattern::AimShot(
			m_gameWorld,
			GetX() + arm.first, GetY() + arm.second,
			targetX, targetY,
			bulletSpeed,
			GetDamage(),
			BulletType::MOTHERSHIPBULLET
		);
	}
}

void Enemy::MothershipNWayShot()
{
	float centerX = GetX() + GetRenderWidth() / 2.f;
	float centerY = GetY() + GetRenderHeight();

	int   nWay = 5;
	float speed = 400.f;
	float spread = 60.f;

	if (GetHealth() <= 2000)		{ nWay = 9; speed = 600.f; spread = 80.f; }
	else if (GetHealth() <= 4000)	{ nWay = 7; speed = 500.f; spread = 70.f; }

	BulletPattern::NWayShot(
		m_gameWorld,
		centerX, centerY,
		speed,
		nWay,
		spread,
		GetDamage(),
		BulletType::MOTHERSHIPBULLET,
		90.f
	);
}

// ===================== Dragon =====================

void Enemy::DragonSpawn(float deltaTime)
{
	SetY(GetY() + GetSpeed() * deltaTime);
	if (GetY() >= m_behavior.fSpawnTargetY)
	{
		SetY(m_behavior.fSpawnTargetY);
		m_state = EnemyState::ATTACK;
	}
}

void Enemy::DragonMove(float deltaTime, float screenWidth)
{
	// 체력 구간별 이동 속도
	float moveSpeed = 80.f;
	if (GetHealth() <= 5000)		moveSpeed = 200.f;
	else if (GetHealth() <= 10000)	moveSpeed = 130.f;

	SetX(GetX() + moveSpeed * m_fMoveDir * deltaTime);

	if (GetX() <= 0.f)
	{
		SetX(0.f);
		m_fMoveDir = 1.f;
	}
	else if (GetX() + GetRenderWidth() >= screenWidth)
	{
		SetX(screenWidth - static_cast<float>(GetRenderWidth()));
		m_fMoveDir = -1.f;
	}
}

void Enemy::DragonAttack(float deltaTime)
{
	DragonMove(deltaTime, 480.f);

	// 차징 중이면 차징 업데이트만
	if (m_behavior.bCharging)
	{
		DragonUpdateCharge(deltaTime);
		return;
	}

	// 레이저 연사 중
	if (m_behavior.nLaserBurstCount > 0)
	{
		m_behavior.fLaserBurstTimer += deltaTime;
		if (m_behavior.fLaserBurstTimer >= m_behavior.fLaserBurstInterval)
		{
			m_behavior.fLaserBurstTimer = 0.f;
			DragonFireLaser();
			--m_behavior.nLaserBurstCount;
		}
		return;
	}

	// 휴식 중
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

	// 일반 발사
	m_behavior.fBurstTimer += deltaTime;
	if (m_behavior.fBurstTimer >= m_behavior.fBurstInterval)
	{
		m_behavior.fBurstTimer = 0.f;

		// 페이즈별 패턴 사이클
		// 페이즈 1 (10000~15000) : RotateShot만
		// 페이즈 2 (5000~10000)  : RotateShot → AimShot 번갈아
		// 페이즈 3 (0~5000)      : RotateShot → AimShot → LaserShot 순환
		int phase = 1;
		if (GetHealth() <= 5000)		phase = 3;
		else if (GetHealth() <= 10000)	phase = 2;

		switch (phase)
		{
		case 1:
			DragonRotateShot();
			break;
		case 2:
			if (m_behavior.nCycleCount % 2 == 0)	DragonRotateShot();
			else									DragonAimShot();
			break;
		case 3:
		{
			int cycle = m_behavior.nCycleCount % 3;
			if (cycle == 0)	DragonRotateShot();
			else if (cycle == 1)	DragonAimShot();
			else					DragonStartCharge();	// 레이저 차징 시작
		}
		break;
		}

		++m_behavior.nBurstCount;
		if (m_behavior.nBurstCount >= m_behavior.nBurstMax)
		{
			m_behavior.bResting = true;
			++m_behavior.nCycleCount;
		}
	}
}

void Enemy::DragonRotateShot()
{
	// 체력 구간별 탄 수/속도 증가
	int   nWay = 16;
	float speed = 180.f;

	if (GetHealth() <= 5000)
	{
		nWay = 24;
		speed = 260.f;
	}
	else if (GetHealth() <= 10000)
	{
		nWay = 20;
		speed = 220.f;
	}

	BulletPattern::RotateShot(
		m_gameWorld,
		GetX() + GetRenderWidth() / 2.f,
		GetY() + GetRenderHeight() / 2.f,
		speed,
		nWay,
		360.f,
		GetDamage(),
		BulletType::DRAGONBULLET,
		m_behavior.fRotateAngle
	);
	m_behavior.fRotateAngle += 7.f;
}

void Enemy::DragonAimShot()
{
	float targetX = m_gameWorld->GetPlayer()->GetX() + m_gameWorld->GetPlayer()->GetRenderWidth() / 2.f;
	float targetY = m_gameWorld->GetPlayer()->GetY() + m_gameWorld->GetPlayer()->GetRenderHeight() / 2.f;

	// 체력 구간별 탄 수 증가 (NWay 조준샷)
	int   nWay = 3;
	float speed = 350.f;
	float spread = 20.f;

	if (GetHealth() <= 5000)
	{
		nWay = 5;
		speed = 500.f;
		spread = 30.f;
	}
	else if (GetHealth() <= 10000)
	{
		nWay = 4;
		speed = 420.f;
		spread = 25.f;
	}

	// 플레이어 방향 각도 계산 후 NWayShot으로 발사
	float dx = targetX - (GetX() + GetRenderWidth() / 2.f);
	float dy = targetY - (GetY() + GetRenderHeight() / 2.f);
	float angle = atan2f(dy, dx) * 180.f / 3.14159f;

	BulletPattern::NWayShot(
		m_gameWorld,
		GetX() + GetRenderWidth() / 2.f,
		GetY() + GetRenderHeight() / 2.f,
		speed,
		nWay,
		spread,
		GetDamage(),
		BulletType::DRAGONBULLET,
		angle
	);
}

void Enemy::DragonStartCharge()
{
	m_behavior.bCharging = true;
	m_behavior.fChargeTimer = 0.f;
}

void Enemy::DragonUpdateCharge(float deltaTime)
{
	m_behavior.fChargeTimer += deltaTime;
	if (m_behavior.fChargeTimer >= m_behavior.fChargeInterval)
	{
		// 차징 완료 → 레이저 연사 시작
		m_behavior.bCharging = false;
		m_behavior.fChargeTimer = 0.f;
		m_behavior.nLaserBurstCount = m_behavior.nLaserBurstMax;
		m_behavior.fLaserBurstTimer = 0.f;
	}
}

void Enemy::DragonFireLaser()
{
	float targetX = m_gameWorld->GetPlayer()->GetX() + m_gameWorld->GetPlayer()->GetRenderWidth() / 2.f;
	float targetY = m_gameWorld->GetPlayer()->GetY() + m_gameWorld->GetPlayer()->GetRenderHeight() / 2.f;

	// 체력 낮을수록 레이저 속도 증가
	float speed = 900.f;
	if (GetHealth() <= 5000) speed = 1200.f;

	BulletPattern::AimShot(
		m_gameWorld,
		GetX() + GetRenderWidth() / 2.f,
		GetY() + GetRenderHeight() / 2.f,
		targetX, targetY,
		speed,
		GetDamage() * 2,	// 레이저는 일반 데미지의 2배
		BulletType::DRAGONBULLET
	);
}