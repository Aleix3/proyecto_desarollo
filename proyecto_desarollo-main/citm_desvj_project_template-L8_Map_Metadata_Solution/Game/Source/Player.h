#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"
#include "Physics.h"
#include <map>
#include <chrono>

struct SDL_Texture;

enum class PlayerState {
	IDLE,
	JUMPING,
	DYING
	// A�adir mas estados para el personaje
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

	void OnCollision(PhysBody* physA, PhysBody* physB);

	bool Cooldown(float cooldown);

	bool Player::LoadState(pugi::xml_node node);

	bool Player::SaveState(pugi::xml_node node);

public:

	Animation* currentAnimation = nullptr;
	Animation* currentJumpAnim = nullptr;

	Animation runAnim;
	Animation walkAnim;
	Animation walkLeftAnim;
	Animation idleAnim;
	Animation jumpAnim;
	Animation dieAnim;
	Animation dashAnim1;
	Animation dashAnim2;

	float speed = 0.2f;
	SDL_Texture* texture = NULL;
	pugi::xml_node config;
	uint texW, texH;
	uint texWl, texHl;
	int pickCoinFxId;

	PhysBody* pbody;

	PhysBody* rectangle;

	b2Vec2 velocity;

	float jumpSpeed = 5.0f;
	float dashSpeed = 100.0f;
	int jumpCount = 0;
	bool left = false;
	bool godmode = false;
	bool tocasuelo = true;
	bool die = false;
	bool dash = false;
	bool isDying = false;
	bool dash2 = false;
	bool comprovacionFX = true;
	bool restarvida = false;
	bool dano = false;
	bool portal = false;
	bool level2 = false;
	bool level1 = false;
	bool diedie = false;
	bool win = true;
	//sonidos
	uint golpePlayerFX = 0;
	uint dashFX = 0;
	uint jumpFX = 0;


	std::chrono::steady_clock::time_point ultimo_uso;
};

#endif // __PLAYER_H__