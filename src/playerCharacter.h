#pragma once
#include "collision.h"
#include "textSprite.h"
#include "sprite.h"
#include "vector2.h"

class Timer;

class PlayerCharacter {
public:
	PlayerCharacter(const char* spritePath, float characterOrientation, Vector2<float> characterPosition);
	~PlayerCharacter();

	void Init();
	void Update();
	void Render();
	void RenderText();

	void ExecuteDeath();
	void FireProjectile();

	void TakeDamage(unsigned int damageAmount);

	const Circle GetCircleCollider() const;

	Sprite* GetSprite();

	const float GetOrientation() const;	

	const int GetCurrentHealth() const;

	const Vector2<float> GetPosition() const;

private:
	void UpdateCollision();
	void UpdateHealthRegen();
	void UpdateInput();
	void UpdateMovement();
	void UpdateTarget();

	Circle _circleCollider;

	const float _attackDamage = 10;
	const float _movementSpeed = 100.f;

	const int _maxHealth = 1000;

	float _orientation = 0.f;

	int _currentHealth = 0;

	Sprite* _characterSprite = nullptr;
	TextSprite* _healthTextSprite = nullptr;

	std::shared_ptr<Timer> _attackTimer = nullptr;
	std::shared_ptr<Timer> _regenerationTimer = nullptr;

	Vector2<float> _position = Vector2<float>(0.f, 0.f);
	Vector2<float> _oldPosition = Vector2<float>(0.f, 0.f);
	Vector2<float> _direction = Vector2<float>(0.f, 0.f);


};

