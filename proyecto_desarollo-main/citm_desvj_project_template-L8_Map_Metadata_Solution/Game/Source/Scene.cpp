#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Item.h"
#include "Heart.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include"SceneIntro.h"
#include "Hud.h"
#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node config)
{
	LOG("Loading Scene");
	bool ret = true;

	player = (Player*) app->entityManager->CreateEntity(EntityType::PLAYER);
	//Assigns the XML node to a member in player
	player->config = config.child("player");  


	for (pugi::xml_node enemyNode = config.child("EnemySamurai"); enemyNode; enemyNode = enemyNode.next_sibling("EnemySamurai"))
	{
		enemySamurai = (EnemySamurai*)app->entityManager->CreateEntity(EntityType::ENEMYSAMURAI);
		enemySamurai->parameters = enemyNode;
	}
	
	for (pugi::xml_node enemyNode = config.child("EnemyVulture"); enemyNode; enemyNode = enemyNode.next_sibling("EnemyVulture"))
	{
		enemyVulture = (EnemyVulture*)app->entityManager->CreateEntity(EntityType::ENEMYVULTURE);
		enemyVulture->parameters = enemyNode;
	}

	for (pugi::xml_node enemyNode = config.child("Boss"); enemyNode; enemyNode = enemyNode.next_sibling("Boss"))
	{
		boss = (Boss*)app->entityManager->CreateEntity(EntityType::BOSS);
		boss->parameters = enemyNode;
	}

	//Get the map name from the config file and assigns the value in the module
	app->map->name = config.child("map").attribute("name").as_string();
	app->map->path = config.child("map").attribute("path").as_string();

	// iterate all items in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}
	for (pugi::xml_node HeartNode = config.child("heart"); HeartNode; HeartNode = HeartNode.next_sibling("heart"))
	{
		heart = (Heart*)app->entityManager->CreateEntity(EntityType::HEART);
		heart->parameters = HeartNode;
	}


	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	
	img = app->tex->Load("Assets/Textures/test.png");
	menu = app->tex->Load("Assets/Textures/Menu/elements2.png");
	//Music is commented so that you can add your own music
	app->audio->PlayMusic("Assets/Audio/Music/Interloper.ogg");

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	app->tex->GetSize(img, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	mouseTileTex = app->tex->Load("Assets/Maps/tileSelection.png");

	SDL_Rect btPos = { windowW / 2 - 60,80, 120,20 };
	/*gcButtom = (GuiControlButton*) app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "MyButton", btPos, this);*/
	

	return true;

	
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	app->sceneintro->active = false;
	return true;
}
bool camaralibre = false;

int contadormenu = 0;

int contadormenusettings = 0;
// Called each loop iteration
bool Scene::Update(float dt)
{
	//L02 DONE 3: Make the camera movement independent of framerate
	float camSpeed = 1;

	

	if (app->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN && app->physics->debug == true)
	{
		camaralibre = !camaralibre; // Esto alternará el valor de camaralibre cada vez que se presione la tecla C en modo debug
	}
	if (app->scene->GetPlayer()->die)
	{
		app->scene->GetPlayer()->die == false;
	}
	if (camaralibre == true)
	{
		if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			app->render->targetCameraY += (int)ceil(camSpeed * dt);

		if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			app->render->targetCameraY -= (int)ceil(camSpeed * dt);

		if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			app->render->targetCameraX += (int)ceil(camSpeed * dt);

		if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			app->render->targetCameraX -= (int)ceil(camSpeed * dt);
	}
	else
	{
		/*app->render->camera.x = 2 * (-player->position.x) + 600;
		app->render->camera.y = 2 * (-player->position.y) + 400;*/

		app->render->targetCameraX = 2 * (-player->position.x) + 600;
		app->render->targetCameraY = 2 * (-player->position.y) + 400;


	}

	
	
	
	// L14: TODO 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();

	uint windowWidth, windowHeight;
	app->win->GetWindowSize(windowWidth, windowHeight);

	if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
	{
		app->entityManager->active = !app->entityManager->active;

		menuu = !menuu;
	}
	

	if (menuu && contadormenu == 0)
	{
		contadormenu++;
		if (gcButtom != nullptr)
		{
			gcButtom->state = GuiControlState::NORMAL;
			exit->state = GuiControlState::NORMAL;
			settings->state = GuiControlState::NORMAL;
		}
		
		
		else
		{
			SDL_Rect btPos = { windowWidth / 2 - 100,windowHeight / 2 - 300, 230,30 };
			gcButtom = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "RESUME", btPos, { 0,0,0,0 }, this);

			SDL_Rect ExitPos = { windowWidth / 2 - 100,windowHeight / 2 + 130, 230,30 };
			exit = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "EXIT", ExitPos, { 0,0,0,0 }, this);

			SDL_Rect SettingsPos = { windowWidth / 2 - 100,windowHeight / 2 - 160, 230,30 };
			settings = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "SETTINGS", SettingsPos, { 0,0,0,0 }, this);

			SDL_Rect TitlePos = { windowWidth / 2 - 100,windowHeight / 2 - 20, 230,50 };
			title = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "TITLE SCREEN", TitlePos, { 0,0,0,0 }, this);
		}
		
		
		

		
	}
	if (menuu)
	{
		app->entityManager->active = false;
		app->physics->active = false;
	}

	if (!menuu)
	{
		
		if (gcButtom != nullptr)
		{
			app->entityManager->active = true;
			app->physics->active = true;
			gcButtom->state = GuiControlState::DISABLED;
			exit->state = GuiControlState::DISABLED;
			settings->state = GuiControlState::DISABLED;
			title->state = GuiControlState::DISABLED;;
			gcButtom = nullptr;
			exit = nullptr;
			settings = nullptr;
			title = nullptr;
			
			contadormenu = 0;
		}
		
	}
	if(gcButtom != nullptr && gcButtom->click == true)
	{
		menuu = false;
		
	}

	if (exit != nullptr && exit->click == true)
	{
		ret = false;
	}

	if (settings != nullptr && settings->click == true)
	{
		menuu = false;
		menuusettings = true;
	}
	if (title != nullptr && title->click == true)
	{
		
		app->scene->active = false;
		app->entityManager->active = false;
		app->map->active = false;
		app->physics->active = false;
		app->hud->active = false;
		app->sceneintro->active = true;
		app->sceneintro->menuu = true;
		app->entityManager->active = true;
		app->physics->active = true;
		gcButtom->state = GuiControlState::DISABLED;
		exit->state = GuiControlState::DISABLED;
		settings->state = GuiControlState::DISABLED;
		title->state = GuiControlState::DISABLED;;
		gcButtom = nullptr;
		exit = nullptr;
		settings = nullptr;
		title = nullptr;
		menuu = false;

		contadormenu = 0;
		
		
	}

	if (menuusettings && contadormenusettings == 0)
	{
		SDL_Rect cruzpos = { windowWidth / 2 + 150,windowHeight / 2 - 300, 50,50 };
		cruz = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "X", cruzpos, { 0,0,0,2 }, this);
		
		SDL_Rect vSyncpos = { windowWidth / 2 + 100,windowHeight / 2 - 220, 200, 50 };
		vsync = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 1, "VSYNC", vSyncpos, { 0,0,20,20 }, this);

		SDL_Rect FullScreen = { windowWidth / 2 + 100,windowHeight / 2 , 200, 50 };
		fullScreen = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 1, "FULLSCREEN", FullScreen, { 0,0,0,0 }, this);

		contadormenusettings++;
		
	}
	if (menuusettings)
	{
		app->entityManager->active = false;
		app->physics->active = false;
	}
	if (!menuusettings)
	{
		if (vsync != nullptr)
		{
			menuu = true;

			vsync->state = GuiControlState::DISABLED;
			fullScreen->state = GuiControlState::DISABLED;
			cruz->state = GuiControlState::DISABLED;
			vsync = nullptr;
			fullScreen = nullptr;
			cruz = nullptr;
			
			contadormenusettings = 0;
			
			
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
		menuusettings = false;
		
	}
	
	return true;
}



// Called each loop iteration
bool Scene::PostUpdate()
{

	// Get the mouse position and obtain the map coordinate
	playerMap = app->map->WorldToMap(player->position.x, player->position.y);

	iPoint mousePos;
	app->input->GetMousePosition(mousePos.x, mousePos.y);
	iPoint mouseTile = app->map->WorldToMap((mousePos.x - app->render->camera.x) / 2,
											(mousePos.y - app->render->camera.y) / 2);

	// Render a texture where the mouse is over to highlight the tile, use the texture 'mouseTileTex'

	/*iPoint highlightedTileWorld = app->map->MapToWorld(mouseTile.x , mouseTile.y);
	app->render->DrawTexture(mouseTileTex, highlightedTileWorld.x, highlightedTileWorld.y);*/

	//iPoint origin = app->map->WorldToMap(enemy->position.x, enemy->position.y);

	////If mouse button is pressed modify player position
	///*if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
	//	player->godmode = true;
	//	player->position = iPoint(highlightedTileWorld.x, highlightedTileWorld.y);
	//}*/

	//if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) //Aqui meter la distancia del enemy al player
	//{
	//	app->map->pathfinding->CreatePath(origin, playerMap);

	//}
	//
	//// L13: Get the latest calculated path and draw
	//const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
	//for (uint i = 0; i < path->Count(); ++i)
	//{
	//	iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		
	//}
	uint windowWidth, windowHeight;
	app->win->GetWindowSize(windowWidth, windowHeight);
	

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	if (menuu || menuusettings)
		app->render->DrawTexture(menu, windowWidth/2 - 250 , windowHeight/2 -400 ,0,0,0,0,0,true);
	

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

Player* Scene::GetPlayer()
{
	return player;
}

EnemySamurai* Scene::GetEnemySamurai()
{
	return enemySamurai;
}

EnemyVulture* Scene::GetEnemyVulture()
{
	return enemyVulture;
}

Boss* Scene::GetBoss()
{
	return boss;
}
Item* Scene::GetItem()
{
	return item;
}
bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Press Gui Control: %d", control->id);

	return true;
}
