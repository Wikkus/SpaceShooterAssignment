#pragma once
#include "collision.h"
#include "sprite.h"
#include "vector2.h"

enum class DamageType {
	DamageEnemy,
	DamagePlayer,
	Count
};

class Projectile {
public:
	Projectile(DamageType damageType, unsigned int projectileDamage, unsigned int projectileID);
	~Projectile();

	void Init();
	void Update();
	void Render();
	
	const Circle GetCollider() const;
	const DamageType GetDamageType() const;
	const unsigned int GetProjectileID() const;
	const unsigned int GetProjectileDamage() const;
	

	float GetOrientation(); 
	Sprite* GetSprite();
	Vector2<float> GetPosition();
	
	void SetDirection(Vector2<float> direction);
	void SetOrientation(float orientation);
	void SetPosition(Vector2<float> position);

	void ActivateProjectile(float orientation, Vector2<float> direction, Vector2<float> position);
	void DeactivateProjectile();

private:
	Circle _circleCollider;

	const float _projectileSpeed = 200.f;
	const float _spriteCollisionOffset = 8.f;

	DamageType _damageType = DamageType::Count;
		
	float _orientation = 0.f;

	unsigned int _projectileID = -1;
	unsigned int _projectileDamage = 0;

	Sprite* _projectileSprite = nullptr;
	
	Vector2<float> _direction = Vector2<float>(0.f, 0.f);
	Vector2<float> _position = Vector2<float>(-10000.f, 10000.f);
};

