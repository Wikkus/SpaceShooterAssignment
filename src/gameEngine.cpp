#include "gameEngine.h"

#include "debugDrawer.h"
#include "enemyBase.h"
#include "enemyManager.h"
#include "imGuiManager.h"
#include "playerCharacter.h"
#include "quadTree.h"
#include "steeringBehaviour.h"
#include "timerManager.h"

#include <vector>

SDL_Window* window;
SDL_Renderer* renderer;

std::shared_ptr<EnemyManager> enemyManager;
std::shared_ptr<DebugDrawer> debugDrawer;
std::shared_ptr<ImGuiHandler> imGuiHandler;
std::shared_ptr<PlayerCharacter> playerCharacter;

std::shared_ptr<ProjectileManager> projectileManager;

QuadTreeTemp<EnemyBase*>* enemyQuadTree;
QuadTreeTemp<Projectile*>* projectileQuadTree;

std::shared_ptr<SteeringBehaviour> separationBehaviour;
std::shared_ptr<TimerManager> timerManager;

float windowHeight = 600.f;
float windowWidth = 800.f;

MouseButtonState mouseButtons[6];

KeyState keys[SDL_NUM_SCANCODES];
bool GetMouseButton(Uint8 button) {
	return mouseButtons[button].state;
}

bool GetMouseButtonPressed(Uint8 button) {
	return mouseButtons[button].state && mouseButtons[button].changeFrame == frameNumber;
}

bool GetMouseButtonReleased(Uint8 button) {
	return !mouseButtons[button].state && mouseButtons[button].changeFrame == frameNumber;
}

bool GetKey(SDL_Scancode key) {
	return keys[key].state;
}

bool GetKeyPressed(SDL_Scancode key) {
	return keys[key].state && keys[key].changeFrame == frameNumber;
}

bool GetKeyReleased(SDL_Scancode key) {
	return !keys[key].state && keys[key].changeFrame == frameNumber;
}

void DrawText(SDL_Texture* textTexture, SDL_Rect* textSrcrect, SDL_Rect textDst) {
	SDL_RenderCopy(renderer, textTexture, textSrcrect, &textDst);
}

void ClearText(SDL_Surface* textSurface, SDL_Texture* textTexture) {
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
}

float deltaTime = 0.f;
int frameNumber = 0;

std::random_device randomDevice;
std::mt19937 randomEngine(randomDevice());