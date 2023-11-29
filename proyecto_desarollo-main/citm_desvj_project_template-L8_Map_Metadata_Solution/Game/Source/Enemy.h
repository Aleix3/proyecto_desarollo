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
	FOLLOWING,
	DYING
	// A�adir mas estados para el personaje
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

	Animation idleAnim;


	float speed = 0.2f;
	SDL_Texture* texture = NULL;
	pugi::xml_node config;
	uint texW, texH;

	PhysBody* pbody;

	float jumpSpeed = 5.0f;
	float dashSpeed = 100.0f;
	int jumpCount = 0;
	bool tocasuelo = true;
	bool die = false;

};

#endif // __PLAYER_H__