#pragma once
#include "collision.h"
#include "vector2.h"

#include <array>
#include <vector>

#include "debugDrawer.h"
#include "enemyBase.h"
#include "gameEngine.h"
#include "projectileManager.h"

struct QuadTreeNode {
	AABB rectangle;

	bool Contains(Circle circleCollider);
	bool Intersect(Circle range);
};

template<typename T> 
class QuadTreeTemp {
public:
	QuadTreeTemp(QuadTreeNode boundary, unsigned int capacity);
	~QuadTreeTemp();

	bool InsertTemp(T object, Circle circleCollider);

	std::vector<T> QueryTemp(Circle range);

	void ClearTemp();
	void UndevideTemp();

	void SubdevideTemp();

	void RenderTemp();

private:
	bool _divided = false;

	unsigned int _capacity = 0;
	QuadTreeNode _quadTreeNode;


	std::array<QuadTreeTemp<T>*, 4> _quadTreeChildren;
	std::vector<T> _objectsInserted;
	std::vector<Circle> _circleColliders;
};

//template<typename T>
//inline QuadTreeTemp<T>::QuadTreeTemp(QuadTreeNode boundary, unsigned int capacity) {
//	_quadTreeNode = boundary;
//	_capacity = capacity;
//
//	_quadTreeChildren[0] = nullptr;
//	_quadTreeChildren[1] = nullptr;
//	_quadTreeChildren[2] = nullptr;
//	_quadTreeChildren[3] = nullptr;
//}
//
//template<typename T>
//inline QuadTreeTemp<T>::~QuadTreeTemp() {
//	ClearTemp();
//}
//
//template<typename T>
//inline bool QuadTreeTemp<T>::InsertTemp(T object, Circle circleCollider) {
//	if (!_quadTreeNode.Contains(circleCollider)) {
//		return false;
//	}
//	if (_objectsInserted.size() < _capacity) {
//		_objectsInserted.emplace_back(object);
//		_circleColliders.emplace_back(circleCollider);
//		return true;
//	} else {
//		if (!_divided) {
//			SubdevideTemp();
//		}
//		if (_quadTreeChildren[0]->InsertTemp(object, circleCollider)) {
//			return true;
//		} else if (_quadTreeChildren[1]->InsertTemp(object, circleCollider)) {
//			return true;
//		} else if (_quadTreeChildren[2]->InsertTemp(object, circleCollider)) {
//			return true;
//		} else if (_quadTreeChildren[3]->InsertTemp(object, circleCollider)) {
//			return true;
//		}
//	}
//}
//
//template<typename T>
//inline std::vector<T> QuadTreeTemp<T>::QueryTemp(Circle range) {
//	std::vector<T> objectsFound;
//	if (_quadTreeNode.Intersect(range)) {
//		for (unsigned int i = 0; i < _objectsInserted.size(); i++) {
//			if (CircleIntersect(range, _circleColliders[i])) {
//				objectsFound.emplace_back(_objectsInserted[i]);
//			}
//		}
//		if (_divided) {
//			std::vector<T> objectsFoundInChild;
//			if (_quadTreeChildren[0]) {
//				objectsFoundInChild = _quadTreeChildren[0]->QueryTemp(range);
//				objectsFound.insert(objectsFound.end(), objectsFoundInChild.begin(), objectsFoundInChild.end());
//			}
//			if (_quadTreeChildren[1]) {
//				objectsFoundInChild = _quadTreeChildren[1]->QueryTemp(range);
//				objectsFound.insert(objectsFound.end(), objectsFoundInChild.begin(), objectsFoundInChild.end());
//			}
//			if (_quadTreeChildren[2]) {
//				objectsFoundInChild = _quadTreeChildren[2]->QueryTemp(range);
//				objectsFound.insert(objectsFound.end(), objectsFoundInChild.begin(), objectsFoundInChild.end());
//			}
//			if (_quadTreeChildren[3]) {
//				objectsFoundInChild = _quadTreeChildren[3]->QueryTemp(range);
//				objectsFound.insert(objectsFound.end(), objectsFoundInChild.begin(), objectsFoundInChild.end());
//			}
//		}
//	}
//	return objectsFound;
//}
//
//template<typename T>
//inline void QuadTreeTemp<T>::ClearTemp() {
//	_objectsInserted.clear();
//	_circleColliders.clear();
//	UndevideTemp();
//}
//
//template<typename T>
//inline void QuadTreeTemp<T>::UndevideTemp() {
//	if (_quadTreeChildren[0]) {
//		_quadTreeChildren[0]->UndevideTemp();
//	}	
//	if (_quadTreeChildren[1]) {
//		_quadTreeChildren[1]->UndevideTemp();
//	}	
//	if (_quadTreeChildren[2]) {
//		_quadTreeChildren[2]->UndevideTemp();
//	}	
//	if (_quadTreeChildren[3]) {
//		_quadTreeChildren[3]->UndevideTemp();
//	}
//
//	_quadTreeChildren[0] = nullptr;
//	delete _quadTreeChildren[0];
//
//	_quadTreeChildren[1] = nullptr;
//	delete _quadTreeChildren[1];
//
//	_quadTreeChildren[2] = nullptr;
//	delete _quadTreeChildren[2];
//
//	_quadTreeChildren[3] = nullptr;
//	delete _quadTreeChildren[3];
//
//	_divided = false;
//}
//
//template<typename T>
//inline void QuadTreeTemp<T>::SubdevideTemp() {
//	QuadTreeNode nw;
//	nw.rectangle =
//		AABB::makeFromPositionSize(Vector2<float>(
//			_quadTreeNode.rectangle.position.x - (_quadTreeNode.rectangle.width * 0.25f),
//			_quadTreeNode.rectangle.position.y - (_quadTreeNode.rectangle.height * 0.25f)),
//			_quadTreeNode.rectangle.height * 0.5f, _quadTreeNode.rectangle.width * 0.5f);
//	_quadTreeChildren[0] = new QuadTreeTemp<T>(nw, _capacity);
//
//	QuadTreeNode ne;
//	ne.rectangle = AABB::makeFromPositionSize(Vector2<float>(
//		_quadTreeNode.rectangle.position.x + (_quadTreeNode.rectangle.width * 0.25f),
//		_quadTreeNode.rectangle.position.y - (_quadTreeNode.rectangle.height * 0.25f)),
//		_quadTreeNode.rectangle.height * 0.5f, _quadTreeNode.rectangle.width * 0.5f);
//	_quadTreeChildren[1] = new QuadTreeTemp<T>(ne, _capacity);
//
//	QuadTreeNode sw;
//	sw.rectangle = AABB::makeFromPositionSize(Vector2<float>(
//		_quadTreeNode.rectangle.position.x - (_quadTreeNode.rectangle.width * 0.25f),
//		_quadTreeNode.rectangle.position.y + (_quadTreeNode.rectangle.height * 0.25f)),
//		_quadTreeNode.rectangle.height * 0.5f, _quadTreeNode.rectangle.width * 0.5f);
//	_quadTreeChildren[2] = new QuadTreeTemp<T>(sw, _capacity);
//
//	QuadTreeNode se;
//	se.rectangle = AABB::makeFromPositionSize(Vector2<float>(
//		_quadTreeNode.rectangle.position.x + (_quadTreeNode.rectangle.width * 0.25f),
//		_quadTreeNode.rectangle.position.y + (_quadTreeNode.rectangle.height * 0.25f)),
//		_quadTreeNode.rectangle.height * 0.5f, _quadTreeNode.rectangle.width * 0.5f);
//	_quadTreeChildren[3] = new QuadTreeTemp<T>(se, _capacity);
//
//	_divided = true;
//}
//
//template<typename T>
//inline void QuadTreeTemp<T>::RenderTemp() {
//	debugDrawer->AddDebugBox(
//		_quadTreeNode.rectangle.position, Vector2<float>(_quadTreeNode.rectangle.min.x, _quadTreeNode.rectangle.min.y),
//		Vector2<float>(_quadTreeNode.rectangle.max.x, _quadTreeNode.rectangle.max.y));
//
//	for (unsigned int i = 0; i < _quadTreeChildren.size(); i++) {
//		if (_quadTreeChildren[i]) {
//			_quadTreeChildren[i]->RenderTemp();
//		}
//	}
//}
