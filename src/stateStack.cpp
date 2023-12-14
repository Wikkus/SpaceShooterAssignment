#include "stateStack.h"

#include "dataStructuresAndMethods.h"
#include "enemyManager.h"
#include "gameEngine.h"
#include "playerCharacter.h"
#include "projectileManager.h"
#include "timerManager.h"


Button::Button(const char* spritePath, int height, int width, Vector2<float> position) {
	_height = height;
	_width = width;

	_position = position;
	
	_sprite = std::make_shared<Sprite>();
	_sprite->h = height;
	_sprite->w = width;
	_sprite->Load(spritePath);

	_boxCollider = AABB::makeFromPositionSize(position, height, width);
	
}

bool Button::ClickedOn() {
	Vector2<float> cursorPosition = GetCursorPosition();
	Circle circle;
	circle.position = cursorPosition;
	circle.radius = 10.f;
	if (GetMouseButtonPressed(SDL_BUTTON_LEFT)) {
		if (AABBCircleIntersect(_boxCollider, circle)) {
			return true;
		}
	}
	return false;
}
void Button::Render() {
	_sprite->RenderCentered(_position);
}

GameStateHandler::GameStateHandler() {
	_buttons[ButtonType::MainMenu] = std::make_shared<Button>("res/sprites/MainMenuButton.png", 64, 128, Vector2<float>(windowWidth * 0.5f, windowHeight * 0.6f));
	_buttons[ButtonType::Play] = std::make_shared<Button>("res/sprites/PlayButton.png", 64, 128, Vector2<float>(windowWidth * 0.5f, windowHeight * 0.3f));
	_buttons[ButtonType::Quit] = std::make_shared<Button>("res/sprites/QuitButton.png", 64, 128, Vector2<float>(windowWidth * 0.5f, windowHeight * 0.8f));
	_buttons[ButtonType::Restart] = std::make_shared<Button>("res/sprites/RestartButton.png", 64, 128, Vector2<float>(windowWidth * 0.5f, windowHeight * 0.4f));
	_buttons[ButtonType::Resume] = std::make_shared<Button>("res/sprites/ResumeButton.png", 64, 128, Vector2<float>(windowWidth * 0.5f, windowHeight * 0.2f));
}

GameStateHandler::~GameStateHandler() {}

void GameStateHandler::AddState(std::shared_ptr<State> state) {
	_states.emplace_back(state);
}

void GameStateHandler::BackToFirstState() {
	while (_states.size() > 1) {
		_states.pop_back();
	}
}

void GameStateHandler::ReplaceCurrentState(std::shared_ptr<State> state) {
	_states.pop_back();
	_states.emplace_back(state);
}

void GameStateHandler::RemoveCurrentState() {
	_states.pop_back();
}

void GameStateHandler::UpdateState() {
	_states.back()->Update();
}

void GameStateHandler::RenderState() {
	_states.back()->Render();
}

void GameStateHandler::RenderStateText() {
	_states.back()->RenderText();
}

GameState::GameState() {
	playerCharacter->Respawn();
}

void GameState::Update() {
	enemyManager->UpdateQuadTree();
	projectileManager->UpdateQuadTree();

	enemyManager->Update();
	projectileManager->Update();
	playerCharacter->Update();
	timerManager->Update();
}

void GameState::Render() {
	enemyManager->Render();
	playerCharacter->Render();
	projectileManager->Render();
}

void GameState::RenderText() {
	playerCharacter->RenderText();
}

GameOverState::GameOverState() {}

void GameOverState::Update() {
	if (_buttons[ButtonType::MainMenu]->ClickedOn()) {
		gameStateHandler->BackToFirstState();

	} else if (_buttons[ButtonType::Quit]->ClickedOn()) {
		runningGame = false;

	} else if (_buttons[ButtonType::Restart]->ClickedOn()) {
		gameStateHandler->ReplaceCurrentState(std::make_shared<GameState>());
	}
}

void GameOverState::Render() {
	_buttons[ButtonType::MainMenu]->Render();
	_buttons[ButtonType::Quit]->Render();
	_buttons[ButtonType::Restart]->Render();
}

void GameOverState::RenderText() {}

MenuState::MenuState() {}

void MenuState::Update() {
	if (_buttons[ButtonType::Play]->ClickedOn()) {
		gameStateHandler->AddState(std::make_shared<GameState>());

	} else if (_buttons[ButtonType::Quit]->ClickedOn()) {
		runningGame = false;
	}
}

void MenuState::Render() {
	_buttons[ButtonType::Play]->Render();
	_buttons[ButtonType::Quit]->Render();
}

void MenuState::RenderText() {}

PauseState::PauseState() {}

void PauseState::Update() {
	if (_buttons[ButtonType::MainMenu]->ClickedOn()) {
		gameStateHandler->BackToFirstState();
	
	} else if (_buttons[ButtonType::Quit]->ClickedOn()) {
		runningGame = false;
	
	} else if (_buttons[ButtonType::Restart]->ClickedOn()) {
		gameStateHandler->BackToFirstState();
		gameStateHandler->ReplaceCurrentState(std::make_shared<GameState>());

	} else if (_buttons[ButtonType::Resume]->ClickedOn() || GetKeyPressed(SDL_SCANCODE_ESCAPE)) {
		gameStateHandler->RemoveCurrentState();
	}
	
}

void PauseState::Render() {
	enemyManager->Render();
	playerCharacter->Render();
	projectileManager->Render();

	_buttons[ButtonType::MainMenu]->Render();
	_buttons[ButtonType::Restart]->Render();
	_buttons[ButtonType::Resume]->Render();
	_buttons[ButtonType::Quit]->Render();
}

void PauseState::RenderText() {}

