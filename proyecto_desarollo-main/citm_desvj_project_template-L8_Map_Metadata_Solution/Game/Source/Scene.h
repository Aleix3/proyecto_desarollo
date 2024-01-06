#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemySamurai.h"
#include "EnemyVulture.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "GuiCheckBox.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	Player* GetPlayer();

	EnemySamurai* GetEnemySamurai();

	bool OnGuiMouseClickEvent(GuiControl* control);

	bool visible = true;

	bool menuu = false;

	bool menuusettings = false;

	bool vsyncActive = false;

	iPoint playerMap;

	EnemySamurai* enemySamurai;

	EnemyVulture* enemyVulture;

	SDL_Texture* mouseTileTex = nullptr;
private:
	SDL_Texture* img;
	SDL_Texture* menu;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	

	//L03: DONE 3b: Declare a Player attribute
	Player* player;
	
	GuiControlButton* gcButtom;
	GuiControlButton* exit;
	GuiControlButton* settings;
	GuiCheckBox* vsync;

	bool ret = true;

};

#endif // __SCENE_H__