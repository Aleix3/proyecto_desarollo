#ifndef __GUICONTROLSCORE_H__
#define __GUICONTROLSCORE_H__

#include "Defs.h"
#include "Log.h"
#include "SString.h"

#include "GuiManager.h"
#include "GuiControl.h"

class GuiControlScore : public GuiControl
{
public:
	GuiControlScore(uint32 id, SDL_Rect bounds);
	~GuiControlScore();

	bool Update(float dt);

private:
	int lastScore;
	SDL_Surface* surface;
	SDL_Texture* texture;
};

#endif // __GUICONTROLSCORE_H__
