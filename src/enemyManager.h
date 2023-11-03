#pragma once
#include "vector2.h"

#include <vector>

enum class EnemyType {
	EnemyFighter, 
	EnemyWizard,
	Count
};

class EnemyBase;
class Timer;

class EnemyManager {
public:
	EnemyManager();
	~EnemyManager();

	void Init();
	void Update();
	void Render();

	std::vector<EnemyBase*> GetActiveEnemies();

	void AddNewEnemy(EnemyType enemyType, float orientation,
		Vector2<float> direction, Vector2<float> position);

	void EnemySpawner();

	void InsertEnemy(EnemyBase* enemy);
	
	void SpawnEnemy(EnemyType enemyType, float orientation,
		Vector2<float> direction, Vector2<float> position);

	void RemoveAllEnemies();
	void RemoveEnemy(unsigned int enemyID);

	void TakeDamage(unsigned int enemyIndex, unsigned int damageAmount);

	void UpdateQuadTree();

	int BinarySearch(std::vector<EnemyBase*> enemies, unsigned int targetID);
	int Partition(int start, int end);
	void QuickSort(int start, int end);


private:
	std::vector<EnemyBase*> _activeEnemies;
	std::vector<EnemyBase*> _inactiveEnemies;

	std::shared_ptr<Timer> _spawnTimer = nullptr;

	unsigned int _activeEnemyLimit = 1500;
	unsigned int _lastEnemyID = 0;
	unsigned int _spawnNumberOfEnemies = 50;
};

