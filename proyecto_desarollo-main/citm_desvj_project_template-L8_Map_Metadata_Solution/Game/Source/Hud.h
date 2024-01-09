#ifndef __HUD_H__
#define __HUD_H__

#include "Module.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemySamurai.h"
#include "EnemyVulture.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "GuiCheckBox.h"

struct SDL_Texture;

class Hud : public Module
{
public:

	Hud();

	// Destructor
	virtual ~Hud();

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

	SString text;


	bool ret = true;

};

#endif // __SCENE_H__