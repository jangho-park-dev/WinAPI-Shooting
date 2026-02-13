#pragma once
#include "ResourceManager.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Bullet.h"
#include "BoxCollider.h"

enum class EnemyType
{
	NONE,
	MONSTER,
	MOTHERSHIP,
	DRAGON,
	GOONS,
};

class Enemy : public GameObject
{
private:
	Sprite* m_sprite;
	EnemyType m_enemyType;

public:
	Enemy(float x = 0.f, float y = 0.f, float speed = 0.f, EnemyType type = EnemyType::NONE);
	~Enemy() {};

	void Update(RECT& client, float deltaTime) override;
	void Render(Renderer& renderer) override;
	void OnCollision(GameObject& other) override;

	void SetSprite();
	void SetWH();
};

