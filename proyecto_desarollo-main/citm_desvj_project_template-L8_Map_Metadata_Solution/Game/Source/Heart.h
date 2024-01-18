#ifndef __HEART_H__
#define __HEART_H__

#include "Enemy.h"

struct SDL_Texture;

class Heart : public Entity
{
public:

	Heart();

	virtual ~Heart();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);



public:

	bool isPicked = false;

private:

	SDL_Texture* texture;
	const char* texturePath;
	uint texW, texH;
	PhysBody* pbody;

};

#endif // __PLAYER_H__