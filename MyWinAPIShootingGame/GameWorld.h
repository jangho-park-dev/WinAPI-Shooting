#pragma once
#include "GameObject.h"
#include "WaveLoader.h"	
#include "SceneManager.h"
#include "GameStats.h"
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

	SceneManager* m_sceneManager;

	// Ελ°θ
	int m_nEnemyKilled;
	float m_fSurvivalTime;

	bool m_bGameOver;

	float m_fFps;

public:
	GameWorld(SceneManager* sceneManager = nullptr);
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
	void CheckRenderFPS(float deltaTime);

	bool KeyDown(int keyCode);
	bool KeyUp(int keyCode);

	Player* GetPlayer() const { return m_player; }
	int GetCurrentWave() const { return m_nCurrentWave; }
	bool IsAllWaveCleared() const { return m_nCurrentWave >= static_cast<int>(m_waves.size()); }

	void AddEnemyKill() { ++m_nEnemyKilled; }

	GameStats GetStats() const { return { m_nCurrentWave, m_nEnemyKilled, m_fSurvivalTime }; }

	void SetGameOver() { m_bGameOver = true; }
	bool IsGameOver() const { return m_bGameOver; }

};

