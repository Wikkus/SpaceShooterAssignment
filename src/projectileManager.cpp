#include "projectileManager.h"

#include "dataStructuresAndMethods.h"
#include "enemyManager.h"
#include "enemyBase.h"
#include "gameEngine.h"
#include "imGuiManager.h"
#include "objectPool.h"
#include "playerCharacter.h"
#include "quadTree.h"

ProjectileManager::ProjectileManager() {
	QuadTreeNode quadTreeNode;
	quadTreeNode.rectangle = AABB::makeFromPositionSize(
		Vector2(windowWidth * 0.5f, windowHeight * 0.5f), windowHeight, windowWidth);
	_projectileQuadTree = std::make_shared<QuadTree<std::shared_ptr<Projectile>>>(quadTreeNode, 25);

	for (unsigned int i = 0; i < (unsigned int)EnemyType::Count; i++) {
		_projectilePools[(ProjectileType)i] = std::make_shared<ObjectPool<std::shared_ptr<Projectile>>>(_projectileAmountLimit);
	}
	_numberOfProjectileTypes = (unsigned int)EnemyType::Count;
}

ProjectileManager::~ProjectileManager() {}

void ProjectileManager::Init() {
	for (unsigned int i = 0; i < _projectileAmountLimit; i++) {
		if (i % 5 == 0) {
			CreateNewProjectile(ProjectileType::PlayerProjectile, 0.f, 30, {0.f, 0.f}, {-10000.f, -10000.f});
		}
		CreateNewProjectile(ProjectileType::EnemyProjectile, 0.f, 1, {0.f, 0.f}, {-10000.f, -10000.f});
	}
}

void ProjectileManager::Update() {
	for (unsigned int i = 0; i < _activeProjectiles.size(); i++) {
		_activeProjectiles[i]->Update();
		if (CheckCollision(_activeProjectiles[i]->GetProjectileType(), i)) {
			continue;
		}
		if (OutOfBorderX(_activeProjectiles[i]->GetPosition().x) ||
			OutOfBorderY(_activeProjectiles[i]->GetPosition().y)) {
			RemoveProjectile(_activeProjectiles[i]->GetProjectileType(), _activeProjectiles[i]->GetObjectID());
		}
	}
}

void ProjectileManager::Render() {
	for (unsigned int i = 0; i < _activeProjectiles.size(); i++) {
		_activeProjectiles[i]->Render();
	}
}

void ProjectileManager::ClearProjectileQuadTree() {
	_projectileQuadTree->Clear();
}

void ProjectileManager::CreateNewProjectile(ProjectileType projectileType, float orientation, unsigned int projectileDamage, Vector2<float> direction, Vector2<float> position) {
	_projectilePools[projectileType]->PoolObject(std::make_shared<Projectile>(projectileType, projectileDamage, _lastProjectileID));
	_lastProjectileID++;
}

void ProjectileManager::SpawnProjectile(ProjectileType projectileType, float orientation, unsigned int projectileDamage, Vector2<float> direction, Vector2<float> position) {
	if (_projectilePools[projectileType]->IsEmpty()) {
		CreateNewProjectile(projectileType, orientation, projectileDamage, direction, position);
		_activeProjectiles.emplace_back(_projectilePools[projectileType]->SpawnObject());
		_activeProjectiles.back()->ActivateProjectile(orientation, direction, position);
	} else {
		_activeProjectiles.emplace_back(_projectilePools[projectileType]->SpawnObject());
		_activeProjectiles.back()->ActivateProjectile(orientation,direction, position);
	}	
}

bool ProjectileManager::CheckCollision(ProjectileType projectileType, unsigned int projectileIndex) {
	if (projectileType == ProjectileType::PlayerProjectile) {
		std::vector<std::shared_ptr<EnemyBase>> enemiesHit = enemyManager->GetEnemyQuadTree()->Query(_activeProjectiles[projectileIndex]->GetCollider());
		for (unsigned int i = 0; i < enemiesHit.size(); i++) {
			if (enemiesHit[i]->TakeDamage(_activeProjectiles[projectileIndex]->GetProjectileDamage())) {
				enemyManager->RemoveEnemy(enemiesHit[i]->GetEnemyType(), enemiesHit[i]->GetObjectID());
			}
		}
		if (enemiesHit.size() > 0) {
			RemoveProjectile(projectileType, _activeProjectiles[projectileIndex]->GetObjectID());
			return true;
		}
	}
	return false;
}

void ProjectileManager::RemoveAllProjectiles() {
	while (_activeProjectiles.size() > 0) {
		_activeProjectiles.back()->DeactivateProjectile();
		_projectilePools[_activeProjectiles.back()->GetProjectileType()]->PoolObject(_activeProjectiles.back());
		_activeProjectiles.pop_back();
	}
}

void ProjectileManager::RemoveProjectile(ProjectileType projectileType, unsigned int projectileID) {
	if (_activeProjectiles.empty()) {
		return;
	} else if (_activeProjectiles.size() > 1) {
		QuickSort(0, _activeProjectiles.size() - 1);
		_latestProjectileIndex = BinarySearch(0, _activeProjectiles.size() - 1, projectileID);
		if (_latestProjectileIndex >= 0) {
			_activeProjectiles[_latestProjectileIndex]->DeactivateProjectile();
			_projectilePools[projectileType]->PoolObject(_activeProjectiles[_latestProjectileIndex]);
			std::swap(_activeProjectiles[_latestProjectileIndex], _activeProjectiles.back());
		}
		_latestProjectileIndex = -1;
	}
	_activeProjectiles.pop_back();
}

void ProjectileManager::UpdateQuadTree() {
	for (unsigned int i = 0; i < _activeProjectiles.size(); i++) {
		_projectileQuadTree->Insert(_activeProjectiles[i], _activeProjectiles[i]->GetCollider());
	}
}

std::shared_ptr<QuadTree<std::shared_ptr<Projectile>>> ProjectileManager::GetProjectileQuadTree() {
	return _projectileQuadTree;
}

int ProjectileManager::BinarySearch(int low, int high, int targetID) {
	while (low <= high) {
		int mid = low + (high - low) / 2;

		if (_activeProjectiles[mid]->GetObjectID() == targetID) {
			return mid;
		}

		if (_activeProjectiles[mid]->GetObjectID() < targetID) {
			low = mid + 1;
		} else {
			high = mid - 1;
		}
	}
	return -1;
}

int ProjectileManager::Partition(int start, int end) {
	unsigned int pivot = _activeProjectiles[start]->GetObjectID();

	int count = 0;
	for (int i = start + 1; i <= end; i++) {
		if (_activeProjectiles[i]->GetObjectID() <= pivot) {
			count++;
		}
	}

	int pivotIndex = start + count;
	std::swap(_activeProjectiles[pivotIndex], _activeProjectiles[start]);

	int i = start, k = end;

	while (i < pivotIndex && k > pivotIndex) {

		while (_activeProjectiles[i]->GetObjectID() <= pivot) {
			i++;
		}

		while (_activeProjectiles[k]->GetObjectID() > pivot) {
			k--;
		}

		if (i < pivotIndex && k > pivotIndex) {
			std::swap(_activeProjectiles[i++], _activeProjectiles[k--]);
		}
	}
	return pivotIndex;
}

void ProjectileManager::QuickSort(int start, int end) {
	if (start >= end) {
		return;
	}
	int p = Partition(start, end);

	QuickSort(start, p - 1);
	QuickSort(p + 1, end);
}
