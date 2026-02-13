#pragma once
#include "ResourceManager.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Bullet.h"
#include "BoxCollider.h"

enum class Keystates
{
	LEFT,
	RIGHT,
	UP,
	DOWN
};

enum class TrailOrder
{
	TL, // 좌상
	TR, // 우상
	BL, // 좌하
	BR  // 우하
};

struct Trail
{
	float x;
	float y;
	float life;
	float maxLife;
	TrailOrder order;
};

class Player : public GameObject
{
private:
	bool m_bLeft, m_bRight, m_bUp, m_bDown;

	bool m_bIdle;
	float m_fIdleTimer;
	float m_fTrailTimer;
	int m_nTrailSpriteIndex;
	Sprite* m_playerSprite;
	Sprite* m_playerTrailSprite;
	Sprite* m_playerOutlineSprite;
	std::vector<Trail> m_trails;

public:
	Player(float x = 240, float y = 600, float speed = 300);
	virtual ~Player() {};
	void Update(RECT& client, float deltaTime) override;
	void Render(Renderer& renderer) override;
	void OnCollision(GameObject& other) override;
	
	void Move(RECT& client, float deltaTime);
	void UpdateTrails(float deltaTime);


	void SetDirection(Keystates key, bool tf);

	// trail 스프라이트 시트의 srcX, srcY 좌표 반환
	int GetSrcX(TrailOrder o) const { return (o == TrailOrder::TR || o == TrailOrder::BR) ? 16 : 0; }
	int GetSrcY(TrailOrder o) const { return (o == TrailOrder::BL || o == TrailOrder::BR) ? 16 : 0; }
};
