#pragma once
#include "Renderer.h"

class Scene
{
public:
	virtual ~Scene() {}

	virtual void Initialize() {}
	virtual void Update(RECT& client, float deltaTime) = 0;
	virtual void Render(Renderer& renderer) = 0;
	virtual void HandleInput(float deltaTime) = 0;
};