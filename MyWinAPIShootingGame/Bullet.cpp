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
	m_fDirection = 1.f;

	switch (m_bulletType)
	{
	case BulletType::PLAYERBULLET:			m_nSrcY = 32;	m_fDirection = -1.f;	break;
	case BulletType::MONSTERBULLET:		m_nSrcY = 8;	break;
	case BulletType::MOTHERSHIPBULLET:	m_nSrcY = 16;	break;
	case BulletType::DRAGONBULLET:		m_nSrcY = 24;	break;
	}
}

void Bullet::Update(RECT& client, float deltaTime)
{
	if (!IsActive())	return;
	
	// ÃÑ¾Ë ÀÌµ¿
	SetY(GetY() + GetSpeed() * deltaTime * m_fDirection);
	if (GetY() + GetHeight() < 0 || GetY() > client.bottom)
		SetActive(false);

	switch (m_bulletType)
	{
	case BulletType::PLAYERBULLET:
	{

	}
	break;

	case BulletType::MONSTERBULLET:
	{

	}
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
	if (other.GetType() == GameObjectType::BULLET)	return;

	SetActive(false);

	other.SetHealth(other.GetHealth() - GetDamage());

	if (other.GetHealth() <= 0)
	{
		other.SetHealth(0);
		other.SetActive(false);
	}

	std::cout << "Bullet collided with type_num " << static_cast<int>(other.GetType()) 
		<< ", other health: " << other.GetHealth() << std::endl;
}

