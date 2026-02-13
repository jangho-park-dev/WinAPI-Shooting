#pragma once

class GameObject;

class Collider
{
private:
	GameObject* m_owner;

public:
	Collider(GameObject* onwer) : m_owner(onwer) {}

	virtual ~Collider() {};

	virtual bool CheckCollision(const Collider& other) = 0;
	GameObject* GetOwner() const { return m_owner; }
};

