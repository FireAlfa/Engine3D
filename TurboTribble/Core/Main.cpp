#include <stdlib.h>
#include "Application.h"
#include "Globals.h"

#include "SDL/include/SDL.h"

#pragma comment( lib,"Core/External/SDL/libx86/SDL2.lib" )
#pragma comment( lib,"Core/External/SDL/libx86/SDL2main.lib" )

enum class MainStates
{
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

Application* app = NULL;

int main(int argc, char ** argv)
{
	LOG("Starting game '%s'...", TITLE);

	int main_return = EXIT_FAILURE;
	MainStates state = MainStates::MAIN_CREATION;
	

	while (state != MainStates::MAIN_EXIT)
	{
		switch (state)
		{
		case MainStates::MAIN_CREATION:

			LOG("-------------- Application Creation --------------");
			app = new Application();
			state = MainStates::MAIN_START;
			break;

		case MainStates::MAIN_START:

			LOG("-------------- Application Init --------------");
			if (app->Init() == false)
			{
				LOG("Application Init exits with ERROR");
				state = MainStates::MAIN_EXIT;
			}
			else
			{
				state = MainStates::MAIN_UPDATE;
				LOG("-------------- Application Update --------------");
			}

			break;

		case MainStates::MAIN_UPDATE:
		{
			UpdateStatus updateReturn = app->Update();

			if (updateReturn == UpdateStatus::UPDATE_ERROR)
			{
				LOG("Application Update exits with ERROR");
				state = MainStates::MAIN_EXIT;
			}

			if (updateReturn == UpdateStatus::UPDATE_STOP)
				state = MainStates::MAIN_FINISH;
		}
			break;

		case MainStates::MAIN_FINISH:

			LOG("-------------- Application CleanUp --------------");
			if (app->CleanUp() == false)
			{
				LOG("Application CleanUp exits with ERROR");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MainStates::MAIN_EXIT;

			break;

		}
	}

	delete app;

	return main_return;
}