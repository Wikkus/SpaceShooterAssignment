#include "weaponComponent.h"

#include "dataStructuresAndMethods.h"
#include "gameEngine.h"
#include "playerCharacter.h"
#include "projectileManager.h"
#include "sprite.h"
#include "timerManager.h"

SwordComponent::SwordComponent() {
	_sprite = std::make_shared<Sprite>();
	_sprite->Load("res/sprites/Sword.png");
	_attackTimer = timerManager->CreateTimer(1.f);
	_attackDamage = 2;
	_attackRange = 25.f;
}

void SwordComponent::Render(Vector2<float> position, float orientation) {
	_sprite->RenderWithOrientation(position, orientation);
}

void SwordComponent::Attack(Vector2<float> position, Vector2<float> targetPosition, float orientation) {
	if (IsInDistance(playerCharacter->GetPosition(), position, _attackRange) && _attackTimer->GetTimerFinished()) {
		playerCharacter->TakeDamage(_attackDamage);
		_attackTimer->ResetTimer();
	}
}

const float SwordComponent::GetAttackDamage() const {
	return _attackDamage;
}

const float SwordComponent::GetAttackRange() const {
	return _attackRange;
}

WizardHatComponent::WizardHatComponent() {
	_sprite = std::make_shared<Sprite>();
	_sprite->Load("res/sprites/WizardHat.png");
	_attackTimer = timerManager->CreateTimer(1.5f);
	_attackDamage = 1;
	_attackRange = 300.f;
}

void WizardHatComponent::Render(Vector2<float> position, float orientation) {
	_sprite->RenderWithOrientation(position, orientation);
}

void WizardHatComponent::Attack(Vector2<float> position, Vector2<float> targetPosition, float orientation) {
	if (IsInDistance(playerCharacter->GetPosition(), position, _attackRange) && _attackTimer->GetTimerFinished()) {
		projectileManager->SpawnProjectile(ProjectileType::EnemyProjectile, orientation, _attackDamage, 
			Vector2<float>(targetPosition - position).normalized(), position);
		_attackTimer->ResetTimer();
	}
}

const float WizardHatComponent::GetAttackDamage() const {
	return _attackDamage;
}

const float WizardHatComponent::GetAttackRange() const {
	return _attackRange;
}
