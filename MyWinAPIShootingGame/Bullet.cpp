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


	switch (m_bulletType)
	{
	case BulletType::PLAYERBULLET:
		m_nSrcX = 0;
		m_nSrcY = 32;
		break;
	case BulletType::MONSTERBULLET:
		m_nSrcX = 0;
		m_nSrcY = 8;
		break;
	case BulletType::MOTHERSHIPBULLET:
		m_nSrcX = 0;
		m_nSrcY = 16;
		break;
	case BulletType::DRAGONBULLET:
		m_nSrcX = 0;
		m_nSrcY = 24;
		break;
	}
}

void Bullet::Update(RECT& client, float deltaTime)
{
	if (!IsActive())	return;
	
	float move = 0.f;
	// 총알 이동
	switch (m_bulletType)
	{
	case BulletType::PLAYERBULLET:
		move = GetSpeed() * deltaTime;
		SetY(GetY() - move);

		// 화면 밖으로 나가면 비활성화
		if (GetY() + GetHeight() < 0)	SetActive(false);
		break;

	case BulletType::MONSTERBULLET:
		move = GetSpeed() * deltaTime;
		SetY(GetY() + move);

		if (GetY() + GetHeight() > client.bottom)	SetActive(false);
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

