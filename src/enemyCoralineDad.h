#pragma once
#include "collision.h"
#include "enemyBase.h"
#include "sprite.h"
#include "vector2.h"

class WeaponComponent;

class EnemyCoralineDad : public EnemyBase {
public:
	EnemyCoralineDad(unsigned int objectID, int maxHealth, float movementSpeed);
	~EnemyCoralineDad();

	void Init() override;
	void Update() override;
	void Render() override;

	bool TakeDamage(unsigned int damageAmount) override;

	const Circle GetCollider() const override;
	const EnemyType GetEnemyType() const override;
	const float GetAttackDamage() const override;
	const float GetAttackRange() const override;
	const float GetOrientation() const override;
	const int GetCurrentHealth() const override;
	const unsigned int GetObjectID() const override;
	const Sprite* GetSprite() const override;
	const std::shared_ptr<Timer> GetAttackTimer() const override;
	const Vector2<float> GetPosition() const override;
	const std::vector<std::shared_ptr<EnemyBase>> GetQueriedEnemies() const override;

	void ActivateEnemy(float orienation, Vector2<float> direction, Vector2<float> position) override;
	void DeactivateEnemy() override;
	void HandleAttack() override;

private:
	void UpdateMovement();
	void UpdateTarget();
	void PickWeapon();

	std::shared_ptr<WeaponComponent> _weaponComponent = nullptr;
};

