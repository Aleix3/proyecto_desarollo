#ifndef __SCENEINTRO_H__
#define __SCENEINTRO_H__

#include "Module.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemySamurai.h"
#include "EnemyVulture.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "GuiCheckBox.h"

struct SDL_Texture;

class SceneIntro : public Module
{
public:

	SceneIntro();

	// Destructor
	virtual ~SceneIntro();

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

	
	bool menuu = true;
	
	
	void levelScreen();

	bool cargar = false;

	bool nidea = true;

	int level = 1;
	
private:

	bool visible = true;

	

	bool menuusettings = false;

	bool vsyncActive = false;

	bool fullScreenActive = false;

	SDL_Texture* img;
	SDL_Texture* menu;
	SDL_Texture* fondo;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	

	//L03: DONE 3b: Declare a Player attribute
	Player* player;
	
	GuiControlButton* gcButtom;
	GuiControlButton* exit;
	GuiControlButton* settings;
	GuiControlButton* credits;
	GuiCheckBox* vsync;
	GuiCheckBox* fullScreen;
	GuiControlButton* cruz;
	bool ret = true;
	

};

#endif // __SCENE_H__