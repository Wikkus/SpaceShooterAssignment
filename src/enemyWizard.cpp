#include "enemyWizard.h"

#include "dataStructuresAndMethods.h"
#include "debugDrawer.h"
#include "gameEngine.h"
#include "playerCharacter.h"
#include "projectileManager.h"
#include "quadTree.h"
#include "steeringBehaviour.h"
#include "timerManager.h"

EnemyWizard::EnemyWizard(unsigned int enemyID) : EnemyBase(enemyID) {
	_id = enemyID;

	_sprite = new Sprite();
	_sprite->Load("res/sprites/CoralineDadWizard.png");

	_position = Vector2<float>(-10000.f, -10000.f);

	_circleCollider.position = _position;
	_circleCollider.radius = 12.f;
	
	_maxHealth = 20;
	_currentHealth = _maxHealth;
	_enemyType = EnemyType::EnemyWizard;
}

EnemyWizard::~EnemyWizard() {
	_sprite = nullptr;
	delete _sprite;
}

void EnemyWizard::Init() {
	_targetPosition = playerCharacter->GetPosition();
	_direction = Vector2<float>(_targetPosition - _position).normalized();

	_orientation = VectorAsOrientation(_direction);

	_attackTimer = timerManager->CreateTimer(1.f);
}

void EnemyWizard::Update() {
	UpdateTarget();
	UpdateMovement();

	_queriedEnemies = enemyQuadTree->QueryTemp(_circleCollider);
	if (IsInDistance(_position, playerCharacter->GetPosition(), _attackRange) && _attackTimer->GetTimerFinished()) {
		ExecuteAttack();
	}
}

void EnemyWizard::Render() {
	_sprite->RenderWithOrientation(_position, _orientation);
}

const Circle EnemyWizard::GetCollider() const {
	return _circleCollider;
}

const EnemyType EnemyWizard::GetEnemyType() const {
	return _enemyType;
}

const float EnemyWizard::GetAttackDamage() const {
	return _attackDamage;
}

const float EnemyWizard::GetAttackRange() const {
	return _attackRange;
}

const float EnemyWizard::GetOrientation() const {
	return _orientation;
}

const int EnemyWizard::GetCurrentHealth() const {
	return _currentHealth;
}

const unsigned int EnemyWizard::GetEnemyID() const {
	return _id;
}

const Sprite* EnemyWizard::GetSprite() const {
	return _sprite;
}

const std::shared_ptr<Timer> EnemyWizard::GetAttackTimer() const {
	return _attackTimer;
}

const Vector2<float> EnemyWizard::GetPosition() const {
	return _position;
}

const std::vector<EnemyBase*> EnemyWizard::GetQueriedEnemies() const {
	return _queriedEnemies;
}

void EnemyWizard::ActivateEnemy(float orienation, Vector2<float> direction, Vector2<float> position) {
	_orientation = orienation;
	_direction = direction;
	_position = position;
	_circleCollider.position = position;
}

void EnemyWizard::DeactivateEnemy() {
	_orientation = 0.f;
	_id = -1;
	_direction = Vector2<float>(0.f, 0.f);
	_position = Vector2<float>(-10000.f, -10000.f);
	_circleCollider.position = _position;
}

bool EnemyWizard::TakeDamage(unsigned int damageAmount) {
	_currentHealth -= damageAmount;
	if (_currentHealth <= 0) {
		return true;
	}
	return false;
}

void EnemyWizard::ExecuteAttack() {
	projectileManager->SpawnProjectile(DamageType::DamagePlayer, _orientation, _direction, _position);
	_attackTimer->ResetTimer();
}

void EnemyWizard::UpdateMovement() {
	_direction = Vector2<float>(_targetPosition - _position).normalized();
	_position += separationBehaviour->Steering(this).linearVelocity * deltaTime;
	if (!IsInDistance(_position, playerCharacter->GetPosition(), _attackRange * 0.5f)) {
		_position += _direction * _movementSpeed * deltaTime;
		_circleCollider.position = _position;
	}
	_orientation = VectorAsOrientation(_direction);
}

void EnemyWizard::UpdateTarget() {
	_targetPosition = playerCharacter->GetPosition();
}