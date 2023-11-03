#pragma once
#include "vector2.h"

struct Circle {
	float radius;
	Vector2<float> position;
};

struct Collision {
	Vector2<float> position = Vector2<float>(0, 0);
	Vector2<float> normal = Vector2<float>(0,0);

	bool collisionDetected = false;
};

struct AABB {
	static AABB makeFromPositionSize(Vector2<float> position, float h, float w);

	void SetPosition(Vector2<float> newPosition);

	float height = 0.f;
	float width = 0.f;

	Vector2<float> position = Vector2<float>(0.f, 0.f);
	Vector2<float> min = Vector2<float>(0.f, 0.f);
	Vector2<float> max = Vector2<float>(0.f, 0.f);
};

bool CircleIntersect(Circle& circleA, Circle& circleB);

bool AABBIntersect(AABB& boxA, AABB& boxB);

bool AABBCircleIntersect(AABB& box, Circle& circle);