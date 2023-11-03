#pragma once
#include "collision.h"
#include "enemyManager.h"
#include "sprite.h"
#include "vector2.h"

class Timer;

class EnemyBase {
public:
	EnemyBase(unsigned int enemyId) {}
	~EnemyBase() {}


	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void ExecuteAttack() = 0;

	virtual bool TakeDamage(unsigned int damageAmount) = 0;

	virtual const Circle GetCollider() const = 0;
	virtual const EnemyType GetEnemyType() const = 0;
	virtual const float GetAttackDamage() const = 0;
	virtual const float GetAttackRange() const = 0;
	virtual const float GetOrientation() const = 0;
	virtual const int GetCurrentHealth() const = 0;
	virtual const unsigned int GetEnemyID() const = 0;
	virtual const Sprite* GetSprite() const = 0;
	virtual const std::shared_ptr<Timer> GetAttackTimer() const = 0;
	virtual const Vector2<float> GetPosition() const = 0;
	virtual const std::vector<EnemyBase*> GetQueriedEnemies() const = 0;
	
	virtual void ActivateEnemy(float orienation, Vector2<float> direction, Vector2<float> position) = 0;
	virtual void DeactivateEnemy() = 0;

protected:
	Circle _circleCollider;
	
	EnemyType _enemyType = EnemyType::Count;

	float _orientation = 0.f;
	
	int _currentHealth = 0.f;
	int _maxHealth = 0;

	unsigned int _id = -1;

	Sprite* _sprite = nullptr;

	std::shared_ptr<Timer> _attackTimer = nullptr;

	Vector2<float> _position = Vector2<float>(-10000.f, -10000.f);
	Vector2<float> _targetPosition = Vector2<float>(0.f, 0.f);
	Vector2<float> _direction = Vector2<float>(0.f, 0.f);

	std::vector<EnemyBase*> _queriedEnemies;
};

