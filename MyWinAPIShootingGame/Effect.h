#pragma once
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

enum class BulletType;

class Effect : public GameObject
{
private:
	Sprite* m_effectSprite;
	int m_nSrcX, m_nSrcY;
	float m_fAnimDelay;

public:
	Effect(float x, float y, BulletType bulletType);

	void Render(Renderer& renderer) override;
	void Update(RECT& client, float deltaTime) override;
	void OnCollision(GameObject& other) override;
	
	void SpriteAnimation(float deltaTime);
};

