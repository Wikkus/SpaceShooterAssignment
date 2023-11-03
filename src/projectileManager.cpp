#include "projectileManager.h"

#include "dataStructuresAndMethods.h"
#include "enemyFighter.h"
#include "enemyManager.h"
#include "gameEngine.h"
#include "imGuiManager.h"
#include "playerCharacter.h"
#include "quadTree.h"

ProjectileManager::ProjectileManager() {
	_activeEnemyProjectiles.reserve(_activeProjectileLimit);
	_activePlayerProjectiles.reserve(_activeProjectileLimit * 0.5f);
}

ProjectileManager::~ProjectileManager() {
	RemoveAllProjectiles();
	for (unsigned int i = 0; i < _inactiveEnemyProjectiles.size(); i++) {
		_inactiveEnemyProjectiles[i] = nullptr;
		delete _inactiveEnemyProjectiles[i];
	}	
	for (unsigned int i = 0; i < _inactivePlayerProjectiles.size(); i++) {
		_inactivePlayerProjectiles[i] = nullptr;
		delete _inactivePlayerProjectiles[i];
	}
}

void ProjectileManager::Init() {
	for (unsigned int i = 0; i < _activeProjectileLimit; i++) {
		_inactiveEnemyProjectiles.emplace_back(new Projectile(DamageType::DamagePlayer, _enemyProjectileDamage, i));
		if (i <= _activeProjectileLimit * 0.5f) {
			_inactivePlayerProjectiles.emplace_back(new Projectile(DamageType::DamageEnemy, _playerProjectileDamage, i + 10000));
		}
		_lastProjectileID += 1;
	}
}

void ProjectileManager::Update() {
	for (unsigned int i = 0; i < _activePlayerProjectiles.size(); i++) {
		_activePlayerProjectiles[i]->Update();		
		if (CheckCollision(DamageType::DamageEnemy, i)) {
			continue;
		}
		if (OutOfBorderX(_activePlayerProjectiles[i]->GetPosition().x) ||
			OutOfBorderY(_activePlayerProjectiles[i]->GetPosition().y)) {
			RemoveProjectile(DamageType::DamageEnemy, _activePlayerProjectiles[i]->GetProjectileID());
		}
	}
	for (unsigned int i = 0; i < _activeEnemyProjectiles.size(); i++) {
		_activeEnemyProjectiles[i]->Update();
		if (OutOfBorderX(_activeEnemyProjectiles[i]->GetPosition().x) ||
			OutOfBorderY(_activeEnemyProjectiles[i]->GetPosition().y)) {
			RemoveProjectile(DamageType::DamagePlayer, _activeEnemyProjectiles[i]->GetProjectileID());
		}
	}
}

void ProjectileManager::Render() {
	for (unsigned int i = 0; i < _activeEnemyProjectiles.size(); i++) {
		_activeEnemyProjectiles[i]->Render();
	}
	for (unsigned int i = 0; i < _activePlayerProjectiles.size(); i++) {
		_activePlayerProjectiles[i]->Render();
	}
}

void ProjectileManager::AddNewProjectile(DamageType damageType, float orientation, unsigned int projectileDamage, Vector2<float> direction, Vector2<float> position) {
	if (damageType == DamageType::DamageEnemy) {
		_inactivePlayerProjectiles.emplace_back(new Projectile(DamageType::DamageEnemy, projectileDamage, _lastProjectileID));
		_inactivePlayerProjectiles.back()->Init();

	} else if (damageType == DamageType::DamagePlayer) {
		_inactiveEnemyProjectiles.emplace_back(new Projectile(DamageType::DamagePlayer, projectileDamage, _lastProjectileID));
		_inactiveEnemyProjectiles.back()->Init();
	}
	_lastProjectileID += 1;
}
void ProjectileManager::InsertProjectile(DamageType damageType, std::vector<Projectile*>& projectiles, Projectile* projectile) {
	if (projectiles.empty()) {
		projectiles.emplace_back(projectile);
		return;
	}
	for (unsigned int i = 0; i < projectiles.size(); i++) {
		if (projectiles[i]->GetProjectileID() > projectile->GetProjectileID()) {
			projectiles.insert(projectiles.begin() + i, projectile);
			return;
		}
	}
}

void ProjectileManager::SpawnProjectile(DamageType damageType, float orientation, Vector2<float> direction, Vector2<float> position) {
	if (damageType == DamageType::DamageEnemy) {
		if (_inactivePlayerProjectiles.empty()) {
			AddNewProjectile(damageType, orientation, _playerProjectileDamage, direction, position);
		}
		_inactivePlayerProjectiles.back()->ActivateProjectile(orientation, direction, position);
		InsertProjectile(damageType, _activePlayerProjectiles, _inactivePlayerProjectiles.back());
		_inactivePlayerProjectiles.pop_back();

	} else if (damageType == DamageType::DamagePlayer) {
		if (_inactiveEnemyProjectiles.empty()) {
			AddNewProjectile(damageType, orientation, _enemyProjectileDamage, direction, position);
		}
		_inactiveEnemyProjectiles.back()->ActivateProjectile(orientation, direction, position);
		InsertProjectile(damageType, _activeEnemyProjectiles, _inactiveEnemyProjectiles.back());
		_inactiveEnemyProjectiles.pop_back();
	}
}

bool ProjectileManager::CheckCollision(DamageType damageType, unsigned int projectileIndex) {
	if (damageType == DamageType::DamageEnemy) {
		std::vector<EnemyBase*> enemiesHit = enemyQuadTree->QueryTemp(_activePlayerProjectiles[projectileIndex]->GetCollider());
		for (unsigned int i = 0; i < enemiesHit.size(); i++) {
			if (enemiesHit[i]->TakeDamage(_activePlayerProjectiles[projectileIndex]->GetProjectileDamage())) {
				enemyManager->RemoveEnemy(enemiesHit[i]->GetEnemyID());
			}
		}
		if (enemiesHit.size() > 0) {
			RemoveProjectile(damageType, _activePlayerProjectiles[projectileIndex]->GetProjectileID());
			return true;
		}
	}
	return false;
}

void ProjectileManager::RemoveAllProjectiles() {
	while (_activeEnemyProjectiles.size() > 0) {
		_inactiveEnemyProjectiles.insert(_inactiveEnemyProjectiles.begin(), _activeEnemyProjectiles.back());
		_inactiveEnemyProjectiles.back()->DeactivateProjectile();
		_activeEnemyProjectiles.pop_back();
	}
	while (_activePlayerProjectiles.size() > 0) {
		_inactivePlayerProjectiles.insert(_inactivePlayerProjectiles.begin(), _activePlayerProjectiles.back());
		_inactivePlayerProjectiles.back()->DeactivateProjectile();
		_activePlayerProjectiles.pop_back();
	}
}

void ProjectileManager::RemoveProjectile(DamageType damageType, unsigned int projectileID) {
	if (damageType == DamageType::DamagePlayer) {
		int projectileIndex = BinarySearch(_activeEnemyProjectiles, projectileID);
		if (projectileIndex >= 0) {
			_activeEnemyProjectiles[projectileIndex]->DeactivateProjectile();
			_inactiveEnemyProjectiles.emplace_back(_activeEnemyProjectiles[projectileIndex]);
			_activeEnemyProjectiles.erase(_activeEnemyProjectiles.begin() + projectileIndex);
		}

	} else if (damageType == DamageType::DamageEnemy) {
		int projectileIndex = BinarySearch(_activePlayerProjectiles, projectileID);
		if (projectileIndex >= 0) {
			_activePlayerProjectiles[projectileIndex]->DeactivateProjectile();
			_inactivePlayerProjectiles.emplace_back(_activePlayerProjectiles[projectileIndex]);
			_activePlayerProjectiles.erase(_activePlayerProjectiles.begin() + projectileIndex);
		}
	}
}

std::vector<Projectile*> ProjectileManager::GetActiveProjectiles() {
	return _activePlayerProjectiles;
}

void ProjectileManager::UpdateQuadTree() {
	for (unsigned int i = 0; i < _activeEnemyProjectiles.size(); i++) {
		projectileQuadTree->InsertTemp(_activeEnemyProjectiles[i], _activeEnemyProjectiles[i]->GetCollider());
	}
}

int ProjectileManager::BinarySearch(std::vector<Projectile*> projectiles, unsigned int targetID) {
	int low = 0;
	int high = projectiles.size() - 1;
	while (low <= high) {
		int mid = low + (high - low) / 2;
		if (projectiles[mid]->GetProjectileID() == targetID) {
			return mid;
		}
		if (projectiles[mid]->GetProjectileID() < targetID) {
			low = mid + 1;
		} else {
			high = mid - 1;
		}
	}
	return -1;
}