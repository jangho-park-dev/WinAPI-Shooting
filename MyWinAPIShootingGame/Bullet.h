#pragma once
#include "Renderer.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "BoxCollider.h"

enum class BulletType
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
	float m_fDirX, m_fDirY;
	
public:
	Bullet(float x, float y, float speed, int damage, BulletType bulletType);
	~Bullet() {};

	void Update(RECT& client, float deltaTime) override;
	void Render(Renderer& renderer) override;
	void OnCollision(GameObject& other) override;

	BulletType GetBulletType() const { return m_bulletType; }
	void SetDirection(float angle);
};

