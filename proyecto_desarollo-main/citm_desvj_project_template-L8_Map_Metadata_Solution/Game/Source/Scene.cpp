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

	//Get the map name from the config file and assigns the value in the module
	app->map->name = config.child("map").attribute("name").as_string();
	app->map->path = config.child("map").attribute("path").as_string();

	// iterate all items in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}


	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	// NOTE: We have to avoid the use of paths in the code, we will move it later to a config file
	img = app->tex->Load("Assets/Textures/test.png");
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
	gcButtom = (GuiControlButton*) app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "MyButton", btPos, this);

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}
bool camaralibre = false;

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
			app->render->camera.y += (int)ceil(camSpeed * dt);

		if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			app->render->camera.y -= (int)ceil(camSpeed * dt);

		if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			app->render->camera.x += (int)ceil(camSpeed * dt);

		if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			app->render->camera.x -= (int)ceil(camSpeed * dt);
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
	//	app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
	//}

	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

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
bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Press Gui Control: %d", control->id);

	return true;
}
