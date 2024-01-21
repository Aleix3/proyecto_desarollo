#ifndef __DIESCREEN_H__
#define __DIESCREEN_H__

#include "Module.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemySamurai.h"
#include "EnemyVulture.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "GuiCheckBox.h"
#

struct SDL_Texture;

class DieScreen : public Module
{
public:

	DieScreen();

	// Destructor
	virtual ~DieScreen();

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

	
private:
	bool ret = true;
	
	

};

#endif // __SCENE_H__