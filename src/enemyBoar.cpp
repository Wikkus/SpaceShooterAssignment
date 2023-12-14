#include "enemyBoar.h"

#include "dataStructuresAndMethods.h"
#include "debugDrawer.h"
#include "gameEngine.h"
#include "playerCharacter.h"
#include "quadTree.h"
#include "steeringBehaviour.h"
#include "timerManager.h"

EnemyBoar::EnemyBoar(unsigned int objectID, int attackDamage, int maxHealth, float attackRange, float movementSpeed) : 
	EnemyBase(objectID) {
	_sprite = new Sprite();
	_sprite->Load("res/sprites/MadBoar.png");

	_position = Vector2<float>(-10000.f, -10000.f);

	_circleCollider.radius = 16.f;
	_circleCollider.position = _position;

	_maxHealth = maxHealth;
	_currentHealth = _maxHealth;

	_attackDamage = attackDamage;
	_attackRange = attackRange;
	_movementSpeed = movementSpeed;

	_enemyType = EnemyType::Boar;
}

EnemyBoar::~EnemyBoar() {
	_sprite = nullptr;
	delete _sprite;
}

void EnemyBoar::Init() {
	_targetPosition = playerCharacter->GetPosition();
	_direction = _targetPosition - _position;

	_attackTimer =  timerManager->CreateTimer(1.f);
}

void EnemyBoar::Update() {
	UpdateTarget();
	_queriedEnemies = enemyManager->GetEnemyQuadTree()->Query(_circleCollider);
	UpdateMovement();
	HandleAttack();
}

void EnemyBoar::Render() {
	_sprite->RenderWithOrientation(_position, _orientation);
}

const Circle EnemyBoar::GetCollider() const {
	return _circleCollider;
}

const EnemyType EnemyBoar::GetEnemyType() const {
	return _enemyType;
}

const float EnemyBoar::GetAttackDamage() const {
	return _attackDamage;
}

const float EnemyBoar::GetAttackRange() const {
	return _attackRange;
}

const std::shared_ptr<Timer> EnemyBoar::GetAttackTimer() const {	
	return _attackTimer;
}

const float EnemyBoar::GetOrientation() const {
	return _orientation;
}

const int EnemyBoar::GetCurrentHealth() const {
	return _currentHealth;
}

const unsigned int EnemyBoar::GetObjectID() const {
	return _objectID;
}

const Sprite* EnemyBoar::GetSprite() const {
	return _sprite;
}

const Vector2<float> EnemyBoar::GetPosition() const {
	return _position;
}

const std::vector<std::shared_ptr<EnemyBase>> EnemyBoar::GetQueriedEnemies() const {
	return _queriedEnemies;
}

void EnemyBoar::ActivateEnemy(float orienation, Vector2<float> direction, Vector2<float> position) {
	_orientation = orienation;
	_direction = direction;
	_position = position;
	_circleCollider.position = _position;
	Init();
}

void EnemyBoar::DeactivateEnemy() {
	_orientation = 0.f;
	_direction = Vector2<float>(0.f, 0.f);
	_position = Vector2<float>(-10000.f, -10000.f);
	_circleCollider.position = _position;
	_attackTimer->DeactivateTimer();
}

bool EnemyBoar::TakeDamage(unsigned int damageAmount) {
	_currentHealth -= damageAmount;
	if (_currentHealth <= 0) {
		return true;
	}
	return false;
}

void EnemyBoar::HandleAttack() {
	if (IsInDistance(_position, playerCharacter->GetPosition(), _attackRange) && _attackTimer->GetTimerFinished()) {
		playerCharacter->TakeDamage(_attackDamage);
		_attackTimer->ResetTimer();
	}

}

void EnemyBoar::UpdateMovement() {
	_direction = Vector2<float>(_targetPosition - _position).normalized();
	
	_position += separationBehaviour->Steering(this).linearVelocity * deltaTime;

	if (!IsInDistance(_position, playerCharacter->GetPosition(), _attackRange * 0.5f)) {
		_position += _direction * _movementSpeed * deltaTime;
		_circleCollider.position = _position;
	}
	_orientation = VectorAsOrientation(_direction);
}

void EnemyBoar::UpdateTarget() {
	_targetPosition = playerCharacter->GetPosition();
}

