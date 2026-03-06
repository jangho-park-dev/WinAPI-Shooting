#pragma once
#include "framework.h"
#include "Sprite.h"
#include "xaudio2.h"
#pragma comment(lib, "xaudio2.lib")

enum class SpriteID
{
	SPRITE_TITLEBACKGROUND,
	SPRITE_GAMEBACKGROUND,
	SPRITE_GAMEOVERBACKGROUND,
	SPRITE_PLAYER,
	SPRITE_PLAYERTRAIL,
	SPRITE_PLAYEROUTLINE,
	SPRITE_BULLET,
	SPRITE_MONSTER,
	SPRITE_MOTHERSHIP,
	SPRITE_DRAGON,
	SPRITE_GOONS,
	SPRITE_ITEM,
	SPRITE_PLAYERBULLETEFFECT,
	SPRITE_ENEMYBULLETEFFECT,
	SPRITE_BTN_START,
	SPRITE_BTN_EXIT,
	SPRITE_BTN_HOME,
	SPRITE_BTN_RESTART,
};

struct SpriteEntry
{
	SpriteID id;
	Sprite* sprite;
};

enum class SoundID
{
	SOUND_TITLE_BGM,
	SOUND_GAME_BGM,
	SOUND_GAMEOVER_BGM,
	SOUND_PLAYER_SHOOT,
	SOUND_PLAYER_HIT,
	SOUND_ENEMY_HIT,
	SOUND_ENEMY_DEATH,
	SOUND_ITEM_PICKUP,
};

struct SoundEntry
{
	SoundID id{};
	WAVEFORMATEX wfx{};				// WAV 포맷 정보
	std::vector<BYTE> audioData;	// WAV 실제 데이터 버퍼
};

class ResourceManager
{
private:
	std::vector<SpriteEntry>	m_sprites;

	IXAudio2*							m_pXAudio2;
	IXAudio2MasteringVoice*	m_pMasterVoice;
	std::vector<SoundEntry>	m_sounds;

	// 루프 재생 중인 보이스 보관(BGM 등)
	std::map<SoundID, IXAudio2SourceVoice*> m_loopVoices;

	// wav 파일 읽어서 SoundEntry에 채우는 함수
	bool LoadWAV(const wchar_t* path, SoundEntry& entry);
	void LoadSound(SoundID id, const wchar_t* path);

public:
	static ResourceManager& GetInstance()
	{
		static ResourceManager instance;
		return instance;
	}

	ResourceManager();
	~ResourceManager();


	Sprite* GetSprite(SpriteID id) const;
	void RPlaySound(SoundID id, float volume, bool loop = false);
	void RStopSound(SoundID id);
};

