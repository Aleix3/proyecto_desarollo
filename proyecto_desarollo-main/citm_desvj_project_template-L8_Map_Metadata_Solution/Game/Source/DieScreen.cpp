#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Item.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include"SceneIntro.h"
#include "Defs.h"
#include "Log.h"
#include "Hud.h"
#include "DieScreen.h"

DieScreen::DieScreen() : Module()
{
	
}

// Destructor
DieScreen::~DieScreen()
{}

// Called before render is available
bool DieScreen::Awake(pugi::xml_node config)
{
	


	return ret;
}

// Called before the first frame
bool DieScreen::Start()
{
	
	
	
	

	return true;
}

// Called each loop iteration
bool DieScreen::PreUpdate()
{
	return true;
}

int num = 0;
// Called each loop iteration
bool DieScreen::Update(float dt)
{
	//L02 DONE 3: Make the camera movement independent of framerate
	
	if (num == 10)
	{
		SDL_Delay(3000);
		app->scene->active = true;
		app->entityManager->active = true;
		app->map->active = true;
		app->physics->active = true;
		app->hud->active = true;
		app->die->active = false;
		num = 0;
	}
	

	num++;
	return true;
}



// Called each loop iteration
bool DieScreen::PostUpdate()
{
	uint windowWidth, windowHeight;
	app->win->GetWindowSize(windowWidth, windowHeight);

	int p = 1;
	char text2[20];
	sprintf_s(text2, "YOU DIED");
	app->render->DrawText(text2, windowWidth / 2 - 200, windowHeight / 2 - 50, 370, 100, 2);
	
	

	return ret;
}

// Called before quitting
bool DieScreen::CleanUp()
{
	LOG("Freeing Prescene");

	return true;
}





