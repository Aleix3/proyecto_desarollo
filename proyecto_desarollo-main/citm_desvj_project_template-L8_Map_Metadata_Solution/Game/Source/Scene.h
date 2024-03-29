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
#include "Boss.h"
#include "item.h"
#include "Heart.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();
	pugi::xml_node config2;
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

	EnemyVulture* GetEnemyVulture();

	Boss* GetBoss();

	Summon* GetSummon();

	Item* GetItem();


	bool visible = true;

	bool menuu = false;

	bool menuusettings = false;

	bool vsyncActive = false;

	bool fullScreenActive = false;

	iPoint playerMap;

	EnemySamurai* enemySamurai;

	EnemyVulture* enemyVulture;

	Player* player;

	Boss* boss;

	Summon* summon;

	SDL_Texture* mouseTileTex = nullptr;

private:
	Item* item;
	Heart* heart;
	SDL_Texture* img;
	SDL_Texture* menu;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	

	//L03: DONE 3b: Declare a Player attribute

	
	GuiControlButton* gcButtom;
	GuiControlButton* exit;
	GuiControlButton* settings;
	GuiControlButton* title;
	GuiCheckBox* vsync;
	GuiCheckBox* fullScreen;
	GuiControlButton* cruz;
	bool ret = true;

};

#endif // __SCENE_H__