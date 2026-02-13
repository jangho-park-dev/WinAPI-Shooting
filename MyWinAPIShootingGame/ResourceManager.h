#pragma once
#include "framework.h"
#include "Sprite.h"

enum SpriteID
{
	SPRITE_BACKGROUND,
	SPRITE_PLAYER,
	SPRITE_PLAYERTRAIL,
	SPRITE_PLAYEROUTLINE,
	SPRITE_BULLET,
	SPRITE_MONSTER,
	SPRITE_MOTHERSHIP,
	SPRITE_DRAGON,
	SPRITE_GOONS,
	SRPITE_POWERUP,
};

struct SpriteEntry
{
	SpriteID id;
	Sprite* sprite;
};

class ResourceManager
{
private:
	std::vector<SpriteEntry> m_sprites;
	// TODO : 나중에 사운드 리스트도 추가해야 될 듯
	
public:
	ResourceManager()
	{
		// Background sprite
		m_sprites.push_back(SpriteEntry{
			SpriteID::SPRITE_BACKGROUND,
			new Sprite(L"GameAssets/Background.bmp") 
			}
		);

		// Player sprite
		m_sprites.push_back(SpriteEntry{
			SpriteID::SPRITE_PLAYER,
			new Sprite(L"GameAssets/VULCAN-Ship.bmp", true, RGB(255, 0, 255))
			}
		);

		m_sprites.push_back(SpriteEntry{
			SpriteID::SPRITE_PLAYERTRAIL,
			new Sprite(L"GameAssets/VULCAN-Ship_BlinkTrail.bmp", true, RGB(255, 0, 255))
			}
		);

		m_sprites.push_back(SpriteEntry{
			SpriteID::SPRITE_PLAYEROUTLINE,
			new Sprite(L"GameAssets/VULCAN-Ship_Outline.bmp", true, RGB(255, 0, 255))
			}
		);

		// Player bullet sprite
		m_sprites.push_back(SpriteEntry{
			SpriteID::SPRITE_BULLET,
			new Sprite(L"GameAssets/VULCAN-Bullet_Sheet.bmp", true, RGB(255, 0, 255))
			}
		);

		// Monster sprite
		m_sprites.push_back(SpriteEntry{
			SpriteID::SPRITE_MONSTER,
			new Sprite(L"GameAssets/VULCAN-Monster.bmp", true, RGB(255, 0, 255))
			}
		);

		// Mothership sprite
		m_sprites.push_back(SpriteEntry{
			SpriteID::SPRITE_MOTHERSHIP,
			new Sprite(L"GameAssets/VULCAN-Mothership.bmp", true, RGB(255, 0, 255))
			}
		);

		// Dragon sprite
		m_sprites.push_back(SpriteEntry{
			SpriteID::SPRITE_DRAGON,
			new Sprite(L"GameAssets/VULCAN-Dragon.bmp", true, RGB(255, 0, 255))
			}
		);

		// Goons sprite
		m_sprites.push_back(SpriteEntry{
			SpriteID::SPRITE_GOONS,
			new Sprite(L"GameAssets/VULCAN-Goons.bmp", true, RGB(255, 0, 255))
			}
		);

		// PowerUp sprite
		m_sprites.push_back(SpriteEntry{
			SpriteID::SRPITE_POWERUP,
			new Sprite(L"GameAssets/VULCAN-Powerup_Sheet.bmp", true, RGB(255, 0, 255))
			}
		);
	}

	~ResourceManager()
	{
		for (auto& e : m_sprites)
			delete e.sprite;
	}

	static ResourceManager& GetInstance()
	{
		static ResourceManager instance;
		return instance;
	}

	Sprite* GetSprite(SpriteID id) const
	{
		for (const auto& e : m_sprites)
			if (e.id == id)
				return e.sprite;

		return nullptr;
	}
};

