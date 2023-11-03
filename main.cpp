#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <windows.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_sdl.h"
#include "ImGui/imgui_impl_sdl.h"

#include "src/dataStructuresAndMethods.h"
#include "src/debugDrawer.h"
#include "src/enemyBase.h"
#include "src/enemyFighter.h"
#include "src/enemyManager.h"
#include "src/gameEngine.h"
#include "src/ImGuiManager.h"
#include "src/playerCharacter.h"
#include "src/projectileManager.h"
#include "src/quadTree.h"
#include "src/sprite.h"
#include "src/spriteSheet.h"
#include "src/steeringBehaviour.h"
#include "src/timerManager.h"
#include "src/textSprite.h"
#include "src/vector2.h"

int main(int argc, char* argv[]) {
	HWND windowHandle = GetConsoleWindow();
	ShowWindow(windowHandle, SW_HIDE);

	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	IMG_Init(1);

	window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, 0);	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	enemyManager = std::make_shared<EnemyManager>();
	debugDrawer = std::make_shared<DebugDrawer>();
	imGuiHandler = std::make_shared<ImGuiHandler>();
	projectileManager = std::make_shared<ProjectileManager>();
	playerCharacter = std::make_shared<PlayerCharacter>("res/sprites/CoralineDadKing.png",
		0.f, Vector2<float>(windowWidth * 0.5f, windowHeight * 0.5f));

	timerManager = std::make_shared<TimerManager>();

	QuadTreeNode quadTreeNode;
	quadTreeNode.rectangle = AABB::makeFromPositionSize(
		Vector2(windowWidth * 0.5f, windowHeight * 0.5f), windowHeight, windowWidth);
	enemyQuadTree = new QuadTreeTemp<EnemyBase*>(quadTreeNode, 50);
	projectileQuadTree = new QuadTreeTemp<Projectile*>(quadTreeNode, 50);

	separationBehaviour = std::make_shared<SeparationBehaviour>();

	//Init here
	enemyManager->Init();
	imGuiHandler->Init();
	playerCharacter->Init();
	projectileManager->Init();

	TextSprite* fpsText = new TextSprite();
	fpsText->Init("res/roboto.ttf", 24, std::to_string(0).c_str(), { 255, 255, 255,255});

	Uint64 previous_ticks = SDL_GetPerformanceCounter();
	bool running = true;
	while (running) {
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();

		frameNumber++;
		const Uint64 ticks = SDL_GetPerformanceCounter();
		const Uint64 delta_ticks = ticks - previous_ticks;
		previous_ticks = ticks;
		deltaTime = (float)delta_ticks / (float)SDL_GetPerformanceFrequency();


		SDL_Event eventType;
		while (SDL_PollEvent(&eventType)) {
			ImGui_ImplSDL2_ProcessEvent(&eventType);
			switch (eventType.type) {
				case SDL_QUIT: {
					running = false;
					break;
				}
				case SDL_KEYDOWN: {
					const int scanCode = eventType.key.keysym.scancode;
					if (eventType.key.repeat) {
						break;
					}
					if (scanCode == SDL_SCANCODE_ESCAPE) {
						running = false;
					}
			
					keys[scanCode].changeFrame = frameNumber;
					keys[scanCode].state = true;
					break;
				}
				case SDL_KEYUP: {
					const int scanCode = eventType.key.keysym.scancode;
					keys[scanCode].changeFrame = frameNumber;
					keys[scanCode].state = false;
					break;
				}
				case SDL_MOUSEBUTTONDOWN: {
					if (eventType.key.repeat) {
						break;
					}
					mouseButtons[eventType.button.button].changeFrame = frameNumber;
					mouseButtons[eventType.button.button].state = true;
					break;
				}
				case SDL_MOUSEBUTTONUP: {
					mouseButtons[eventType.button.button].changeFrame = frameNumber;
					mouseButtons[eventType.button.button].state = false;
				}
			}
		}
		//Update here
		enemyManager->UpdateQuadTree();
		projectileManager->UpdateQuadTree();

		enemyManager->Update();
		projectileManager->Update();
		playerCharacter->Update();
		timerManager->Update();

		SDL_SetRenderDrawColor(renderer, 0, 125, 0, 255);
		SDL_RenderClear(renderer);

		//Render images here
		enemyManager->Render();
		playerCharacter->Render();
		projectileManager->Render();

		enemyQuadTree->ClearTemp();
		projectileQuadTree->ClearTemp();

		//Render text here
		fpsText->ChangeText(std::to_string(1 / deltaTime).c_str(), { 255, 255, 255, 255 });
		fpsText->Render();
		playerCharacter->RenderText();

		debugDrawer->DrawBoxes();
		debugDrawer->DrawCircles();
		debugDrawer->DrawLines();

		imGuiHandler->Render();

		SDL_RenderPresent(renderer);
		SDL_Delay(16);
	}
	imGuiHandler->ShutDown();
	SDL_DestroyWindow(window);
	SDL_Quit();
	std::exit(0);
	
	return 0;
}