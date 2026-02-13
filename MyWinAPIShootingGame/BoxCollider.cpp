#include "BoxCollider.h"
#include "GameObject.h"

bool BoxCollider::CheckCollision(const Collider& other)
{
	RECT alpha = GetOwner()->GetAABB();
	RECT beta = other.GetOwner()->GetAABB();

	// 충돌 발생
	return 
		alpha.left <= beta.right &&
		alpha.right >= beta.left &&
		alpha.top <= beta.bottom &&
		alpha.bottom >= beta.top;
}

