#pragma once
#include "GameObject.h"
#include "Player.h"

class Renderer;
class ResourceManager;

class GameWorld
{
private:
	std::vector<GameObject*> m_objects;
	Player* m_player;

public:
	GameWorld();
	~GameWorld();

	void Initialize();

	void Update(RECT& client, float deltaTime);
	void Render(Renderer& renderer);

	void AddObject(GameObject* object);
	void RemoveInactiveObjects();
	void CheckCollisions();
	void HandleInput(float deltaTime);
	void HandleFire(float deltaTime);
};

