#include "BulletPattern.h"
#include "Bullet.h"
#include <cmath>

void BulletPattern::NWayShot(
	GameWorld* world,
	float x, float y,
	float speed,
	int nWay,
	float angleSpread,
	int damage,
	BulletType bulletType,
	float centerAngle)
{
	float angleStep = (nWay - 1) > 0 ? angleSpread / (nWay - 1) : 0.f;
	float startAngle = centerAngle - angleSpread / 2;
	for (int i = 0; i < nWay; ++i)
	{
		float angle = startAngle + i * angleStep;
		Bullet* bullet = new Bullet(x, y, speed, damage, bulletType);
		bullet->SetDirection(angle);
		world->AddObject(bullet);
	}
}

void BulletPattern::RotateShot(
	GameWorld* world,
	float x, float y,
	float speed,
	int nWay,
	float angleSpread,
	int damage,
	BulletType bulletType,
	float& currentAngle)
{
	float angleStep = angleSpread / nWay;
	for (int i = 0; i < nWay; ++i)
	{
		float angle = currentAngle + i * angleStep;
		Bullet* bullet = new Bullet(x, y, speed, damage, bulletType);
		bullet->SetDirection(angle);
		world->AddObject(bullet);
	}
	currentAngle += angleStep; // 다음 회전 각도 업데이트
	if (currentAngle >= 135.f) currentAngle -= 90.f;
}

void BulletPattern::AimShot(
	GameWorld* world,
	float x, float y,
	float targetX, float targetY,
	float speed,
	int damage,
	BulletType bulletType)
{
	float dx = targetX - x;
	float dy = targetY - y;
	float angle = atan2f(dy, dx) * 180.f / 3.14159f; // 라디안을 각도로 변환
	Bullet* bullet = new Bullet(x, y, speed, damage, bulletType);
	bullet->SetDirection(angle);
	world->AddObject(bullet);
}



