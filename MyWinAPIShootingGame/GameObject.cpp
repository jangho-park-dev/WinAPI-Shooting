#include "GameObject.h"
#include "Collider.h"

GameObject::GameObject()
{
	m_type = GameObjectType::NONE;
	m_layer = GameObjectLayer::NONE_LAYER;
	m_bActive = true;
	m_fX = 0.f;
	m_fY = 0.f;
	m_fSpeed = 0.f;
	m_nWidth = 0;
	m_nHeight = 0;
	m_nHealth = 0;
	m_nDamage = 0;
	m_collider = nullptr;
}

GameObject::~GameObject()
{
	delete m_collider;
}

RECT GameObject::GetAABB() const
{
	return {
		static_cast<LONG>(GetX()),
		static_cast<LONG>(GetY()),
		static_cast<LONG>(GetX() + static_cast<float>(GetWidth())),
		static_cast<LONG>(GetY() + static_cast<float>(GetHeight()))
	};
}

