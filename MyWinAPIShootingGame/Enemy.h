#pragma once
#include "EnemyType.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Bullet.h"
#include "BoxCollider.h"
#include "BulletPattern.h"

class GameWorld;

enum class EnemyState
{
	SPAWN,
	IDLE,
	ATTACK,
};

struct EnemyBehavior
{
	int nBurstCount;					// 현재까지 쏜 횟수	
	int nBurstMax;					// 버스트 당 발사 횟수
	float fBurstTimer;				// 발사 간격 타이머
	float fBurstInterval;			// 발사 간격
	float fRestTimer;				// 휴식 타이머
	float fRestInterval;				// 휴식 시간
	bool bResting;					// 휴식 중 여부

	float fRotateAngle;				// 회전샷 초기 각도
	float fSpawnTargetY;			// 지정된 Y 좌표(스폰 위치에서 멈추는 지점)
	float fSineMoveTimer;			// 사인 곡선 이동 타이머(Goons 전용)
	float fSineMoveAmplitude;	// 사인 곡선 이동 진폭(Goons 전용)
};

class Enemy : public GameObject
{
private:
	Sprite* m_sprite;
	EnemyType m_enemyType;
	EnemyState m_state;
	EnemyBehavior m_behavior;

	int m_nSrcX;							// 스프라이트 시트에서의 X 좌표(Goons만 랜덤값)

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
	void MothershipAttack(float deltaTime);
	void MothershipFire();
	void DragonAttack(float deltaTime);
	void DragonSpawn(float deltaTime);
	void DragonFire();
};

