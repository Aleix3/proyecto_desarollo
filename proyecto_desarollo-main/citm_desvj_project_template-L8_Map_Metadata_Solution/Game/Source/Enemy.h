#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"
#include "Physics.h"
#include <map>

struct SDL_Texture;

enum class EnemyState {
	IDLE,
	JUMPING,
	ATACKING,
	CHASING,
	SEARCHING,
	DYING
	// Añadir mas estados para el personaje
};

class Enemy : public Entity
{
public:

	EnemyState currentState;
	std::map<EnemyState, Animation> animations;

	Enemy();

	virtual ~Enemy();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	Animation* currentAnimation = nullptr;
	SDL_Texture* texture = NULL;
	pugi::xml_node config;
	uint texW, texH;
	
};

#endif // __PLAYER_H__