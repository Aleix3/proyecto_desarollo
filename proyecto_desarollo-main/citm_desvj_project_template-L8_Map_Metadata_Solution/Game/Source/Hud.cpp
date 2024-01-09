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
#include "Hud.h"
#include "Defs.h"
#include "Log.h"

Hud::Hud() : Module()
{
	
}

// Destructor
Hud::~Hud()
{}

// Called before render is available
bool Hud::Awake(pugi::xml_node config)
{
	


	return ret;
}

// Called before the first frame
bool Hud::Start()
{
	
	
	
	

	return true;
}

// Called each loop iteration
bool Hud::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Hud::Update(float dt)
{
	//L02 DONE 3: Make the camera movement independent of framerate



	uint windowWidth, windowHeight;
	app->win->GetWindowSize(windowWidth, windowHeight);
	
	
	
	return true;
}



// Called each loop iteration
bool Hud::PostUpdate()
{
	uint windowWidth, windowHeight;
	app->win->GetWindowSize(windowWidth, windowHeight);

	app->render->DrawText("Hola", windowWidth, windowHeight, 200, 200, 2);

	

	return ret;
}

// Called before quitting
bool Hud::CleanUp()
{
	LOG("Freeing Prescene");

	return true;
}



