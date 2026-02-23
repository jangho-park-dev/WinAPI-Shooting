#pragma once
#include "GameObject.h"
#include "Player.h"
#include "Enemy.h"
#include <queue>

class Renderer;
class ResourceManager;

class GameWorld
{
private:
	std::vector<GameObject*> m_objects;
	std::queue<GameObject*> m_spawnQueue;
	Player* m_player;

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

	bool KeyDown(int keyCode);
	bool KeyUp(int keyCode);
};

