#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"
#include "Physics.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	bool IsOnGround();

	void EndContact(PhysBody* physA, PhysBody* physB);

	// L07 TODO 6: Define OnCollision function for the player. 
	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	Animation* currentAnimation = nullptr;
	Animation* currentJumpAnim = nullptr;

	// A set of animations
	Animation idleAnim;


	//L02: DONE 2: Declare player parameters
	bool isOnGround = false;
	float speed = 0.2f;
	SDL_Texture* texture = NULL;
	pugi::xml_node config;
	uint texW, texH;


	//Audio fx
	int pickCoinFxId;

	// L07 TODO 5: Add physics to the player - declare a Physics body
	PhysBody* pbody;

	float jumpSpeed = 3.0f;
	float dashSpeed = 100.0f;

	
};

#endif // __PLAYER_H__