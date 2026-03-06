#include "ResourceManager.h"

ResourceManager::ResourceManager()
	: m_pXAudio2(nullptr), m_pMasterVoice(nullptr)
{
	// XAudio2 초기화
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(XAudio2Create(&m_pXAudio2)))
		std::cerr << "XAudio2Create failed" << std::endl;
	else if (FAILED(m_pXAudio2->CreateMasteringVoice(&m_pMasterVoice)))
		std::cerr << "CreateMasteringVoice failed" << std::endl;


	// Background sprite
	m_sprites.push_back(SpriteEntry{
		SpriteID::SPRITE_GAMEBACKGROUND,
		new Sprite(L"GameAssets/GameScene.bmp")
		}
	);

	m_sprites.push_back(SpriteEntry{
		SpriteID::SPRITE_TITLEBACKGROUND,
		new Sprite(L"GameAssets/TitleScene.bmp")
		}
	);

	m_sprites.push_back(SpriteEntry{
		SpriteID::SPRITE_GAMEOVERBACKGROUND,
		new Sprite(L"GameAssets/GameOverScene.bmp")
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

	// Item sprite
	m_sprites.push_back(SpriteEntry{
		SpriteID::SPRITE_ITEM,
		new Sprite(L"GameAssets/VULCAN-Powerup_Sheet.bmp", true, RGB(255, 0, 255))
		}
	);

	// playerBulletCollisionEffect sprite
	m_sprites.push_back(SpriteEntry{
		SpriteID::SPRITE_PLAYERBULLETEFFECT,
		new Sprite(L"GameAssets/playerBulletCollisionEffect.bmp", true, RGB(255, 0, 255))
		}
	);

	// enemyBulletCollisionEffect sprite
	m_sprites.push_back(SpriteEntry{
		SpriteID::SPRITE_ENEMYBULLETEFFECT,
		new Sprite(L"GameAssets/enemyBulletCollisionEffect.bmp", true, RGB(255, 0, 255))
		}
	);

	// 버튼 스프라이트
	m_sprites.push_back({
		SpriteID::SPRITE_BTN_START,
		new Sprite(L"GameAssets/StartButton.bmp", true, RGB(255, 0, 255))
		}
	);

	m_sprites.push_back({
		SpriteID::SPRITE_BTN_EXIT,
		new Sprite(L"GameAssets/ExitButton.bmp", true, RGB(255, 0, 255))
		}
	);

	m_sprites.push_back({
		SpriteID::SPRITE_BTN_HOME,
		new Sprite(L"GameAssets/HomeButton.bmp", true, RGB(255, 0, 255))
		}
	);

	m_sprites.push_back({
		SpriteID::SPRITE_BTN_RESTART,
		new Sprite(L"GameAssets/RestartButton.bmp", true, RGB(255, 0, 255))
		}
	);



	// Bgm sound
	LoadSound(SoundID::SOUND_GAME_BGM, L"GameAssets/gameBgm.wav");
	LoadSound(SoundID::SOUND_TITLE_BGM, L"GameAssets/titleBgm.wav");
	LoadSound(SoundID::SOUND_GAMEOVER_BGM, L"GameAssets/gameOverBgm.wav");

	// Player shooting sound
	LoadSound(SoundID::SOUND_PLAYER_SHOOT, L"GameAssets/player_shoot.wav");

	// Player hit sound
	LoadSound(SoundID::SOUND_PLAYER_HIT, L"GameAssets/player_hit.wav");

	// Enemy hit sound
	LoadSound(SoundID::SOUND_ENEMY_HIT, L"GameAssets/enemy_hit.wav");

	// Enemy death sound
	LoadSound(SoundID::SOUND_ENEMY_DEATH, L"GameAssets/enemy_death.wav");

	// Item pickup sound
	LoadSound(SoundID::SOUND_ITEM_PICKUP, L"GameAssets/item_pickup.wav");
}

ResourceManager::~ResourceManager()
{
	for (auto& e : m_sprites)
		delete e.sprite;

	// 루프 중인 보이스 전부 정지 및 해제
	for (auto& pair : m_loopVoices)
	{
		if (pair.second)
		{
			pair.second->Stop();
			pair.second->DestroyVoice();
		}
	}
	m_loopVoices.clear();

	if (m_pMasterVoice)
	{
		m_pMasterVoice->DestroyVoice();
		m_pMasterVoice = nullptr;
	}
	if (m_pXAudio2)
	{
		m_pXAudio2->Release();
		m_pXAudio2 = nullptr;
	}
	CoUninitialize();
}

bool ResourceManager::LoadWAV(const wchar_t* path, SoundEntry& entry)
{
	std::ifstream file(path, std::ios::binary);
	if (!file.is_open())
	{
		std::cerr << "Failed to open WAV: " << path << std::endl;
		return false;
	}

	// RIFF 헤더 확인
	char riff[4];
	file.read(riff, 4);
	if (strncmp(riff, "RIFF", 4) != 0) return false;

	DWORD fileSize;
	file.read(reinterpret_cast<char*>(&fileSize), 4);

	char wave[4];
	file.read(wave, 4);
	if (strncmp(wave, "WAVE", 4) != 0) return false;

	// fmt / data 청크 탐색
	while (!file.eof())
	{
		char chunkID[4];
		DWORD chunkSize;
		file.read(chunkID, 4);
		file.read(reinterpret_cast<char*>(&chunkSize), 4);

		if (strncmp(chunkID, "fmt ", 4) == 0)
		{
			// WAVEFORMATEX 채우기 (최대 18바이트)
			DWORD readSize = min(chunkSize, static_cast<DWORD>(sizeof(WAVEFORMATEX)));
			file.read(reinterpret_cast<char*>(&entry.wfx), readSize);
			if (chunkSize > readSize)
				file.seekg(chunkSize - readSize, std::ios::cur);
		}
		else if (strncmp(chunkID, "data", 4) == 0)
		{
			entry.audioData.resize(chunkSize);
			file.read(reinterpret_cast<char*>(entry.audioData.data()), chunkSize);
			break;
		}
		else
		{
			// 알 수 없는 청크는 건너뜀
			file.seekg(chunkSize, std::ios::cur);
		}
	}

	return !entry.audioData.empty();
}

void ResourceManager::LoadSound(SoundID id, const wchar_t* path)
{
	SoundEntry entry;
	entry.id = id;
	if (LoadWAV(path, entry))
		m_sounds.push_back(std::move(entry));
}

Sprite* ResourceManager::GetSprite(SpriteID id) const
{
	for (const auto& e : m_sprites)
		if (e.id == id)
			return e.sprite;

	return nullptr;
}

// loop: true면 BGM처럼 반복 재생
void ResourceManager::RPlaySound(SoundID id, float volume, bool loop)
{
	if (!m_pXAudio2) return;

	// SoundEntry 찾기
	SoundEntry* entry = nullptr;
	for (auto& e : m_sounds)
		if (e.id == id) { entry = &e; break; }
	if (!entry) return;

	// SourceVoice는 매 재생마다 새로 생성 (동시 재생 지원)
	IXAudio2SourceVoice* pVoice = nullptr;
	if (FAILED(m_pXAudio2->CreateSourceVoice(&pVoice, &entry->wfx)))
		return;

	XAUDIO2_BUFFER buf = {};
	buf.AudioBytes = static_cast<UINT32>(entry->audioData.size());
	buf.pAudioData = entry->audioData.data();
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;

	pVoice->SubmitSourceBuffer(&buf);
	pVoice->SetVolume(volume);
	pVoice->Start();

	// 루프 사운드(BGM)는 따로 보관해서 나중에 Stop 가능하게
	if (loop)
		m_loopVoices[id] = pVoice;
	// 효과음은 재생 후 자동 소멸
	// 짧은 효과음은 그냥 leak 허용하거나, 아래 StopSound로 정리
}

// 루프 사운드 정지
void ResourceManager::RStopSound(SoundID id)
{
	auto it = m_loopVoices.find(id);
	if (it == m_loopVoices.end()) return;
	it->second->Stop();
	it->second->DestroyVoice();
	m_loopVoices.erase(it);
}
