#pragma once
#include "framework.h"
#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"


class Background : public GameObject
{
private:
	Sprite* m_backgroundSprite;

public:
	Background()
	{
		m_backgroundSprite = ResourceManager::GetInstance().GetSprite(SpriteID::SPRITE_BACKGROUND);
		SetType(GameObjectType::BACKGROUND);
		SetLayer(GameObjectLayer::BACKGROUND_LAYER);
		SetActive(true);
		SetX(0.f);
		SetY(0.f);
		SetWidth(m_backgroundSprite->GetWidth());
		SetHeight(m_backgroundSprite->GetHeight());
	}
	
	void Render(Renderer& renderer)
	{
		renderer.DrawSprite(*m_backgroundSprite, static_cast<int>(GetX()), static_cast<int>(GetY()));
	}

	void Update(RECT& client, float deltaTime) override {}
	void OnCollision(GameObject& other) override {}
};

