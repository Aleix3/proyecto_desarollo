#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"

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

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:
	float speed = 0.2f;
	const char* texturePath;
	SDL_Texture* texture = NULL;
	PhysBody* pbody;
	int pickCoinFxId;

	Animation* currentAnimation = nullptr;
	Animation* currentJumpAnim = nullptr;

	// A set of animations
	Animation idleAnim;
	Animation forwardAnim;
	Animation crouched_idleAnim;
	Animation crouched_forwardAnim;
	Animation crouched_AttackAnim;
	Animation jumpAnim;
	Animation jumpAttackAnim;
	Animation DeathAnim;

	Animation PatadaAnim;
	Animation EspadaCrouchAnim;
	Animation EspadaAnim;

};

#endif // __PLAYER_H__