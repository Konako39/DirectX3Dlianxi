#include "collision.h"

using namespace DirectX;

bool Collision_isOverlap(const CollisionCircle& a, const CollisionCircle& b)
{
	XMVECTOR pos_a = XMLoadFloat2(&a.position);
	XMVECTOR pos_b = XMLoadFloat2(&b.position);
	

	//a.position - b.position 计算两个圆心的距离平方	
	float lengthSq = XMVectorGetX(XMVector2LengthSq((pos_a-pos_b)));

    return (a.radius+b.radius)* (a.radius + b.radius)> lengthSq;
	
	//如果两个圆心的距离平方小于两个半径和的平方，就说明两个圆相交，返回true
}
