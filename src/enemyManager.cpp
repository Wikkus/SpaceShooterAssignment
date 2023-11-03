#include "enemyManager.h"

#include "enemyBase.h"
#include "enemyFighter.h"
#include "enemyWizard.h"
#include "gameEngine.h"
#include "quadTree.h"
#include "timerManager.h"

EnemyManager::EnemyManager() {
	_activeEnemies.reserve(_activeEnemyLimit);
}

EnemyManager::~EnemyManager() {
	RemoveAllEnemies();
	for (unsigned int i = 0; i < _inactiveEnemies.size(); i++) {
		_inactiveEnemies[i] = nullptr;
		delete _inactiveEnemies[i];
	}
}

void EnemyManager::Init() {
	_spawnTimer = timerManager->CreateTimer(2.f);
	for (unsigned i = 0; i < _activeEnemyLimit; i++) {
		_inactiveEnemies.emplace_back(new EnemyFighter(_lastEnemyID));
		_inactiveEnemies.back()->Init();
		_lastEnemyID += 1;
		_inactiveEnemies.emplace_back(new EnemyWizard(_lastEnemyID));
		_inactiveEnemies.back()->Init();
		_lastEnemyID += 1;
	}
}

void EnemyManager::Update() {
	if (_spawnTimer->GetTimerFinished() && _activeEnemies.size() < _activeEnemyLimit) {
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

std::vector<EnemyBase*> EnemyManager::GetActiveEnemies() {
	return _activeEnemies;
}

void EnemyManager::AddNewEnemy(EnemyType enemyType, float orientation, Vector2<float> direction, Vector2<float> position) {
	_inactiveEnemies.emplace_back(new EnemyFighter(_lastEnemyID));
	_inactiveEnemies.back()->Init();
	_lastEnemyID += 1;	
	_inactiveEnemies.emplace_back(new EnemyWizard(_lastEnemyID));
	_inactiveEnemies.back()->Init();
	_lastEnemyID += 1;
}

void EnemyManager::EnemySpawner() {
	for (unsigned int i = 0; i < _spawnNumberOfEnemies; i++) {
		std::uniform_int_distribution dist{ 0, 1 };
		if (i < _spawnNumberOfEnemies * 0.5f) {
			float distX = 0.f;
			std::uniform_real_distribution<float> distY{ 10.f, windowHeight - 10.f };
			int temp = dist(randomEngine);
			if (temp == 0) {
				distX = 0;
			} else {
				distX = windowWidth;
			}
			enemyManager->SpawnEnemy(EnemyType::EnemyFighter, 0.f,
				Vector2<float>(0.f, 0.f), Vector2<float>(distX, distY(randomEngine)));

		} else {
			std::uniform_real_distribution<float> distX{ 10.f, windowWidth - 10.f };
			float distY = 0.f;
			int temp = dist(randomEngine);
			if (temp == 0) {
				distY = 0;
			} else {
				distY = windowHeight;
			}
			enemyManager->SpawnEnemy(EnemyType::EnemyFighter, 0.f,
				Vector2<float>(0.f, 0.f), Vector2<float>(distX(randomEngine), distY));
		}
	}
	_spawnTimer->ResetTimer();
}

void EnemyManager::InsertEnemy(EnemyBase* enemy) {
	if (_activeEnemies.empty()) {
		_activeEnemies.emplace_back(enemy);
		return;
	}
	for (unsigned int i = 0; i < _activeEnemies.size(); i++) {
		if (_activeEnemies[i]->GetEnemyID() > enemy->GetEnemyID()) {
			_activeEnemies.insert(_activeEnemies.begin() + i, enemy);
			return;
		}
	}
}

void EnemyManager::SpawnEnemy(EnemyType enemyType, float orientation,
	Vector2<float> direction, Vector2<float> position) {
	if (_inactiveEnemies.empty()) {
		AddNewEnemy(enemyType, orientation, direction, position);
	}
	_inactiveEnemies.back()->ActivateEnemy(orientation, direction, position);
	InsertEnemy(_inactiveEnemies.back());
	_inactiveEnemies.pop_back();
	
}

void EnemyManager::RemoveAllEnemies() {
	while (_activeEnemies.size() > 0) {
		_inactiveEnemies.insert(_inactiveEnemies.begin(), _activeEnemies.back());
		_inactiveEnemies.front()->DeactivateEnemy();
		_activeEnemies.pop_back();
	}
	_spawnTimer->ResetTimer();
}

void EnemyManager::RemoveEnemy(unsigned int enemyID) {
	int enemyIndex = BinarySearch(_activeEnemies, enemyID);
	if (enemyIndex >= 0) {
		_activeEnemies[enemyIndex]->DeactivateEnemy();
		_inactiveEnemies.emplace_back(_activeEnemies[enemyIndex]);
		_activeEnemies.erase(_activeEnemies.begin() + enemyIndex);
	}
}

void EnemyManager::TakeDamage(unsigned int enemyIndex, unsigned int damageAmount) {
	//TakeDamage returns true if the enemy dies
	if(_activeEnemies[enemyIndex]->TakeDamage(damageAmount)) {
		RemoveEnemy(enemyIndex);
	}
}

void EnemyManager::UpdateQuadTree() {
	for (unsigned i = 0; i < _activeEnemies.size(); i++) {
		enemyQuadTree->InsertTemp(_activeEnemies[i], _activeEnemies[i]->GetCollider());
	}
}

int EnemyManager::BinarySearch(std::vector<EnemyBase*> enemies, unsigned int targetID) {
	int low = 0;
	int high = enemies.size() - 1;
	while (low <= high) {
		int mid = low + (high - low) / 2;
		if (enemies[mid]->GetEnemyID() == targetID) {
			return mid;
		}
		if (enemies[mid]->GetEnemyID() < targetID) {
			low = mid + 1;
		} else {
			high = mid - 1;
		}
	}
	return -1;
}

int EnemyManager::Partition(int start, int end) {
	int pivot = _activeEnemies[start]->GetEnemyID();

	int count = 0;
	for (int i = start + 1; i <= end; i++) {
		if (_activeEnemies[i]->GetEnemyID() <= pivot){
			count++;
		}
	}

	// Giving pivot element its correct position
	int pivotIndex = start + count;
	std::swap(_activeEnemies[pivotIndex], _activeEnemies[start]);

	// Sorting left and right parts of the pivot element
	int i = start, k = end;

	while (i < pivotIndex && k > pivotIndex) {

		while (_activeEnemies[i]->GetEnemyID() <= pivot) {
			i++;
		}

		while (_activeEnemies[k]->GetEnemyID() > pivot) {
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

