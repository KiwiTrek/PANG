#include <stdio.h>
#include <iostream>
#include "Game.h"
#include "Globals.h"

#include "SDL/include/SDL.h"
#include "SDL_Image/include/SDL_image.h"
#include "SDL_Mixer/include/SDL_mixer.h"

#pragma comment(lib,"SDL/libx86/SDL2.lib")
#pragma comment(lib,"SDL/libx86/SDL2main.lib")
#pragma comment(lib,"SDL_Image/libx86/SDL2_image.lib")
#pragma comment(lib,"SDL_Mixer/libx86/SDL2_mixer.lib")


enum class MAIN_STATES
{
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

int main() {
	//ReportMemoryLeaks();

	int main_return = EXIT_FAILURE;
	MAIN_STATES state = MAIN_STATES::MAIN_CREATION;
	Game* game = nullptr;

	while (state != MAIN_STATES::MAIN_EXIT) {
		switch (state) {
		case MAIN_STATES::MAIN_CREATION: {
			//LOG("Game Creation --------------\n");
			game = new Game();
			state = MAIN_STATES::MAIN_START;
		}	
		break;

		case MAIN_STATES::MAIN_START: {
			//LOG("Game Start --------------\n");
			if (game->Init() == false) {
				//LOG("Game Init exits with error -----\n");
				state = MAIN_STATES::MAIN_EXIT;
			}
			else { state = MAIN_STATES::MAIN_UPDATE; }
		}	
		break;

		case MAIN_STATES::MAIN_UPDATE: {
			UPDATE_STATUS status = game->Update();

			if (status == UPDATE_STATUS::UPDATE_ERROR) {
				//LOG("Game Update exits with error -----\n");
				state = MAIN_STATES::MAIN_EXIT;
			}
			else if (status == UPDATE_STATUS::UPDATE_STOP) { state = MAIN_STATES::MAIN_FINISH; }
		}
		break;

		// TODO 1: Implement case MAIN_FINISH
		// Remember to quit the game with EXIT_SUCCESS if everything went well :)
		}
	}

	//LOG("\nBye :)\n");

	// TODO 6: Remove ALL memory leaks

	return main_return;
}