#pragma once
#include "Player.h"
#include "Item.h"

Player::Player(float x, float y, float speed)
{
	m_playerSprite = ResourceManager::GetInstance().GetSprite(SpriteID::SPRITE_PLAYER);
	m_playerTrailSprite = ResourceManager::GetInstance().GetSprite(SpriteID::SPRITE_PLAYERTRAIL);
	m_playerOutlineSprite = ResourceManager::GetInstance().GetSprite(SpriteID::SPRITE_PLAYEROUTLINE);
	m_playerSprite->SetSpriteSizeMultiplier(1.2f);
	m_playerTrailSprite->SetSpriteSizeMultiplier(1.2f);
	m_playerOutlineSprite->SetSpriteSizeMultiplier(1.2f);

	SetType(GameObjectType::PLAYER);
	SetLayer(GameObjectLayer::PLAYER_LAYER);
	SetActive(true);
	SetX(x);
	SetY(y);
	SetSpeed(speed);
	SetSrcWidth(m_playerOutlineSprite->GetWidth());
	SetSrcHeight(m_playerOutlineSprite->GetHeight());
	SetRenderSize(
		static_cast<int>(GetSrcWidth() * m_playerOutlineSprite->GetSpriteSizeMultiplier()),
		static_cast<int>(GetSrcHeight() * m_playerOutlineSprite->GetSpriteSizeMultiplier())
	);
	SetHealth(100000);
	SetDamage(10);
	SetCollider(new BoxCollider(this));

	m_bLeft = false;
	m_bRight = false;
	m_bUp = false;
	m_bDown = false;

	m_bIdle = true;
	m_fIdleTimer = 0.f;
	m_fTrailTimer = 0.f;
	m_nTrailSpriteIndex = 0;

	SetNumberOfBullets(1);
	m_fBaseSpeed = speed;
}

void Player::Update(RECT& client, float deltaTime)
{
	m_fIdleTimer += deltaTime;
	if (m_fIdleTimer >= 0.12f)
	{
		m_bIdle = true;
	}

	Move(client, deltaTime);
	UpdateTrails(deltaTime);
}

void Player::Move(RECT& client, float deltaTime)
{
	float move = GetSpeed() * deltaTime;

	if (m_bLeft || m_bRight || m_bUp || m_bDown)
	{
		m_bIdle = false;
		m_fIdleTimer = 0.f;
	}

	if (m_bLeft)	SetX(GetX() - move);
	if (m_bRight)	SetX(GetX() + move);
	if (m_bUp)		SetY(GetY() - move);
	if (m_bDown)	SetY(GetY() + move);

	if (GetX() < 0.f)	SetX(0.f);
	if (GetY() < 0.f)	SetY(0.f);
	if (GetX() + GetRenderWidth() > client.right)
		SetX(static_cast<float>(client.right - GetRenderWidth()));
	if (GetY() + GetRenderHeight() > client.bottom)
		SetY(static_cast<float>(client.bottom - GetRenderHeight()));
}

void Player::UpdateTrails(float deltaTime)
{
	if (m_bIdle)
	{
		m_trails.clear();
		return;
	}

	m_fTrailTimer += deltaTime;
	if (m_fTrailTimer >= 0.03f)   // 0.03초마다
	{
		m_fTrailTimer = 0.f;

		Trail trail;
		trail.x = GetX();
		trail.y = GetY();
		trail.life = 0.09f;
		trail.maxLife = 0.09f;
		trail.order = static_cast<TrailOrder>(m_nTrailSpriteIndex);
		m_trails.push_back(trail);
		m_nTrailSpriteIndex = (m_nTrailSpriteIndex + 1) % 4;

		m_trails.erase(
			std::remove_if(m_trails.begin(), m_trails.end(),
				[](const Trail& t) { return t.life <= 0.f; }),
			m_trails.end()
		);
	}

	for (auto& t : m_trails)
		t.life -= deltaTime;
}

void Player::Render(Renderer& renderer)
{
	int drawX = static_cast<int>(GetX() + 0.5f);
	int drawY = static_cast<int>(GetY() + 0.5f);

	// 트레일 먼저
	for (auto& t : m_trails)
	{
		int sx = GetSrcX(t.order);
		int sy = GetSrcY(t.order);

		// sprite는 트레일, 크기는 player 본체 크기로
		renderer.DrawSprite(
			*m_playerTrailSprite,
			static_cast<int>(t.x),
			static_cast<int>(t.y),
			sx, sy,
			m_playerSprite->GetWidth(), m_playerSprite->GetHeight(),
			static_cast<int>(m_playerSprite->GetWidth() * m_playerSprite->GetSpriteSizeMultiplier()),
			static_cast<int>(m_playerSprite->GetHeight() * m_playerSprite->GetSpriteSizeMultiplier())
		);
	}

	// 본체 나중에
	renderer.DrawSprite(*m_playerOutlineSprite,
		drawX, drawY,
		0, 0,
		GetSrcWidth(), GetSrcHeight(),
		GetRenderWidth(), GetRenderHeight()
	);
}

void Player::OnCollision(GameObject& other)
{
	// TODO : 피격 사운드
	if (other.GetType() == GameObjectType::ITEM)
	{
		switch (dynamic_cast<Item*>(&other)->GetItemType())
		{
		case ItemType::SPEEDUP:
		{
			m_fBaseSpeed += 20.f;
			SetSpeed(m_fBaseSpeed);
			std::cout << "Speed-up item collected! Speed increased to " << GetSpeed() << std::endl;
		}
		break;
		case ItemType::POWERUP:
		{
			SetDamage(GetDamage() + 5);
			std::cout << "Power-up item collected! Damage increased to " << GetDamage() << std::endl;
		}
		break;
		case ItemType::BULLETUP:
		{
			if (GetNumberOfBullets() < 4)
			{
				SetNumberOfBullets(GetNumberOfBullets() + 1);
				std::cout << "Bullet-up item collected! Number of bullets increased to " << GetNumberOfBullets() << std::endl;
			}
		}
		break;
		case ItemType::HEALTH:
		{
			SetHealth(GetHealth() + 20);
			//if (GetHealth() > 100) SetHealth(100);
			std::cout << "Health item collected! Health increased to " << GetHealth() << std::endl;
		}
		break;
		}
	}
}

void Player::SetDirection(Keystates key, bool tf)
{
	if		(key == Keystates::LEFT)		m_bLeft = tf;
	else if (key == Keystates::RIGHT)	m_bRight = tf;
	else if (key == Keystates::UP)			m_bUp = tf;
	else if (key == Keystates::DOWN)	m_bDown = tf;
}


