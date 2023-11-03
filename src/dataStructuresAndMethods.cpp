#include "dataStructuresAndMethods.h"
#include "gameEngine.h"

bool IsInDistance(Vector2<float> positionA, Vector2<float> positionB, float distance) {
	return Vector2<float>::distanceBetweenVectors(positionA, positionB) <= distance;
}

bool OutOfBorderX(float positionX) {
	return positionX < 0.f || positionX > windowWidth;
}

bool OutOfBorderY(float positionY) {
	return positionY < 0.f || positionY > windowHeight;
}

Vector2<float> GetCursorPosition() {
	int mousePositionX;
	int mousePositionY;
	SDL_GetMouseState(&mousePositionX, &mousePositionY);
	Vector2<float> mousePosition = Vector2<float>(mousePositionX, mousePositionY);
	return mousePosition;

}

Vector2<float> OrientationAsVector(float orientation) {
	return  Vector2<float>(-sinf(orientation), cosf(orientation)).normalized();
}

float VectorAsOrientation(Vector2<float> direction) {
	direction.normalize();
	return atan2f(direction.x, -direction.y);
}

float WrapMax(float rotation, float maxValue) {
	return fmod(maxValue + fmod(rotation, maxValue), maxValue);
}

float WrapMinMax(float rotation, float minValue, float maxValue) {
	return minValue + WrapMax(rotation - minValue, maxValue - minValue);
}

