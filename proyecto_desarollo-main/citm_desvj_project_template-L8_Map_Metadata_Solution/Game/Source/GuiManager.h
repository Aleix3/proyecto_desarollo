#ifndef __GUIMANAGER_H__
#define __GUIMANAGER_H__

#include "Module.h"
#include "GuiControl.h"

#include "List.h"

class GuiManager : public Module
{
public:

	// Constructor
	GuiManager();

	// Destructor
	virtual ~GuiManager();

	// Called before the first frame
	 bool Start();

	 // Called each loop iteration
	 bool Update(float dt);

	 bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Additional methods
	GuiControl* CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, SDL_Rect bounds2, Module* observer, SDL_Rect sliderBounds = { 0,0,0,0 });

	bool GuiManager::DestroyGuiControl(GuiControl* control);

public:

	List<GuiControl*> guiControlsList;
	SDL_Texture* texture;

};

#endif // __GUIMANAGER_H__
