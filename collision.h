#pragma once
#ifndef COLLISION_H
#define COLLISION_H
#include <DirectXMath.h>

struct CollisionCircle {
	DirectX::XMFLOAT2 position;
	float radius;
};

bool Collision_isOverlap(const CollisionCircle& a,const CollisionCircle& b);

#endif // !COLLISION_H

