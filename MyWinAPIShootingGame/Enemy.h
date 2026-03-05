#pragma once
#include "EnemyType.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Bullet.h"
#include "BoxCollider.h"
#include "BulletPattern.h"
#include "Effect.h"

class GameWorld;

enum class EnemyState
{
	SPAWN,
	IDLE,
	ATTACK,
};

struct EnemyBehavior
{
	int nBurstCount;				// 현재까지 쏜 횟수
	int nBurstMax;					// 버스트 당 발사 횟수
	float fBurstTimer;				// 발사 간격 타이머
	float fBurstInterval;			// 발사 간격
	float fRestTimer;				// 휴식 타이머
	float fRestInterval;			// 휴식 시간
	bool bResting;					// 휴식 중 여부

	float fRotateAngle;				// 회전탄 초기 각도
	float fSpawnTargetY;			// 지정된 Y 좌표(스폰 위치에서 멈추는 지점)
	float fSineMoveTimer;			// 사인 곡 이동 타이머(Goons 전용)
	float fSineMoveAmplitude;		// 사인 곡 이동 진폭(Goons 전용)

	int nCycleCount;				// 버스트 사이클 카운터 (패턴 전환용)

	// Dragon 레이저 차징 전용
	bool  bCharging;				// 차징 중 여부
	float fChargeTimer;				// 차징 경과 시간
	float fChargeInterval;			// 차징 완료까지 걸리는 시간
	int   nLaserBurstCount;			// 레이저 연사 발사 횟수
	int   nLaserBurstMax;			// 레이저 연사 최대 횟수
	float fLaserBurstTimer;			// 레이저 연사 간격 타이머
	float fLaserBurstInterval;		// 레이저 연사 간격
};

class Enemy : public GameObject
{
private:
	Sprite* m_enemySprite;
	EnemyType m_enemyType;
	EnemyState m_state;
	EnemyBehavior m_behavior;

	int m_nSrcX;
	int m_nSrcY;

	float m_fMoveDir;				// 좌우 이동 방향 (+1 오른쪽, -1 왼쪽)

	GameWorld* m_gameWorld;

public:
	Enemy(float x = 0.f,
		float y = 0.f,
		float speed = 0.f,
		EnemyType type = EnemyType::NONE,
		GameWorld* gameWorld = nullptr
	);
	void SetBehavior();
	~Enemy() {};

	void Update(RECT& client, float deltaTime) override;
	void Render(Renderer& renderer) override;
	void OnCollision(GameObject& other) override;
	void SetMothershipSpriteByHealth();
	void OnDeath();

	void SetSprite();
	void SetWH();
	void SetState(EnemyState state) { m_state = state; }

	void EnemyAttackProcess(float deltaTime);
	void EnemySpawnProcess(float deltaTime);

	void MonsterAttack(float deltaTime);
	void MonsterSpawn(float deltaTime);
	void MonsterFire();
	void GoonsAttack(float deltaTime);
	void GoonsSpawn(float deltaTime);
	void SineMove(float deltaTime);
	void GoonsFire();
	void MothershipSpawn(float deltaTime);
	void MothershipMove(float deltaTime, float screenWidth);
	void MothershipAttack(float deltaTime);
	void MothershipAimShot();
	void MothershipNWayShot();
	void DragonSpawn(float deltaTime);
	void DragonMove(float deltaTime, float screenWidth);
	void DragonAttack(float deltaTime);
	void DragonRotateShot();
	void DragonAimShot();
	void DragonStartCharge();
	void DragonUpdateCharge(float deltaTime);
	void DragonFireLaser();
};