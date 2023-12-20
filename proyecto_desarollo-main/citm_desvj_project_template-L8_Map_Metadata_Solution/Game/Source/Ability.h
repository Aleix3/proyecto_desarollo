#ifndef __ABILITY_H__
#define __ABILITY_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"
#include "Module.h"

struct SDL_Texture;

class Ability : public Module
{
public:

	Ability();
	virtual ~Ability();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

public:

	bool isPicked = false;
	PhysBody* dispar;

	bool arriba = false;
	bool left = false;

private:

	SDL_Texture* texture;
	uint texW, texH;

	Animation* currentAnimation = nullptr;
	Animation fireballAnim;

	

	iPoint position;
	bool renderable = true;
};

#endif // __ABILITY_H__