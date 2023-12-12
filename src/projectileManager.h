#pragma once
#include "projectile.h"

#include <unordered_map>
#include <vector>

template<typename T> class ObjectPool;
template<typename T> class QuadTree;

class ProjectileManager {
public:
	ProjectileManager();
	~ProjectileManager();

	void Init();
	void Update();
	void Render();

	bool CheckCollision(ProjectileType projectileType, unsigned int projectileIndex);

	void ClearProjectileQuadTree();
	void CreateNewProjectile(ProjectileType projectileType, float orientation, unsigned int projectileDamage,
		Vector2<float> direction, Vector2<float> position);
	void SpawnProjectile(ProjectileType projectileType, float orientation, unsigned int projectileDamage, Vector2<float> direction, Vector2<float> position);
	void RemoveAllProjectiles();
	void RemoveProjectile(ProjectileType projectileType, unsigned int projectileIndex);

	void UpdateQuadTree();

	std::shared_ptr<QuadTree<std::shared_ptr<Projectile>>> GetProjectileQuadTree();

	int BinarySearch(int low, int high, int targetID);

	int Partition(int start, int end);
	void QuickSort(int start, int end);

private:
	std::unordered_map<ProjectileType, std::shared_ptr<ObjectPool<std::shared_ptr<Projectile>>>> _projectilePools;
	std::vector<std::shared_ptr<Projectile>> _activeProjectiles;

	std::shared_ptr<QuadTree<std::shared_ptr<Projectile>>> _projectileQuadTree;

	unsigned int _projectileAmountLimit = 2000;
	unsigned int _numberOfProjectileTypes = 0;

	unsigned int _lastProjectileID = 0;
	int _latestProjectileIndex = -1;

};

