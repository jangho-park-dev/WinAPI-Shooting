#include "Bullet.h"
#include <cmath>

Bullet::Bullet(float x, float y, float speed, int damage, BulletType bulletType)
	: m_bulletType(bulletType)
{
	m_bulletSprite = ResourceManager::GetInstance().GetSprite(SpriteID::SPRITE_BULLET);

	if (m_bulletType == BulletType::MOTHERSHIPBULLET)
		m_bulletSprite->SetSpriteSizeMultiplier(2.3f);
	else if (m_bulletType == BulletType::DRAGONBULLET)
		m_bulletSprite->SetSpriteSizeMultiplier(2.f);
	else
		m_bulletSprite->SetSpriteSizeMultiplier(1.3f);
	
	SetType(GameObjectType::BULLET);
	SetLayer(GameObjectLayer::BULLET_LAYER);
	SetActive(true);
	SetX(x);
	SetY(y);
	SetSpeed(speed);
	SetSrcWidth(8);
	SetSrcHeight(8);
	SetRenderSize(
		static_cast<int>(GetSrcWidth() * m_bulletSprite->GetSpriteSizeMultiplier()),
		static_cast<int>(GetSrcHeight() * m_bulletSprite->GetSpriteSizeMultiplier())
	);
	SetHealth(1);
	SetDamage(damage);
	SetCollider(new BoxCollider(this));

	m_nSrcX = 0;
	m_nSrcY = 0;
	
	m_fDirX = 0.f;
	m_fDirY = 0.f;

	m_fAnimDelay = 0.15f;

	switch (m_bulletType)
	{
	case BulletType::PLAYERBULLET:			m_nSrcY = 32;	m_fDirY = -1.f;	break;
	case BulletType::GOONSBULLET:			m_nSrcY = 0;	break;
	case BulletType::MONSTERBULLET:		m_nSrcY = 8;	break;
	case BulletType::MOTHERSHIPBULLET:	m_nSrcY = 16;	break;
	case BulletType::DRAGONBULLET:		m_nSrcY = 24;	break;
	}
}

void Bullet::Update(RECT& client, float deltaTime)
{
	if (!IsActive())	return;

	SpriteAnimation(deltaTime);
	
	// 총알 이동
	SetX(GetX() + GetSpeed() * deltaTime * m_fDirX);
	SetY(GetY() + GetSpeed() * deltaTime * m_fDirY);
	if (GetY() + GetSrcHeight() < 0 || GetY() > client.bottom ||
		GetX() + GetSrcWidth() < 0 || GetX() > client.right)
		SetActive(false);
}

void Bullet::SpriteAnimation(float deltaTime)
{
	if (m_bulletType == BulletType::PLAYERBULLET)	return; // 플레이어 총알은 애니메이션 없음

	m_fAnimDelay -= deltaTime;
	if (m_fAnimDelay <= 0.f)
	{
		m_fAnimDelay = 0.15f;
		m_nSrcX += 8;
		if (m_nSrcX >= m_bulletSprite->GetWidth())
			m_nSrcX = 0;
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
		GetSrcWidth(), GetSrcHeight(),
		GetRenderWidth(), GetRenderHeight()
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
		other.OnDeath();
		other.SetActive(false);
	}

	SetActive(false);
}

void Bullet::SetDirection(float angle)
{
	float rad = angle * 3.14159f / 180.f;
	m_fDirX = cosf(rad);
	m_fDirY = sinf(rad);
}
