#pragma once
#include "collision.h"
#include "enemyManager.h"
#include "objectBase.h"
#include "sprite.h"
#include "vector2.h"

class Timer;

enum class EnemyType {
	Boar,
	CoralineDad,
	Count
};

class EnemyBase : public ObjectBase {
public:
	EnemyBase(int objectID) : ObjectBase(objectID) {}
	~EnemyBase() {}

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

	virtual bool TakeDamage(unsigned int damageAmount) = 0;


	virtual const Circle GetCollider() const = 0;
	virtual const EnemyType GetEnemyType() const = 0;
	virtual const float GetAttackDamage() const = 0;
	virtual const float GetAttackRange() const = 0;
	virtual const float GetOrientation() const = 0;
	virtual const int GetCurrentHealth() const = 0;
	virtual const unsigned int GetObjectID() const = 0;
	virtual const Sprite* GetSprite() const = 0;
	virtual const Vector2<float> GetPosition() const = 0;
	virtual const std::shared_ptr<Timer> GetAttackTimer() const = 0;
	virtual const std::vector<std::shared_ptr<EnemyBase>> GetQueriedEnemies() const = 0;
	
	virtual void ActivateEnemy(float orienation, Vector2<float> direction, Vector2<float> position) = 0;
	virtual void DeactivateEnemy() = 0;
	virtual void HandleAttack() = 0;

protected:
	Circle _circleCollider;	

	int _currentHealth = 0;
	int _maxHealth = 0;

	std::shared_ptr<Timer> _attackTimer = nullptr;

	Vector2<float> _targetPosition = Vector2<float>(0.f, 0.f);
	Vector2<float> _direction = Vector2<float>(0.f, 0.f);

	std::vector<std::shared_ptr<EnemyBase>> _queriedEnemies;

	EnemyType _enemyType = EnemyType::Count;

	float _movementSpeed = 0.f;

};

