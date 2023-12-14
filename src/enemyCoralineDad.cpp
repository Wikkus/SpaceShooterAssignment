#include "enemyCoralineDad.h"

#include "dataStructuresAndMethods.h"
#include "debugDrawer.h"
#include "gameEngine.h"
#include "playerCharacter.h"
#include "projectileManager.h"
#include "quadTree.h"
#include "steeringBehaviour.h"
#include "timerManager.h"
#include "weaponComponent.h"

EnemyCoralineDad::EnemyCoralineDad(unsigned int objectID, int maxHealth, float movementSpeed) :
	EnemyBase(objectID) {
	_sprite = new Sprite();
	_sprite->Load("res/sprites/CoralineDad.png");

	_position = Vector2<float>(-10000.f, -10000.f);

	_circleCollider.position = _position;
	_circleCollider.radius = 12.f;
	
	_movementSpeed = movementSpeed;

	_maxHealth = maxHealth;
	_currentHealth = _maxHealth;
	_enemyType = EnemyType::CoralineDad;

}

EnemyCoralineDad::~EnemyCoralineDad() {
	_sprite = nullptr;
	delete _sprite;
}

void EnemyCoralineDad::Init() {
	_targetPosition = playerCharacter->GetPosition();
	_direction = Vector2<float>(_targetPosition - _position).normalized();

	_orientation = VectorAsOrientation(_direction);

	_attackTimer = timerManager->CreateTimer(1.f);

	PickWeapon();
}

void EnemyCoralineDad::Update() {
	UpdateTarget();
	UpdateMovement();

	_queriedEnemies = enemyManager->GetEnemyQuadTree()->Query(_circleCollider);
	HandleAttack();
}

void EnemyCoralineDad::Render() {
	_sprite->RenderWithOrientation(_position, _orientation);
	_weaponComponent->Render(_position, _orientation);
}

const Circle EnemyCoralineDad::GetCollider() const {
	return _circleCollider;
}

const EnemyType EnemyCoralineDad::GetEnemyType() const {
	return _enemyType;
}

const float EnemyCoralineDad::GetAttackDamage() const {
	return _weaponComponent->GetAttackDamage();
}

const float EnemyCoralineDad::GetAttackRange() const {
	return _weaponComponent->GetAttackRange();
}

const float EnemyCoralineDad::GetOrientation() const {
	return _orientation;
}

const int EnemyCoralineDad::GetCurrentHealth() const {
	return _currentHealth;
}

const unsigned int EnemyCoralineDad::GetObjectID() const {
	return _objectID;
}

const Sprite* EnemyCoralineDad::GetSprite() const {
	return _sprite;
}

const std::shared_ptr<Timer> EnemyCoralineDad::GetAttackTimer() const {
	return _attackTimer;
}

const Vector2<float> EnemyCoralineDad::GetPosition() const {
	return _position;
}

const std::vector<std::shared_ptr<EnemyBase>> EnemyCoralineDad::GetQueriedEnemies() const {
	return _queriedEnemies;
}

void EnemyCoralineDad::ActivateEnemy(float orienation, Vector2<float> direction, Vector2<float> position) {
	_orientation = orienation;
	_direction = direction;
	_position = position;
	_circleCollider.position = position;
	Init();
}

void EnemyCoralineDad::DeactivateEnemy() {
	_orientation = 0.f;
	_direction = Vector2<float>(0.f, 0.f);
	_position = Vector2<float>(-10000.f, -10000.f);
	_circleCollider.position = _position;
	_attackTimer->DeactivateTimer();
}

void EnemyCoralineDad::HandleAttack() {
	_weaponComponent->Attack(_position, _targetPosition, _orientation);
}

bool EnemyCoralineDad::TakeDamage(unsigned int damageAmount) {
	_currentHealth -= damageAmount;
	if (_currentHealth <= 0) {
		return true;
	}
	return false;
}

void EnemyCoralineDad::UpdateMovement() {
	_direction = Vector2<float>(_targetPosition - _position).normalized();
	_position += separationBehaviour->Steering(this).linearVelocity * deltaTime;
	if (!IsInDistance(_position, playerCharacter->GetPosition(), _weaponComponent->GetAttackRange() * 0.5f)) {
		_position += _direction * _movementSpeed * deltaTime;
		_circleCollider.position = _position;
	}
	_orientation = VectorAsOrientation(_direction);
}

void EnemyCoralineDad::UpdateTarget() {
	_targetPosition = playerCharacter->GetPosition();
}

void EnemyCoralineDad::PickWeapon() {
	std::uniform_int_distribution dist{ 0, 1 };
	int temp = dist(randomEngine);
	if (temp == 0) {
		_weaponComponent = std::make_shared<SwordComponent>();

	} else {
		_weaponComponent = std::make_shared<WizardHatComponent>();
	}
}
