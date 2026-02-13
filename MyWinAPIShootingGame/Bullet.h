#pragma once
#include "Renderer.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "BoxCollider.h"

enum BulletType
{
	PLAYERBULLET,
	MONSTERBULLET,
	MOTHERSHIPBULLET,
	DRAGONBULLET,
};

class Bullet : public GameObject
{
private:
	Sprite* m_bulletSprite;
	BulletType m_bulletType;
	int m_nSrcX, m_nSrcY;
	
public:
	Bullet(float x = 0.f, float y = 0.f, float speed = 500.f, BulletType bulletType = BulletType::PLAYERBULLET);
	~Bullet() {};

	void Update(RECT& client, float deltaTime) override;
	void Render(Renderer& renderer) override;
	void OnCollision(GameObject& other) override;
};

