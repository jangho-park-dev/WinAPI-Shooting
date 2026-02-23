#pragma once
#include "ResourceManager.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Bullet.h"
#include "BoxCollider.h"

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

	float m_fTargetY;
	float m_fFireTimer;
	float m_fFireInterval;

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

