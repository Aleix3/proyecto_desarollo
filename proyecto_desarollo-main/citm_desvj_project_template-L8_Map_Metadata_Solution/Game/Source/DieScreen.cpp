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
#include "ability.h"

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
int num2 = 0;
int num3 = 0;
// Called each loop iteration
bool DieScreen::Update(float dt)
{
	//L02 DONE 3: Make the camera movement independent of framerate
	
	//pantalla muerte
	if (app->scene->GetPlayer()->diedie == true)
	{
		if (num == 10)
		{
			SDL_Delay(3000);
			app->scene->active = true;
			app->entityManager->active = true;
			app->map->active = true;
			app->physics->active = true;
			app->hud->active = true;
			app->die->active = false;
			app->ability->active = true;
			num = 0;
			app->scene->GetPlayer()->diedie = false;
		}
		num++;
	}
	if (app->sceneintro->credito == true)		//pantalla creditos
	{
		if (num2 == 10)
		{
			
			SDL_Delay(3000);
			uint windowWidth, windowHeight;
			app->win->GetWindowSize(windowWidth, windowHeight);

			SDL_Rect btPos = { windowWidth / 2 - 100,windowHeight / 2 - 70, 230,30 };
			app->sceneintro->gcButtom = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "START", btPos, { 0,0,0,0 }, this);

			SDL_Rect ExitPos = { windowWidth / 2 - 100,windowHeight / 2 + 300, 230,30 };
			app->sceneintro->exit = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "EXIT", ExitPos, { 0,0,0,0 }, this);

			SDL_Rect SettingsPos = { windowWidth / 2 - 100,windowHeight / 2 + 60, 230,30 };
			app->sceneintro->settings = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "SETTINGS", SettingsPos, { 0,0,0,0 }, this);

			SDL_Rect CreditsPos = { windowWidth / 2 - 100,windowHeight / 2 + 180, 230,30 };
			app->sceneintro->credits = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "CREDITS", CreditsPos, { 0,0,0,0 }, this);
			
			app->sceneintro->active = true;
			app->sceneintro->credito = false;
			app->die->active = false;
			
			num2 = 0;
		}

		num2++;
	}


	if (app->scene->GetPlayer()->win == true)		//pantalla victoria
	{
		if (num3 == 10)
		{
			SDL_Delay(3000);
			app->scene->active = false;
			app->entityManager->active = false;
			app->map->active = false;
			app->physics->active = false;
			app->hud->active = false;
			app->sceneintro->active = true;
			app->sceneintro->menuu = true;
			app->physics->active = false;
			app->sceneintro->cargar = false;
			app->ability->active = false;
			app->scene->GetPlayer()->level2 = false;
			app->sceneintro->level = 1;
			app->scene->GetPlayer()->win = false;
			num3 = 0;
		}
		num3++;
	}
	
	

	
	return true;
}



// Called each loop iteration
bool DieScreen::PostUpdate()
{
	uint windowWidth, windowHeight;
	app->win->GetWindowSize(windowWidth, windowHeight);

	if (app->scene->GetPlayer()->diedie == true)
	{
		char text2[20];
		sprintf_s(text2, "YOU DIED");
		app->render->DrawText(text2, windowWidth / 2 - 200, windowHeight / 2 - 50, 370, 100, 2);
	}
	
	if (app->sceneintro->credito == true)
	{
		char text3[30];
		char text4[30];
		char text5[30];
		sprintf_s(text3, "DEVS:");
		sprintf_s(text4, "Eduard Garcia");
		sprintf_s(text5, "Aleix Botella");
		app->render->DrawText(text3, windowWidth / 2 - 200, windowHeight / 2 - 250, 370, 100, 1);
		app->render->DrawText(text4, windowWidth / 2 - 200, windowHeight / 2 - 50, 370, 100, 1);
		app->render->DrawText(text5, windowWidth / 2 - 200, windowHeight / 2 + 150, 370, 100, 1);
	}
	if (app->scene->GetPlayer()->win == true)
	{
		char text6[30];
		sprintf_s(text6, "YOU WIN:");
		app->render->DrawText(text6, windowWidth / 2 - 200, windowHeight / 2 - 50, 370, 100, 3);
	}

	return ret;
}

// Called before quitting
bool DieScreen::CleanUp()
{
	LOG("Freeing Prescene");

	return true;
}





