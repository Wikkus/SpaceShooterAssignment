#include "enemyFighter.h"

#include "dataStructuresAndMethods.h"
#include "debugDrawer.h"
#include "gameEngine.h"
#include "playerCharacter.h"
#include "quadTree.h"
#include "steeringBehaviour.h"
#include "timerManager.h"

EnemyFighter::EnemyFighter(unsigned int objectID, int attackDamage, float attackRange, float movementSpeed) : 
	EnemyBase(objectID, attackDamage, attackRange, movementSpeed) {
	_sprite = new Sprite();
	_sprite->Load("res/sprites/CoralineDadFighter.png");

	_position = Vector2<float>(-10000.f, -10000.f);

	_circleCollider.radius = 16.f;
	_circleCollider.position = _position;

	_maxHealth = 30;
	_currentHealth = _maxHealth;

	_attackDamage = attackDamage;
	_attackRange = attackRange;
	_movementSpeed = movementSpeed;

	_enemyType = EnemyType::EnemyFighter;
}

EnemyFighter::~EnemyFighter() {
	_sprite = nullptr;
	delete _sprite;
}

void EnemyFighter::Init() {
	_targetPosition = playerCharacter->GetPosition();
	_direction = _targetPosition - _position;

	_attackTimer =  timerManager->CreateTimer(1.f);
}

void EnemyFighter::Update() {
	UpdateTarget();
	_queriedEnemies = enemyManager->GetEnemyQuadTree()->Query(_circleCollider);
	UpdateMovement();
}

void EnemyFighter::Render() {
	_sprite->RenderWithOrientation(_position, _orientation);
}

const Circle EnemyFighter::GetCollider() const {
	return _circleCollider;
}

const EnemyType EnemyFighter::GetEnemyType() const {
	return _enemyType;
}

const std::shared_ptr<Timer> EnemyFighter::GetAttackTimer() const {	return _attackTimer;
}

const float EnemyFighter::GetAttackDamage() const {
	return _attackDamage;
}

const float EnemyFighter::GetAttackRange() const {
	return _attackRange;
}

const float EnemyFighter::GetOrientation() const {
	return _orientation;
}

const int EnemyFighter::GetCurrentHealth() const {
	return _currentHealth;
}

const unsigned int EnemyFighter::GetObjectID() const {
	return _objectID;
}

const Sprite* EnemyFighter::GetSprite() const {
	return _sprite;
}

const Vector2<float> EnemyFighter::GetPosition() const {
	return _position;
}

const std::vector<std::shared_ptr<EnemyBase>> EnemyFighter::GetQueriedEnemies() const {
	return _queriedEnemies;
}

void EnemyFighter::ActivateEnemy(float orienation, Vector2<float> direction, Vector2<float> position) {
	_orientation = orienation;
	_direction = direction;
	_position = position;
	_circleCollider.position = _position;
	Init();
}

void EnemyFighter::DeactivateEnemy() {
	_orientation = 0.f;
	_direction = Vector2<float>(0.f, 0.f);
	_position = Vector2<float>(-10000.f, -10000.f);
	_circleCollider.position = _position;
	_attackTimer->DeactivateTimer();
}

bool EnemyFighter::TakeDamage(unsigned int damageAmount) {
	_currentHealth -= damageAmount;
	if (_currentHealth <= 0) {
		return true;
	}
	return false;
}

void EnemyFighter::ExecuteAttack() {
	playerCharacter->TakeDamage(_attackDamage);
	_attackTimer->ResetTimer();
}

void EnemyFighter::UpdateMovement() {
	_direction = Vector2<float>(_targetPosition - _position).normalized();
	
	_position += separationBehaviour->Steering(this).linearVelocity * deltaTime;

	if (!IsInDistance(_position, playerCharacter->GetPosition(), _attackRange * 0.5f)) {
		_position += _direction * _movementSpeed * deltaTime;
		_circleCollider.position = _position;
	}
	_orientation = VectorAsOrientation(_direction);
}

void EnemyFighter::UpdateTarget() {
	_targetPosition = playerCharacter->GetPosition();
}

void EnemyFighter::Separation() {
	
}
