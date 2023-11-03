#include "steeringBehaviour.h"

#include "enemyBase.h"
#include "gameEngine.h"
#include "imGuiManager.h"

SteeringOutput SeparationBehaviour::Steering(EnemyBase* enemyData) {
	SteeringOutput result;
	_targetPosition = Vector2<float>(0, 0);
	_direction = Vector2<float>(0, 0);
	_distance = 0;
	_strength = 0.f;

	for (unsigned int i = 0; i < enemyData->GetQueriedEnemies().size(); i++) {
		if (enemyData->GetEnemyID() == enemyData->GetQueriedEnemies()[i]->GetEnemyID()) {
			continue;
		}
		_targetPosition = enemyData->GetQueriedEnemies()[i]->GetPosition();
		_direction = _targetPosition - enemyData->GetPosition();
		_distance = _direction.absolute();
		if (_distance < _separationThreshold) {
			_strength = std::min(_decayCoefficient / (_distance * _distance), _maxAcceleration);
			_direction.normalize();
			result.linearVelocity -= _direction * _strength;
		}
	}
	return result;
}

void SeparationBehaviour::UpdateImgui() {
	imGuiHandler->InputFloat("SeparationBehavior", "Decay", _decayCoefficient);
	imGuiHandler->InputFloat("SeparationBehavior", "Acceleration", _maxAcceleration);
	imGuiHandler->InputFloat("SeparationBehavior", "Threshold", _separationThreshold);
}
