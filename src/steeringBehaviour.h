#pragma once
#include "vector2.h"

class EnemyBase;

struct SteeringOutput {
	Vector2<float> linearVelocity = Vector2<float>(0, 0);
	float angularVelocity = 0.f;
};

class SteeringBehaviour {
public:
	SteeringBehaviour() {}
	~SteeringBehaviour() {}
	virtual SteeringOutput Steering(EnemyBase* enemyData) = 0;

	virtual void UpdateImgui() = 0;
};

class SeparationBehaviour : public SteeringBehaviour {
public:
	SeparationBehaviour() {}
	~SeparationBehaviour() {}

	SteeringOutput Steering(EnemyBase* enemyData) override;
	void UpdateImgui() override;

private:
	Vector2<float> _targetPosition = Vector2<float>(0, 0);
	Vector2<float> _direction = Vector2<float>(0, 0);
	float _decayCoefficient = 10000.f;
	float _distance = 0;
	float _maxAcceleration = 50.f;
	float _separationThreshold = 64.f;
	float _strength = 0.f;
};