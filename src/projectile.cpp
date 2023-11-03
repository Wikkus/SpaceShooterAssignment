#include "projectile.h"

#include "debugDrawer.h"
#include "enemyFighter.h"
#include "enemyManager.h"
#include "gameEngine.h"

Projectile::Projectile(DamageType damageType, unsigned int projectileDamage, unsigned int projectileID) {
	_projectileSprite = new Sprite();
	if (damageType == DamageType::DamageEnemy) {
		_projectileSprite->Load("res/sprites/Fireball.png");

	} else if (damageType == DamageType::DamagePlayer) {
		_projectileSprite->Load("res/sprites/Arcaneball.png");
	}
	_damageType = damageType;
	_projectileDamage = projectileDamage;
	_projectileID = projectileID;	

	_circleCollider.radius = 8.f;
	_circleCollider.position = _position;
}

Projectile::~Projectile() {
	_projectileSprite = nullptr;
	delete _projectileSprite;
}

void Projectile::Init() {}

void Projectile::Update() {
	_position += _direction * _projectileSpeed * deltaTime;
	_circleCollider.position = _position + _direction * (_projectileSprite->h * 0.25f);
}

void Projectile::Render() {
	_projectileSprite->RenderWithOrientation(_position, _orientation);
}

const Circle Projectile::GetCollider() const {
	return _circleCollider;
}

const DamageType Projectile::GetDamageType() const {
	return _damageType;
}

const unsigned int Projectile::GetProjectileID() const {
	return _projectileID;
}

Sprite* Projectile::GetSprite() {
	return _projectileSprite;
}

float Projectile::GetOrientation() {
	return _orientation;
}

const unsigned int Projectile::GetProjectileDamage() const {
	return _projectileDamage;
}

Vector2<float> Projectile::GetPosition() {
	return _position;
}

void Projectile::SetDirection(Vector2<float> direction) {
	_direction = direction;
}

void Projectile::SetOrientation(float orientation) {
	_orientation = orientation;
}

void Projectile::SetPosition(Vector2<float> position) {
	_position = position;
	_circleCollider.position = _position + _direction * (_projectileSprite->h * 0.25f);
}

void Projectile::ActivateProjectile(float orientation, Vector2<float> direction, Vector2<float> position) {
	_orientation = orientation;
	_direction = direction.normalized();
	_position = position;
	_circleCollider.position = _position + _direction * (_projectileSprite->h * 0.25f);
}

void Projectile::DeactivateProjectile() {
	_orientation = 0.f;
	_direction = Vector2<float>(0.f, 0.f);
	_position = Vector2<float>(-10000.f, 10000.f);
	_circleCollider.position = _position;
}
