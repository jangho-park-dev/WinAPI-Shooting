#pragma once
#include "GameObject.h"
#include "WaveLoader.h"	
#include <queue>

class Renderer;
class ResourceManager;
class Player;
class Enemy;

class GameWorld
{
private:
	std::vector<GameObject*> m_objects;
	std::queue<GameObject*> m_spawnQueue;
	Player* m_player;

	std::vector<Wave> m_waves;
	int m_nCurrentWave;
	float m_fWaveTimer;

public:
	GameWorld();
	~GameWorld();

	void Initialize();

	void Update(RECT& client, float deltaTime);
	void Render(Renderer& renderer);

	void AddObject(GameObject* object);
	void ObjectsSortByLayer();
	void RemoveInactiveObjects();
	void ProcessSpawnQueue();
	void CheckCollisions();
	void HandleInput(float deltaTime);
	void HandleFire(float deltaTime);
	void UpdateWave(float deltaTime);
	void CheckWaveCleared();

	bool KeyDown(int keyCode);
	bool KeyUp(int keyCode);

	Player* GetPlayer() const { return m_player; }
	int GetCurrentWave() const { return m_nCurrentWave; }
};

