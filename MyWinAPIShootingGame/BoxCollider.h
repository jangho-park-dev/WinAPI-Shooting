#pragma once
#include "Collider.h"

class BoxCollider : public Collider
{
public:
	BoxCollider(GameObject* owner) : Collider(owner) {}

	bool CheckCollision(const Collider& other) override;

};

