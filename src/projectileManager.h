#pragma once
#include "projectile.h"

#include <vector>


class ProjectileManager {
public:
	ProjectileManager();
	~ProjectileManager();

	void Init();
	void Update();
	void Render();

	bool CheckCollision(DamageType damageType, unsigned int projectileIndex);

	void AddNewProjectile(DamageType damageType, float orientation, unsigned int projectileDamage,
		Vector2<float> direction, Vector2<float> position);
	void InsertProjectile(DamageType damageType, std::vector<Projectile*>& projectiles, Projectile* projectile);
	void SpawnProjectile(DamageType damageType, float orientation, Vector2<float> direction, Vector2<float> position);
	void RemoveAllProjectiles();
	void RemoveProjectile(DamageType damageType, unsigned int projectileIndex);

	void UpdateQuadTree();

	std::vector<Projectile*> GetActiveProjectiles();

	int BinarySearch(std::vector<Projectile*> projectiles, unsigned int targetID);

private:
	std::vector<Projectile*> _activeEnemyProjectiles;
	std::vector<Projectile*> _inactiveEnemyProjectiles;

	std::vector<Projectile*> _activePlayerProjectiles;
	std::vector<Projectile*> _inactivePlayerProjectiles;

	Circle _intersectedCollider;
	Circle _currentCollider;
	const unsigned int _enemyProjectileDamage = 2;
	const unsigned int _playerProjectileDamage = 30;

	unsigned int _activeProjectileLimit = 5000;
	unsigned int _lastProjectileID = 0;
};

