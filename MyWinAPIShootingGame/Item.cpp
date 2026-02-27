#include "Item.h"

Item::Item(float x, float y, float speed, ItemType itemType)
	: m_itemSprite(ResourceManager::GetInstance().GetSprite(SpriteID::SPRITE_ITEM))
	, m_itemType(itemType)
{
	m_itemSprite->SetSpriteSizeMultiplier(2.5f);

	SetType(GameObjectType::ITEM);
	SetLayer(GameObjectLayer::ITEM_LAYER);
	SetActive(true);
	SetX(x);
	SetY(y);
	SetSpeed(speed);
	SetSrcWidth(8);
	SetSrcHeight(8);
	SetRenderSize(
		static_cast<int>(GetSrcWidth() * m_itemSprite->GetSpriteSizeMultiplier()),
		static_cast<int>(GetSrcHeight() * m_itemSprite->GetSpriteSizeMultiplier())
	);
	SetHealth(1);
	SetDamage(0);

	SetCollider(new BoxCollider(this));

	m_nSrcX = 0;
	m_nSrcY = 0;
	
	switch (m_itemType)
	{
	case ItemType::SPEEDUP:	m_nSrcY = 0;	break;
	case ItemType::POWERUP:	m_nSrcY = 24;	break;
	case ItemType::BULLETUP:	m_nSrcY = 32;	break;
	case ItemType::HEALTH:		m_nSrcY = 40;	break;
	}
}

void Item::Update(RECT& client, float deltaTime)
{
	if (!IsActive())	return;
	SetY(GetY() + GetSpeed() * deltaTime);
	if (GetY() > client.bottom)	SetActive(false);
	SpriteAnimation(deltaTime);
}

void Item::Render(Renderer& renderer)
{
	renderer.DrawSprite(
		*m_itemSprite,
		static_cast<int>(GetX()),
		static_cast<int>(GetY()),
		m_nSrcX, m_nSrcY,
		GetSrcWidth(), GetSrcHeight(),
		GetRenderWidth(), GetRenderHeight()
	);
}

void Item::OnCollision(GameObject& other)
{
	if (other.GetType() == GameObjectType::PLAYER)	SetActive(false);
}

void Item::SpriteAnimation(float deltaTime)
{
	m_fAnimDelay -= deltaTime;
	if (m_fAnimDelay <= 0.f)
	{
		m_fAnimDelay = 0.15f;
		m_nSrcX += 8;
		if (m_nSrcX >= m_itemSprite->GetWidth())
			m_nSrcX = 0;
	}
}

