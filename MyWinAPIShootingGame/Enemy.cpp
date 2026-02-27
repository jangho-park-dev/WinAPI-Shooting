#include "Enemy.h"
#include "GameWorld.h"
#include "Player.h"
#include "Item.h"

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
		// 0.2초 간격으로 10발 발사 후 2초 휴식
		m_behavior.nBurstCount = 0;
		m_behavior.nBurstMax = 10;
		m_behavior.fBurstTimer = 0.f;
		m_behavior.fBurstInterval = 0.2f;
		m_behavior.fRestTimer = 0.f;
		m_behavior.fRestInterval = 2.f;
		m_behavior.bResting = false;

		// 회전샷 초기 각도
		m_behavior.fRotateAngle = 30.f;
		// 목표 Y지점
		m_behavior.fSpawnTargetY = static_cast<float>(rand() % 300);
		// 사인 곡선 이동 타이머
		m_behavior.fSineMoveTimer = 0.f;
		// 사인 곡선 이동 진폭
		m_behavior.fSineMoveAmplitude = 0.f;
		// 데미지
		SetDamage(10);
		SetHealth(300 * (m_gameWorld->GetCurrentWave() + 1));
	}
	break;
	case EnemyType::GOONS:
	{
		// 0.3초 간격으로 1발 발사 후 1초 휴식
		m_behavior.nBurstCount = 0;
		m_behavior.nBurstMax = 1;
		m_behavior.fBurstTimer = 0.f;
		m_behavior.fBurstInterval = 0.3f;
		m_behavior.fRestTimer = 0.f;
		m_behavior.fRestInterval = 1.f;
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
		SetHealth(30 * (m_gameWorld->GetCurrentWave() + 1));
	}
	break;
	case EnemyType::MOTHERSHIP:
	{
		// 0.2초 간격으로 발사, 7발 발사 후 2초 휴식
		m_behavior.nBurstCount = 0;
		m_behavior.nBurstMax = 7;
		m_behavior.fBurstTimer = 0.f;
		m_behavior.fBurstInterval = 0.2f;
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
		SetDamage(10);
		SetHealth(1000);
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
	case EnemyState::SPAWN:		EnemySpawnProcess(deltaTime);		break;
	case EnemyState::ATTACK:		EnemyAttackProcess(deltaTime);		break;
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
	{
		renderer.DrawSprite(
			*m_enemySprite,
			drawX, drawY
		);
	}
}

void Enemy::OnCollision(GameObject& other)
{
	if (!IsActive())	return;

	SetMothershipSpriteByHealth();
	// TODO : 피격 사운드
}

void Enemy::SetMothershipSpriteByHealth()
{
	// TODO : 하드코딩 제거
	if (m_enemyType == EnemyType::MOTHERSHIP)
	{
		if (900 < GetHealth() && GetHealth() <= 1000)
		{
			m_nSrcX = 0;
			m_nSrcY = 0;
		}
		else if (800 < GetHealth() && GetHealth() <= 900)
		{
			m_nSrcX = 240;
			m_nSrcY = 0;
		}
		else if (600 < GetHealth() && GetHealth() <= 800)
		{
			m_nSrcX = 480;
			m_nSrcY = 0;
		}
		else if (300 < GetHealth() && GetHealth() <= 600)
		{
			m_nSrcX = 0;
			m_nSrcY = 128;
		}
		else if (0 < GetHealth() && GetHealth() <= 300)
		{
			m_nSrcX = 240;
			m_nSrcY = 128;
		}
	}
}

void Enemy::OnDeath()
{
	ItemType dropType = ItemType::SPEEDUP;
	switch (rand() % 4)
	{
	case 0:	dropType = ItemType::SPEEDUP;		break;
	case 1:	dropType = ItemType::POWERUP;	break;
	case 2:	dropType = ItemType::BULLETUP;	break;
	case 3:	dropType = ItemType::HEALTH;		break;
	}
	if (rand() % 100 < 30)	// 30% 확률로 아이템 드롭
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
	/*float fLeftGunX = GetX() + 12.f;
	float fRightGunX = GetX() + GetRenderWidth() - 18.f;
	float fGunY = GetY() + GetRenderHeight() + 10.f;*/

	float targetX = m_gameWorld->GetPlayer()->GetX() + m_gameWorld->GetPlayer()->GetRenderWidth() / 2.f;
	float targetY = m_gameWorld->GetPlayer()->GetY() + m_gameWorld->GetPlayer()->GetRenderHeight() / 2.f;

	int damage = GetDamage();

	// 회전샷
	BulletPattern::RotateShot(
		m_gameWorld,
		GetX() + GetRenderWidth() / 2, GetY() + GetRenderHeight() / 2,
		200.f,
		20,
		360.f,
		damage,
		BulletType::MONSTERBULLET,
		m_behavior.fRotateAngle
	);
	m_behavior.fRotateAngle += 5.f; // 바로 다음 발사 각도 업데이트
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
	float fGunY = GetY() + GetRenderHeight() + 10.f;

	float targetX = m_gameWorld->GetPlayer()->GetX() + m_gameWorld->GetPlayer()->GetRenderWidth() / 2.f;
	float targetY = m_gameWorld->GetPlayer()->GetY() + m_gameWorld->GetPlayer()->GetRenderHeight() / 2.f;

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
		MothershipAimShot();


		++m_behavior.nBurstCount;
		if (m_behavior.nBurstCount >= m_behavior.nBurstMax)	m_behavior.bResting = true;
	}
}

void Enemy::MothershipAimShot()
{
	static std::vector<std::pair<float, float>> msArmsOffset;
	msArmsOffset =
	{
		// 좌측
		{33.f, 61.f},
		{64.f, 81.f},
		{94.f, 98.f},
		// 우측
		{146.f, 97.f},
		{177.f, 79.f},
		{207.f, 60.f}
	};

	float targetX = m_gameWorld->GetPlayer()->GetX() + m_gameWorld->GetPlayer()->GetRenderWidth() / 2.f;
	float targetY = m_gameWorld->GetPlayer()->GetY() + m_gameWorld->GetPlayer()->GetRenderHeight() / 2.f;

	int damage = GetDamage();

	// 조준샷
	for (auto arm : msArmsOffset)
	{
		BulletPattern::AimShot(
			m_gameWorld,
			GetX() + arm.first, GetY() + arm.second,
			targetX, targetY,
			800.f,
			damage,
			BulletType::MOTHERSHIPBULLET
		);
	}
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


