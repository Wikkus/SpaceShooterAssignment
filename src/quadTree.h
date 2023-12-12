#pragma once
#include "collision.h"
#include "vector2.h"

#include <array>
#include <vector>

#include "debugDrawer.h"
#include "gameEngine.h"
#include "projectileManager.h"

struct QuadTreeNode {
	AABB rectangle;

	bool Contains(Circle circleCollider);
	bool Intersect(Circle range);
};

template<typename T> 
class QuadTree {
public:
	QuadTree(QuadTreeNode boundary, unsigned int capacity);
	~QuadTree();

	bool Insert(T object, Circle circleCollider);

	std::vector<T> Query(Circle range);

	void Clear();
	void Undevide();

	void Subdevide();

	void Render();

private:
	bool _divided = false;

	unsigned int _capacity = 0;
	QuadTreeNode _quadTreeNode;

	std::array<std::shared_ptr<QuadTree<T>>, 4> _quadTreeChildren;
	std::vector<T> _objectsInserted;
	std::vector<Circle> _circleColliders;
};
template<typename T>
inline QuadTree<T>::QuadTree(QuadTreeNode boundary, unsigned int capacity) {
	_quadTreeNode = boundary;
	_capacity = capacity;

	_quadTreeChildren[0] = nullptr;
	_quadTreeChildren[1] = nullptr;
	_quadTreeChildren[2] = nullptr;
	_quadTreeChildren[3] = nullptr;
}
template<typename T>
inline QuadTree<T>::~QuadTree() {
	Clear();
}

template<typename T>
inline bool QuadTree<T>::Insert(T object, Circle circleCollider) {
	if (!_quadTreeNode.Contains(circleCollider)) {
		return false;
	}
	if (_objectsInserted.size() < _capacity) {
		_objectsInserted.emplace_back(object);
		_circleColliders.emplace_back(circleCollider);
		return true;
	} else {
		if (!_divided) {
			Subdevide();
		}
		if (_quadTreeChildren[0]->Insert(object, circleCollider)) {
			return true;
		} else if (_quadTreeChildren[1]->Insert(object, circleCollider)) {
			return true;
		} else if (_quadTreeChildren[2]->Insert(object, circleCollider)) {
			return true;
		} else if (_quadTreeChildren[3]->Insert(object, circleCollider)) {
			return true;
		}
	}
}
template<typename T>
inline std::vector<T> QuadTree<T>::Query(Circle range) {
	std::vector<T> objectsFound;
	if (_quadTreeNode.Intersect(range)) {
		for (unsigned int i = 0; i < _objectsInserted.size(); i++) {
			if (CircleIntersect(range, _circleColliders[i])) {
				objectsFound.emplace_back(_objectsInserted[i]);
			}
		}
		if (_divided) {
			std::vector<T> objectsFoundInChild;
			if (_quadTreeChildren[0]) {
				objectsFoundInChild = _quadTreeChildren[0]->Query(range);
				objectsFound.insert(objectsFound.end(), objectsFoundInChild.begin(), objectsFoundInChild.end());
			}
			if (_quadTreeChildren[1]) {
				objectsFoundInChild = _quadTreeChildren[1]->Query(range);
				objectsFound.insert(objectsFound.end(), objectsFoundInChild.begin(), objectsFoundInChild.end());
			}
			if (_quadTreeChildren[2]) {
				objectsFoundInChild = _quadTreeChildren[2]->Query(range);
				objectsFound.insert(objectsFound.end(), objectsFoundInChild.begin(), objectsFoundInChild.end());
			}
			if (_quadTreeChildren[3]) {
				objectsFoundInChild = _quadTreeChildren[3]->Query(range);
				objectsFound.insert(objectsFound.end(), objectsFoundInChild.begin(), objectsFoundInChild.end());
			}
		}
	}
	return objectsFound;
}
template<typename T>
inline void QuadTree<T>::Clear() {
	_objectsInserted.clear();
	_circleColliders.clear();
	Undevide();
}
template<typename T>
inline void QuadTree<T>::Undevide() {
	if (_quadTreeChildren[0]) {
		_quadTreeChildren[0]->Undevide();
	}
	if (_quadTreeChildren[1]) {
		_quadTreeChildren[1]->Undevide();
	}
	if (_quadTreeChildren[2]) {
		_quadTreeChildren[2]->Undevide();
	}
	if (_quadTreeChildren[3]) {
		_quadTreeChildren[3]->Undevide();
	}
	_quadTreeChildren[0] = nullptr;
	_quadTreeChildren[1] = nullptr;
	_quadTreeChildren[2] = nullptr;
	_quadTreeChildren[3] = nullptr;
	_divided = false;
}
template<typename T>
inline void QuadTree<T>::Subdevide() {
	QuadTreeNode nw;
	nw.rectangle =
		AABB::makeFromPositionSize(Vector2<float>(
			_quadTreeNode.rectangle.position.x - (_quadTreeNode.rectangle.width * 0.25f),
			_quadTreeNode.rectangle.position.y - (_quadTreeNode.rectangle.height * 0.25f)),
			_quadTreeNode.rectangle.height * 0.5f, _quadTreeNode.rectangle.width * 0.5f);
	_quadTreeChildren[0] = std::make_shared<QuadTree<T>>(nw, _capacity);

	QuadTreeNode ne;
	ne.rectangle = AABB::makeFromPositionSize(Vector2<float>(
		_quadTreeNode.rectangle.position.x + (_quadTreeNode.rectangle.width * 0.25f),
		_quadTreeNode.rectangle.position.y - (_quadTreeNode.rectangle.height * 0.25f)),
		_quadTreeNode.rectangle.height * 0.5f, _quadTreeNode.rectangle.width * 0.5f);
	_quadTreeChildren[1] = std::make_shared<QuadTree<T>>(ne, _capacity);

	QuadTreeNode sw;
	sw.rectangle = AABB::makeFromPositionSize(Vector2<float>(
		_quadTreeNode.rectangle.position.x - (_quadTreeNode.rectangle.width * 0.25f),
		_quadTreeNode.rectangle.position.y + (_quadTreeNode.rectangle.height * 0.25f)),
		_quadTreeNode.rectangle.height * 0.5f, _quadTreeNode.rectangle.width * 0.5f);
	_quadTreeChildren[2] = std::make_shared<QuadTree<T>>(sw, _capacity);

	QuadTreeNode se;
	se.rectangle = AABB::makeFromPositionSize(Vector2<float>(
		_quadTreeNode.rectangle.position.x + (_quadTreeNode.rectangle.width * 0.25f),
		_quadTreeNode.rectangle.position.y + (_quadTreeNode.rectangle.height * 0.25f)),
		_quadTreeNode.rectangle.height * 0.5f, _quadTreeNode.rectangle.width * 0.5f);
	_quadTreeChildren[3] = std::make_shared<QuadTree<T>>(se, _capacity);
	_divided = true;
}
template<typename T>
inline void QuadTree<T>::Render() {
	debugDrawer->AddDebugBox(
		_quadTreeNode.rectangle.position, Vector2<float>(_quadTreeNode.rectangle.min.x, _quadTreeNode.rectangle.min.y),
		Vector2<float>(_quadTreeNode.rectangle.max.x, _quadTreeNode.rectangle.max.y), { 255, 125, 0, 255});

	for (unsigned int i = 0; i < _quadTreeChildren.size(); i++) {
		if (_quadTreeChildren[i]) {
			_quadTreeChildren[i]->Render();
		}
	}
}