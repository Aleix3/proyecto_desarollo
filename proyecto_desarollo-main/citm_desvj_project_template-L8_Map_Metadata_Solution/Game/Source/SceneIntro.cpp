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

SceneIntro::SceneIntro() : Module()
{
	
}

// Destructor
SceneIntro::~SceneIntro()
{}

// Called before render is available
bool SceneIntro::Awake(pugi::xml_node config)
{
	


	return ret;
}

// Called before the first frame
bool SceneIntro::Start()
{
	
	
	menu = app->tex->Load("Assets/Textures/Menu/elements2.png");
	fondo = app->tex->Load("Assets/Textures/PLATAFORMA/Texture/fondo2.jpg");
	//Music is commentapped so that you can add your own music	
	app->audio->PlayMusic("Assets/Audio/Music/Interloper.ogg");

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	app->scene->active = false;
	app->entityManager->active = false;
	app->map->active = false;
	app->physics->active = false;
	app->hud->active = false;
	app->die->active = false;

	return true;
}

// Called each loop iteration
bool SceneIntro::PreUpdate()
{
	return true;
}
bool camaralibre0 = false;

int contadormenu0 = 0;

int contadormenusettings0 = 0;
// Called each loop iteration
bool SceneIntro::Update(float dt)
{
	//L02 DONE 3: Make the camera movement independent of framerate

	

	uint windowWidth, windowHeight;
	app->win->GetWindowSize(windowWidth, windowHeight);
	
	if (nidea == true)
	{
		char text10[20];
		sprintf_s(text10, "NIDEA TEAM");
		app->render->DrawText(text10, windowWidth / 2 -195, windowHeight / 2 - 50, 370, 100, 1);
		
	}
	else
	{
		if (menuu && contadormenu0 == 0)
		{
			contadormenu0++;
			if (gcButtom != nullptr)
			{
				gcButtom->state = GuiControlState::NORMAL;
				exit->state = GuiControlState::NORMAL;
				settings->state = GuiControlState::NORMAL;
			}


			else
			{
				SDL_Rect btPos = { windowWidth / 2 - 100,windowHeight / 2 - 50, 230,30 };
				gcButtom = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "START", btPos, { 0,0,0,0 }, this);

				SDL_Rect ExitPos = { windowWidth / 2 - 100,windowHeight / 2 + 250, 230,30 };
				exit = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "EXIT", ExitPos, { 0,0,0,0 }, this);

				SDL_Rect SettingsPos = { windowWidth / 2 - 100,windowHeight / 2 + 110, 230,30 };
				settings = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "SETTINGS", SettingsPos, { 0,0,0,0 }, this);
			}





		}
		if (cargar == true)
		{

			SDL_Delay(3000);
			app->scene->active = true;
			app->entityManager->active = true;
			app->map->active = true;
			app->physics->active = true;
			app->hud->active = true;

		}

		if (!menuu)
		{


			if (gcButtom != nullptr || app->scene->GetPlayer()->level2 == true)
			{


				if (app->scene->GetPlayer()->level2 == false)
				{
					gcButtom->state = GuiControlState::DISABLED;
					exit->state = GuiControlState::DISABLED;
					settings->state = GuiControlState::DISABLED;
					gcButtom = nullptr;
					exit = nullptr;
					settings = nullptr;
				}


				contadormenu0 = 0;


				if (menuusettings == false)
				{
					cargar = true;

					app->scene->GetPlayer()->level2 = true;

				}


			}

		}


		if (gcButtom != nullptr && gcButtom->click == true || app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{


			menuu = false;


		}

		if (exit != nullptr && exit->click == true)
		{
			ret = false;
		}

		if (settings != nullptr && settings->click == true)
		{
			menuusettings = true;
			menuu = false;

		}

		if (menuusettings && contadormenusettings0 == 0)
		{
			SDL_Rect cruzpos = { windowWidth / 2 + 150,windowHeight / 2 - 150, 50,50 };
			cruz = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "X", cruzpos, { 0,0,0,2 }, this);

			SDL_Rect vSyncpos = { windowWidth / 2 + 100,windowHeight / 2 - 70, 200, 50 };
			vsync = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 1, "VSYNC", vSyncpos, { 0,0,20,20 }, this);

			SDL_Rect FullScreen = { windowWidth / 2 + 100,windowHeight / 2 + 150, 200, 50 };
			fullScreen = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 1, "FULLSCREEN", FullScreen, { 0,0,0,0 }, this);

			contadormenusettings0++;

		}
		if (menuusettings)
		{

		}
		if (!menuusettings)
		{

			if (vsync != nullptr)
			{


				vsync->state = GuiControlState::DISABLED;
				fullScreen->state = GuiControlState::DISABLED;
				cruz->state = GuiControlState::DISABLED;
				vsync = nullptr;
				fullScreen = nullptr;
				cruz = nullptr;

				contadormenusettings0 = 0;


			}

		}
		if (vsync != nullptr && vsync->click == true)
		{
			vsyncActive = true;
		}

		if (vsync != nullptr && vsync->click == false)
		{
			vsyncActive = false;
		}

		if (fullScreen != nullptr && fullScreen->click == true && fullScreenActive == false)
		{
			fullScreenActive = true;
			app->win->ToggleFullscreen();
		}

		if (fullScreen != nullptr && fullScreen->click == false && fullScreenActive == true)
		{
			fullScreenActive = false;
			app->win->ToggleFullscreen();
		}
		if (cruz != nullptr && cruz->click == true)
		{
			menuu = true;
			menuusettings = false;

		}
	}

	
	
	return true;
}

int c = 0;

// Called each loop iteration
bool SceneIntro::PostUpdate()
{

	uint windowWidth, windowHeight;
	app->win->GetWindowSize(windowWidth, windowHeight);

	if (nidea == false)
	{
		if (cargar == false)
		{

			app->render->DrawTexture(fondo, windowWidth / 2 - 650, windowHeight / 2 - 400, 0, 0, 0, 0, 0, true);

			char text1[20];
			sprintf_s(text1, "WIZARD");
			app->render->DrawText(text1, windowWidth / 2 - 170, windowHeight / 2 - 300, 370, 100, 3);

			char text3[20];
			sprintf_s(text3, "ADVENTURE");
			app->render->DrawText(text3, windowWidth / 2 - 200, windowHeight / 2 - 210, 450, 100, 3);
		}

		else
		{
			int p = 1;
			char text2[20];
			sprintf_s(text2, "LEVEL %d/2", level);
			app->render->DrawText(text2, windowWidth / 2 - 200, windowHeight / 2 - 50, 370, 100, 1);


		}
		
	}
	
	
	if (nidea == true)
	{
		c++;
		if (c == 3)
		{
			SDL_Delay(2000);
			nidea = false;
		}
		
	}
	
	
	


	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;
	

	return ret;
}

// Called before quitting
bool SceneIntro::CleanUp()
{
	LOG("Freeing Prescene");

	return true;
}

void SceneIntro::levelScreen()
{

}



