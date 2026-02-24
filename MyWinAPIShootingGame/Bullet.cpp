#include "Bullet.h"
#include <cmath>

Bullet::Bullet(float x, float y, float speed, int damage, BulletType bulletType)
	: m_bulletType(bulletType)
{
	m_bulletSprite = ResourceManager::GetInstance().GetSprite(SpriteID::SPRITE_BULLET);
	SetType(GameObjectType::BULLET);
	SetLayer(GameObjectLayer::BULLET_LAYER);
	SetActive(true);
	SetX(x);
	SetY(y);
	SetSpeed(speed);
	SetWidth(8);
	SetHeight(8);
	SetHealth(1);
	SetDamage(damage);
	SetCollider(new BoxCollider(this));

	m_nSrcX = 0;
	m_nSrcY = 0;
	
	m_fDirX = 0.f;
	m_fDirY = 0.f;

	switch (m_bulletType)
	{
	case BulletType::PLAYERBULLET:			m_nSrcY = 32;	m_fDirY = -1.f;	break;
	case BulletType::MONSTERBULLET:		m_nSrcY = 8;	break;
	case BulletType::MOTHERSHIPBULLET:	m_nSrcY = 16;	break;
	case BulletType::DRAGONBULLET:		m_nSrcY = 24;	break;
	}
}

void Bullet::Update(RECT& client, float deltaTime)
{
	if (!IsActive())	return;
	
	// ÃÑ¾Ë ÀÌµ¿
	SetX(GetX() + GetSpeed() * deltaTime * m_fDirX);
	SetY(GetY() + GetSpeed() * deltaTime * m_fDirY);
	if (GetY() + GetHeight() < 0 || GetY() > client.bottom ||
		GetX() + GetWidth() < 0 || GetX() > client.right)
		SetActive(false);
}

void Bullet::Render(Renderer& renderer)
{
	if (!IsActive())	return;

	int drawX = static_cast<int>(GetX() + 0.5f);
	int drawY = static_cast<int>(GetY() + 0.5f);

	renderer.DrawSprite(
		*m_bulletSprite, 
		drawX, drawY,
		m_nSrcX, m_nSrcY,
		GetWidth(), GetHeight()
	);
}

void Bullet::OnCollision(GameObject& other)
{
	if (!IsActive())	return;
	
	if (m_bulletType == BulletType::PLAYERBULLET && other.GetType() == GameObjectType::PLAYER) return;
	if (m_bulletType != BulletType::PLAYERBULLET && other.GetType() == GameObjectType::ENEMY) return;

	other.SetHealth(other.GetHealth() - GetDamage());
	if (other.GetHealth() <= 0)
	{
		other.SetHealth(0);
		other.SetActive(false);
	}
	std::cout << "Bullet hit type_num " << static_cast<int>(other.GetType())
		<< ",\tother hp : " << other.GetHealth() << std::endl;

	SetActive(false);
}

void Bullet::SetDirection(float angle)
{
	float rad = angle * 3.14159f / 180.f;
	m_fDirX = cosf(rad);
	m_fDirY = sinf(rad);
}
