#pragma once
#include "framework.h"
#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"


class Background : public GameObject
{
private:
	Sprite* m_backgroundSprite;
	SpriteID m_spriteID;

public:
	Background(float x = 0.f, float y = 0.f, SpriteID spriteID = SpriteID::SPRITE_TITLEBACKGROUND)
		: m_spriteID(spriteID)
	{
		m_backgroundSprite = ResourceManager::GetInstance().GetSprite(spriteID);
		SetType(GameObjectType::BACKGROUND);
		SetLayer(GameObjectLayer::BACKGROUND_LAYER);
		SetActive(true);
		SetX(x);
		SetY(y);
		SetSrcWidth(m_backgroundSprite->GetWidth());
		SetSrcHeight(m_backgroundSprite->GetHeight());
		SetRenderSize(GetSrcWidth(), GetSrcHeight());
	}
	
	void Render(Renderer& renderer) override
	{
		renderer.DrawSprite(*m_backgroundSprite, static_cast<int>(GetX()), static_cast<int>(GetY()));
	}

	void Update(RECT& client, float deltaTime) override
	{
		if (m_spriteID == SpriteID::SPRITE_GAMEBACKGROUND)
		{
			float move = 100.f * deltaTime;
			SetY(GetY() + move);

			if (GetY() >= 640.f)	SetY(-640.f);
		}
	}

	void OnCollision(GameObject& other) override {}
};

