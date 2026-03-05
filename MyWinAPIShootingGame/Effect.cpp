#include "Effect.h"
#include "Bullet.h"

Effect::Effect(float x, float y, BulletType bulletType)
{
	if (bulletType == BulletType::PLAYERBULLET)
	{
		m_effectSprite = ResourceManager::GetInstance().GetSprite(SpriteID::SPRITE_PLAYERBULLETEFFECT);
		m_effectSprite->SetSpriteSizeMultiplier(1.f);
	}
	else
	{
		m_effectSprite = ResourceManager::GetInstance().GetSprite(SpriteID::SPRITE_ENEMYBULLETEFFECT);
		m_effectSprite->SetSpriteSizeMultiplier(0.6f);
	}
	SetType(GameObjectType::EFFECT);
	SetLayer(GameObjectLayer::EFFECT_LAYER);
	SetActive(true);
	SetX(x);
	SetY(y);
	SetSpeed(0.f);
	SetSrcWidth(64);
	SetSrcHeight(64);
	SetHealth(1);
	SetRenderSize(
		static_cast<int>(GetSrcWidth() * m_effectSprite->GetSpriteSizeMultiplier()),
		static_cast<int>(GetSrcHeight() * m_effectSprite->GetSpriteSizeMultiplier())
	);
	SetDamage(0);
	SetCollider(nullptr);

	m_nSrcX = 0;
	m_nSrcY = 0;
	m_fAnimDelay = 0.03f;
}

void Effect::Render(Renderer& renderer)
{
	if (!IsActive())	return;

	int drawX = static_cast<int>(GetX() + 0.5f);
	int drawY = static_cast<int>(GetY() + 0.5f);

	renderer.DrawSprite(
		*m_effectSprite,
		drawX, drawY,
		m_nSrcX, m_nSrcY,
		GetSrcWidth(), GetSrcHeight(),
		GetRenderWidth(), GetRenderHeight()
	);
}

void Effect::Update(RECT& client, float deltaTime)
{
	SpriteAnimation(deltaTime);
}

void Effect::OnCollision(GameObject& other)
{

}

void Effect::SpriteAnimation(float deltaTime)
{
	m_fAnimDelay -= deltaTime;
	if (m_fAnimDelay <= 0.f)
	{
		m_fAnimDelay = 0.1f;
		m_nSrcX += 64;
		if (m_nSrcX >= m_effectSprite->GetWidth())
		{
			m_nSrcX = 0;
			// 여기서 이펙트 오브젝트 없애야 됨
			SetActive(false);
		}
	}
}
