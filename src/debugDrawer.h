#pragma once
#include "vector2.h"

#include <vector>

struct DebugBox {
	Vector2<float> min = Vector2<float>(0.f, 0.f);
	Vector2<float> max = Vector2<float>(0.f, 0.f);
	Vector2<float> position = Vector2<float>(0.f, 0.f);
};
struct DebugCircle {
	Vector2<float> position = Vector2<float>(0.f, 0.f);
	float radius = 0.f;
};
struct DebugLine {
	Vector2<float> startPosition = Vector2<float>(0.f, 0.f);
	Vector2<float> endPosition = Vector2<float>(0, 0.f);
};


class DebugDrawer {
public:
	DebugDrawer() {}
	~DebugDrawer() {}

	void AddDebugBox(Vector2<float> position, Vector2<float> min, Vector2<float> max);
	void AddDebugCircle(Vector2<float> position, float radius);
	void AddDebugCross(Vector2<float> position, float length);
	void AddDebugLine(Vector2<float> startPosition, Vector2<float> endPosition);

	void DrawBoxes();
	void DrawCircles();
	void DrawLines();

private:

	std::vector<DebugBox> _debugBox;
	std::vector<DebugLine> _debugLines;
	std::vector<DebugCircle> _debugCircles;

};

