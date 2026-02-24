#pragma once
#include "GameWorld.h"

enum class BulletType;

class BulletPattern
{
public:
	static void NWayShot(
		GameWorld* world,
		float x, float y,
		float speed,
		int nWay,
		float angleSpread,
		int damage,
		BulletType bulletType,
		float centerAngle = 90.f);
	
	static void RotateShot(
		GameWorld* world,
		float x, float y,
		float speed,
		int nWay,
		float angleSpread,
		int damage,
		BulletType bulletType,
		float& currentAngle);

	static void AimShot(
		GameWorld* world,
		float x, float y,
		float targetX, float targetY,
		float speed,
		int damage,
		BulletType bulletType);
};

