#include "Bullet.h"

Bullet::Bullet(float x, float y, float speed, BulletType bulletType)
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
	//SetHealth(1);
	//SetDamage(10);
	SetCollider(new BoxCollider(this));

	m_nSrcX = 0;
	m_nSrcY = 0;

	switch (m_bulletType)
	{
	case BulletType::PLAYERBULLET:		m_nSrcY = 32;	break;
	case BulletType::MONSTERBULLET:		m_nSrcY = 8;	break;
	case BulletType::MOTHERSHIPBULLET:	m_nSrcY = 16;	break;
	case BulletType::DRAGONBULLET:		m_nSrcY = 24;	break;
	}
}

void Bullet::Update(RECT& client, float deltaTime)
{
	if (!IsActive())	return;

	float nDir = 1.f;
	if (m_bulletType == BulletType::PLAYERBULLET)	nDir *= -1;
	
	// ÃÑ¾Ë ÀÌµ¿
	SetY(GetY() + GetSpeed() * deltaTime * nDir);
	if (GetY() + GetHeight() < 0 || GetY() > client.bottom)
		SetActive(false);

	std::cout << "Bullet y : " << GetY() << std::endl;
	switch (m_bulletType)
	{
	case BulletType::PLAYERBULLET:

		break;

	case BulletType::MONSTERBULLET:

		break;
	}
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
	if (other.GetLayer() != GameObjectLayer::ENEMY_LAYER)		return;
	
	SetActive(false);
	other.SetHealth(other.GetHealth() - GetDamage());
}

