#include "enemyManager.h"

#include "enemyBase.h"
#include "enemyBoar.h"
#include "enemyCoralineDad.h"
#include "gameEngine.h"
#include "objectPool.h"
#include "playerCharacter.h"
#include "quadTree.h"
#include "timerManager.h"
#include "weaponComponent.h"

EnemyManager::EnemyManager() {
	QuadTreeNode quadTreeNode;
	quadTreeNode.rectangle = AABB::makeFromPositionSize(
		Vector2(windowWidth * 0.5f, windowHeight * 0.5f), windowHeight, windowWidth);
	_enemyQuadTree = std::make_shared<QuadTree<std::shared_ptr<EnemyBase>>>(quadTreeNode, 25);

	for (unsigned int i = 0; i < (unsigned int)EnemyType::Count; i++) {
		_enemyPools[(EnemyType)i] = std::make_shared<ObjectPool<std::shared_ptr<EnemyBase>>>(_enemyAmountLimit);
	}
	_numberOfEnemyTypes = (unsigned int)EnemyType::Count;
}

EnemyManager::~EnemyManager() {
	RemoveAllEnemies();
}

void EnemyManager::Init() {
	_spawnTimer = timerManager->CreateTimer(2.f);
	int enemyTypes = (int)EnemyType::Count;
	for (unsigned i = 0; i < _enemyAmountLimit / enemyTypes; i++) {
		for (unsigned int k = 0; k < _numberOfEnemyTypes; k++) {
			CreateNewEnemy(EnemyType(k), 0.f, {0.f, 0.f}, {-10000.f, -10000.f});
		}
	}
}

void EnemyManager::Update() {
	if (_spawnTimer->GetTimerFinished() && _activeEnemies.size() < _enemyAmountLimit) {
		EnemySpawner();
	}
	for (unsigned i = 0; i < _activeEnemies.size(); i++) {
		_activeEnemies[i]->Update();
	}
}

void EnemyManager::Render() {
	for (unsigned i = 0; i < _activeEnemies.size(); i++) {
		_activeEnemies[i]->Render();
	}
}

std::vector<std::shared_ptr<EnemyBase>> EnemyManager::GetActiveEnemies() {
	return _activeEnemies;
}

std::shared_ptr<QuadTree<std::shared_ptr<EnemyBase>>> EnemyManager::GetEnemyQuadTree() {
	return _enemyQuadTree;
}

void EnemyManager::ClearEnemyQuadTree() {
	_enemyQuadTree->Clear();
}

void EnemyManager::CreateNewEnemy(EnemyType enemyType, float orientation, Vector2<float> direction, Vector2<float> position) {
	switch (enemyType) {
	case EnemyType::Boar:
		_enemyPools[enemyType]->PoolObject(std::make_shared<EnemyBoar>(_lastEnemyID, 1, 20, 15.f, 100.f));
		break;

	case EnemyType::CoralineDad:
		_enemyPools[enemyType]->PoolObject(std::make_shared<EnemyCoralineDad>(_lastEnemyID, 15, 75.f));
		break;
	
	default:
		break;
	}
	_lastEnemyID ++;
}

void EnemyManager::EnemySpawner() {
	for (unsigned int i = 0; i < _spawnNumberOfEnemies; i++) {
		std::uniform_int_distribution dist{ 0, 1 };
		Vector2<float> spawnPosition = { 0.f, 0.f };

		if (i < _spawnNumberOfEnemies * 0.5f) {
			float distX = 0.f;
			std::uniform_real_distribution<float> distY{ 0.f, windowHeight };
			int temp = dist(randomEngine);
			if (temp == 0) {
				distX = 0;
			} else {
				distX = windowWidth;
			}
			spawnPosition = { distX, distY(randomEngine) };

		} else {
			std::uniform_real_distribution<float> distX{ 0.f, windowWidth };
			float distY = 0.f;
			int temp = dist(randomEngine);
			if (temp == 0) {
				distY = 0;
			} else {
				distY = windowHeight;
			}
			spawnPosition = { distX(randomEngine), distY };
		}
		if (i % 3 == 0) {
			enemyManager->SpawnEnemy(EnemyType::Boar, 0.f, Vector2<float>(0.f, 0.f), spawnPosition);
		} else {
			enemyManager->SpawnEnemy(EnemyType::CoralineDad, 0.f, Vector2<float>(0.f, 0.f), spawnPosition);
		}

	}
	_spawnTimer->ResetTimer();
}

void EnemyManager::SpawnEnemy(EnemyType enemyType, float orientation,
	Vector2<float> direction, Vector2<float> position) {
	if (_enemyPools[enemyType]->IsEmpty()) {
		CreateNewEnemy(enemyType, orientation, direction, position);
	}
	_activeEnemies.emplace_back(_enemyPools[enemyType]->SpawnObject());
	_activeEnemies.back()->ActivateEnemy(orientation, direction, position);
}

void EnemyManager::RemoveAllEnemies() {
	while (_activeEnemies.size() > 0) {
		_activeEnemies.back()->DeactivateEnemy();
		_enemyPools[_activeEnemies.back()->GetEnemyType()]->PoolObject(_activeEnemies.back());
		_activeEnemies.pop_back();
	}
	_spawnTimer->ResetTimer();
}

void EnemyManager::RemoveEnemy(EnemyType enemyType, unsigned int objectID) {
	if (_activeEnemies.empty()) {
		return;
	} else if (_activeEnemies.size() > 1) {
		QuickSort(0, _activeEnemies.size() - 1);
		_latestEnemyIndex = BinarySearch(0, _activeEnemies.size() - 1, objectID);
		if (_latestEnemyIndex >= 0) {
			_activeEnemies[_latestEnemyIndex]->DeactivateEnemy();
			_enemyPools[enemyType]->PoolObject(_activeEnemies[_latestEnemyIndex]);
			std::swap(_activeEnemies[_latestEnemyIndex], _activeEnemies.back());
		}
		_latestEnemyIndex = -1;
	}
	_activeEnemies.pop_back();
}

void EnemyManager::TakeDamage(unsigned int enemyIndex, unsigned int damageAmount) {
	if(_activeEnemies[enemyIndex]->TakeDamage(damageAmount)) {
		RemoveEnemy(_activeEnemies[enemyIndex]->GetEnemyType(), enemyIndex);
	}
}

void EnemyManager::UpdateQuadTree() {
	for (unsigned i = 0; i < _activeEnemies.size(); i++) {
		_enemyQuadTree->Insert(_activeEnemies[i], _activeEnemies[i]->GetCollider());
	}
}

int EnemyManager::BinarySearch(int low, int high, int objectID) {
	while (low <= high) {
		int mid = low + (high - low) / 2;

		if (_activeEnemies[mid]->GetObjectID() == objectID) {
			return mid;
		}

		if (_activeEnemies[mid]->GetObjectID() < objectID) {
			low = mid + 1;
		} else {
			high = mid - 1;
		}
	}
	return -1;
}

int EnemyManager::Partition(int start, int end) {
	int pivot = _activeEnemies[start]->GetObjectID();

	int count = 0;
	for (int i = start + 1; i <= end; i++) {
		if (_activeEnemies[i]->GetObjectID() <= pivot) {
			count++;
		}
	}

	int pivotIndex = start + count;
	std::swap(_activeEnemies[pivotIndex], _activeEnemies[start]);

	int i = start, k = end;

	while (i < pivotIndex && k > pivotIndex) {

		while (_activeEnemies[i]->GetObjectID() <= pivot) {
			i++;
		}

		while (_activeEnemies[k]->GetObjectID() > pivot) {
			k--;
		}

		if (i < pivotIndex && k > pivotIndex) {
			std::swap(_activeEnemies[i++], _activeEnemies[k--]);
		}
	}
	return pivotIndex;
}

void EnemyManager::QuickSort(int start, int end) {
	if (start >= end) {
		return;
	}
	int p = Partition(start, end);

	QuickSort(start, p - 1);
	QuickSort(p + 1, end);
}
