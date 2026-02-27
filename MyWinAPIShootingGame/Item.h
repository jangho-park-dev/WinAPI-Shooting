#pragma once
#include "GameObject.h"
#include "ResourceManager.h"
#include "BoxCollider.h"

enum class ItemType
{
	SPEEDUP,
	POWERUP,
	BULLETUP,
	HEALTH,
};

class Item : public GameObject
{
private:
	Sprite* m_itemSprite;

	ItemType m_itemType;
	int m_nSrcX;
	int m_nSrcY; // 스프라이트 시트에서의 Y 좌표(아이템 종류에 따라 달라짐)
	float m_fAnimDelay = 0.15f;

public:
	Item(float x = 0.f, float y = 0.f, float speed = 100.f, ItemType itemType = ItemType::POWERUP);
	~Item() {};

	void Update(RECT& client, float deltaTime) override;
	void Render(Renderer& renderer) override;
	void OnCollision(GameObject& other) override;
	void SpriteAnimation(float deltaTime);
	
	ItemType GetItemType() const { return m_itemType; }
};

