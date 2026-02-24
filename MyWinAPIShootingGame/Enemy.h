#pragma once
#include "ResourceManager.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Bullet.h"
#include "BoxCollider.h"
#include "BulletPattern.h"

class GameWorld;

enum class EnemyType
{
	NONE,
	MONSTER,
	MOTHERSHIP,
	DRAGON,
	GOONS,
};

enum class EnemyState
{
	SPAWN,
	IDLE,
	ATTACK,
};

class Enemy : public GameObject
{
private:
	Sprite* m_sprite;
	EnemyType m_enemyType;
	EnemyState m_state;

	float m_fSpawnTargetY;	// 지정된 Y 좌표 (스폰 위치에서 멈추는 지점)
	
	int m_nBurstCount;		// 현재까지 쏜 횟수
	int m_nBurstMax;			// 버스트당 발사 횟수
	float m_fBurstTimer;		// 발사 간격 타이머
	float m_fBurstInterval;	// 발사 간격
	float m_fRestTimer;		// 휴식 타이머
	float m_fRestInterval;		// 휴식 시간
	bool m_bResting;			// 휴식 중 여부

	GameWorld* m_gameWorld;

public:
	Enemy(float x = 0.f,
		float y = 0.f,
		float speed = 0.f,
		EnemyType type = EnemyType::NONE,
		GameWorld* gameWorld = nullptr
	);
	~Enemy() {};

	void Update(RECT& client, float deltaTime) override;
	void EnemyAttackProcess(float deltaTime);
	void EnemySpawnProcess(float deltaTime);
	void Render(Renderer& renderer) override;
	void OnCollision(GameObject& other) override;

	void SetSprite();
	void SetWH();
	void SetState(EnemyState state) { m_state = state; }

	void MonsterAttack(float deltaTime);
	void MonsterSpawn(float deltaTime);
	void MonsterFire();
	void GoonsAttack(float deltaTime);
	void GoonsSpawn(float deltaTime);
	void GoonsFire();
	void MothershipSpawn(float deltaTime);
	void MothershipAttack(float deltaTime);
	void MothershipFire();
	void DragonAttack(float deltaTime);
	void DragonSpawn(float deltaTime);
	void DragonFire();
};

