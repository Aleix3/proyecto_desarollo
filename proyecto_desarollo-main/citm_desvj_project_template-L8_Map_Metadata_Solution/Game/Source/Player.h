#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"
#include "Physics.h"


#include <map>


struct SDL_Texture;

enum class PlayerState {
	IDLE,
	JUMPING,
	DYING
	// Añadir mas estados para el personaje
};

class Player : public Entity
{
public:

	PlayerState currentState;
	std::map<PlayerState, Animation> animations;

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	// L07 TODO 6: Define OnCollision function for the player. 
	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	Animation* currentAnimation = nullptr;
	Animation* currentJumpAnim = nullptr;

	// A set of animations
	Animation runAnim;
	Animation walkAnim;
	Animation idleAnim;
	Animation jumpAnim;
	Animation dieAnim;


	//L02: DONE 2: Declare player parameters
	float speed = 0.2f;
	SDL_Texture* texture = NULL;
	pugi::xml_node config;
	uint texW, texH;


	//Audio fx
	int pickCoinFxId;

	// L07 TODO 5: Add physics to the player - declare a Physics body
	PhysBody* pbody;

	float jumpSpeed = 5.0f;
	float dashSpeed = 100.0f;
	int jumpCount = 0;
	bool left = false;
	bool godmode;
	bool tocasuelo = true;
	bool die = false;
	
};

#endif // __PLAYER_H__