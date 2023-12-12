#pragma once
#include "quadTree.h"
#include "vector2.h"

#include <vector>
#include <unordered_map>
#include <memory>

class EnemyBase;
class Timer;
template<typename T> class ObjectPool;
template<typename T> class QuadTree;

enum class EnemyType;

class EnemyManager {
public:
	EnemyManager();
	~EnemyManager();

	void Init();
	void Update();
	void Render();

	std::vector<std::shared_ptr<EnemyBase>> GetActiveEnemies();
	std::shared_ptr<QuadTree<std::shared_ptr<EnemyBase>>> GetEnemyQuadTree();

	void ClearEnemyQuadTree();

	void CreateNewEnemy(EnemyType enemyType, float orientation,
		Vector2<float> direction, Vector2<float> position);

	void EnemySpawner();


	void SpawnEnemy(EnemyType enemyType, float orientation,
		Vector2<float> direction, Vector2<float> position);

	void RemoveAllEnemies();
	void RemoveEnemy(EnemyType enemyType, unsigned int objectID);

	void TakeDamage(unsigned int enemyIndex, unsigned int damageAmount);

	void UpdateQuadTree();

	int BinarySearch(int low, int high, int objectID);

	int Partition(int start, int end);
	void QuickSort( int start, int end);

private:
	std::shared_ptr<QuadTree<std::shared_ptr<EnemyBase>>> _enemyQuadTree;

	std::vector<std::shared_ptr<EnemyBase>> _activeEnemies;

	std::shared_ptr<Timer> _spawnTimer = nullptr;

	std::unordered_map<EnemyType, std::shared_ptr<ObjectPool<std::shared_ptr<EnemyBase>>>> _enemyPools;

	int _lastEnemyID = 0;
	int _latestEnemyIndex = -1;

	unsigned int _enemyAmountLimit = 1000;
	unsigned int _numberOfEnemyTypes = 0;
	unsigned int _spawnNumberOfEnemies = 25;
};

