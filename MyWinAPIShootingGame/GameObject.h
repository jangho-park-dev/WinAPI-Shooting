#pragma once
#include "Renderer.h"

class Collider;

enum class GameObjectType
{
	NONE, 
	BACKGROUND,
	PLAYER,
	ENEMY,
	POWERUP,
	BULLET
};

enum class GameObjectLayer
{
	NONE_LAYER,
	BACKGROUND_LAYER,
	PLAYER_LAYER,
	ENEMY_LAYER,
	POWERUP_LAYER,
	BULLET_LAYER,
	UI_LAYER
};

class GameObject
{
private:
	GameObjectType m_type;
	GameObjectLayer m_layer;
	bool m_bActive;
	float m_fX, m_fY;
	float m_fSpeed;
	int m_nWidth, m_nHeight;
	int m_nHealth;
	int m_nDamage;

	Collider* m_collider;

public:
	GameObject();
	virtual ~GameObject();

	virtual void Update(RECT& client, float deltaTime) = 0;
	virtual void Render(Renderer& renderer) = 0;
	virtual void OnCollision(GameObject& other) = 0;

	GameObjectType GetType() const { return m_type; }
	GameObjectLayer GetLayer() const { return m_layer; }
	bool IsActive() const { return m_bActive; }
	float GetX() const { return m_fX; }
	float GetY() const { return m_fY; }
	float GetSpeed() const { return m_fSpeed; }
	int GetWidth() const { return m_nWidth; }
	int GetHeight() const { return m_nHeight; }
	int GetHealth() const { return m_nHealth; }
	int GetDamage() const { return m_nDamage; }
	Collider* GetCollider() const { return m_collider; }
	RECT GetAABB() const;

	void SetType(GameObjectType type) { m_type = type; }
	void SetLayer(GameObjectLayer layer) { m_layer = layer; }
	void SetActive(bool active) { m_bActive = active; }
	void SetX(float x) { m_fX = x; }
	void SetY(float y) { m_fY = y; }
	void SetSpeed(float speed) { m_fSpeed = speed; }
	void SetWidth(int width) { m_nWidth = width; }
	void SetHeight(int height) { m_nHeight = height; }
	void SetHealth(int health) { m_nHealth = health; }
	void SetDamage(int damage) { m_nDamage = damage; }
	void SetCollider(Collider* collider) { m_collider = collider; }
};

