#include "debugDrawer.h"
#include "gameEngine.h"

#include <SDL2/SDL.h>

void DebugDrawer::AddDebugBox(Vector2<float> position, Vector2<float> min, Vector2<float> max) {
	DebugBox debugBox;
	debugBox.min = min;
	debugBox.max = max;
	debugBox.position = position;
	_debugBox.emplace_back(debugBox);
}

void DebugDrawer::AddDebugCircle(Vector2<float> position, float radius) {
	DebugCircle debugCircle;
	debugCircle.position = position;
	debugCircle.radius = radius;
	_debugCircles.emplace_back(debugCircle);
}

void DebugDrawer::AddDebugCross(Vector2<float> position, float length){
	DebugLine debugLineHorizontal;
	DebugLine debugLineVertical;

	debugLineHorizontal.startPosition = Vector2<float>(position.x - (length * 0.5f), position.y);
	debugLineHorizontal.endPosition = Vector2<float>(position.x + (length * 0.5f), position.y);

	debugLineVertical.startPosition = Vector2<float>(position.x, position.y - (length * 0.5f));
	debugLineVertical.endPosition = Vector2<float>(position.x, position.y + (length * 0.5f));

	_debugLines.emplace_back(debugLineHorizontal);
	_debugLines.emplace_back(debugLineVertical);
}

void DebugDrawer::AddDebugLine(Vector2<float> startPosition, Vector2<float> endPosition) {
	DebugLine debugLine;
	debugLine.startPosition = startPosition;
	debugLine.endPosition = endPosition;
	
	_debugLines.emplace_back(debugLine);
}

void DebugDrawer::DrawBoxes() {
	SDL_SetRenderDrawColor(renderer, 125, 0, 125, 255);
	for (int i = 0; i < _debugBox.size(); i++) {
		SDL_Rect rect = {
			_debugBox[i].position.x - ((_debugBox[i].max.x - _debugBox[i].min.x) * 0.5f),
			_debugBox[i].position.y - ((_debugBox[i].max.y - _debugBox[i].min.y) * 0.5f),
			_debugBox[i].max.x - _debugBox[i].min.x,
			_debugBox[i].max.y - _debugBox[i].min.y			
		};
		SDL_RenderDrawRect(renderer, &rect);
	}
	_debugBox.clear();
}

void DebugDrawer::DrawCircles() {
	SDL_SetRenderDrawColor(renderer, 125, 0, 125, 255);
	for (int i = 0; i < _debugCircles.size(); i++) {

		int resolution = 24;
		float step = (2 * PI) / resolution;

		for (int k = 0; k < resolution; k++) {

			float angle = step * k;
			float x1 = cos(angle);
			float y1 = sin(angle);

			float nextAngle = step * (k + 1);
			float x2 = cos(nextAngle);
			float y2 = sin(nextAngle);

			SDL_RenderDrawLine(
				renderer,
				x1 * _debugCircles[i].radius + _debugCircles[i].position.x,
				y1 * _debugCircles[i].radius + _debugCircles[i].position.y,
				x2 * _debugCircles[i].radius + _debugCircles[i].position.x,
				y2 * _debugCircles[i].radius + _debugCircles[i].position.y);

		}
	}
	_debugCircles.clear();
}

void DebugDrawer::DrawLines() {
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	for (int i = 0; i < _debugLines.size(); i++) {
		SDL_RenderDrawLineF(renderer, _debugLines[i].startPosition.x, _debugLines[i].startPosition.y, 
			_debugLines[i].endPosition.x, _debugLines[i].endPosition.y);
	}
	_debugLines.clear();

}
