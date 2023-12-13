#ifndef __ENEMYVULTURE_H__
#define __ENEMYVULTURE_H__

#include "Enemy.h"

struct SDL_Texture;

class EnemyVulture : public Enemy
{
public:

	EnemyVulture();

	virtual ~EnemyVulture();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	Animation* currentAnimation = nullptr;

	Animation idleAnim;
	Animation dieAnim;
	Animation flyAnim;

	float speed = 0.2f;
	SDL_Texture* texture = NULL;
	pugi::xml_node config;
	uint texW, texH;

	b2Vec2 velocity2;
	PhysBody* pbody;

	float jumpSpeed = 5.0f;
	float dashSpeed = 100.0f;
	int jumpCount = 0;
	bool tocasuelo = true;
	bool die = false;
	bool faceleft;
	float patrolTime = 0.0f;
	float patrolInterval = 3.0f;

};

#endif // __PLAYER_H__