#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 32, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

		///////////////////////
		//    ANIMACIONES    //
		///////////////////////

		// idle animation
		idleAnim.PushBack({ 60, 48, 46, 60 });
		idleAnim.speed = 0.2f;

		// walk forward animation
		forwardAnim.PushBack({ 11, 112, 46, 60 });
		forwardAnim.PushBack({ 65, 112, 46, 60 });
		forwardAnim.PushBack({ 119, 112, 46, 60 });
		forwardAnim.PushBack({ 173, 112, 46, 60 });
		forwardAnim.PushBack({ 227, 112, 46, 60 });
		forwardAnim.PushBack({ 281, 112, 46, 60 });
		forwardAnim.speed = 0.17f;

		//jump anim
		jumpAnim.PushBack({ 10, 357, 33, 58 });
		jumpAnim.PushBack({ 47, 357, 33, 58 });
		jumpAnim.PushBack({ 47, 357, 33, 58 });
		jumpAnim.PushBack({ 47, 357, 33, 58 });
		jumpAnim.PushBack({ 47, 357, 33, 58 });
		jumpAnim.PushBack({ 47, 357, 33, 58 });
		jumpAnim.PushBack({ 84, 357, 33, 58 });

		jumpAnim.speed = 0.035f;
		jumpAnim.loop = false;

		jumpAttackAnim.PushBack({ 190, 357, 49, 59 });
		jumpAttackAnim.PushBack({ 243, 357, 49, 59 });

		jumpAttackAnim.speed = 0.1f;
		jumpAttackAnim.loop = false;


		DeathAnim.PushBack({ 67, 651, 41, 43 });
		DeathAnim.PushBack({ 112, 651, 41, 43 });
		DeathAnim.PushBack({ 157, 672, 72, 22 });
		DeathAnim.PushBack({ 233, 672, 72, 22 });
		DeathAnim.loop = false;


		DeathAnim.speed = 0.1f;

		//Espada

		PatadaAnim.PushBack({ 375, 656, 44, 37 });
		PatadaAnim.PushBack({ 427, 654, 65, 39 });
		PatadaAnim.PushBack({ 375, 656, 44, 37 });
		PatadaAnim.speed = 0.2f;
		PatadaAnim.loop = false;


		EspadaAnim.PushBack({ 10, 916, 67, 63 });
		EspadaAnim.PushBack({ 83, 916, 67, 63 });
		EspadaAnim.PushBack({ 156, 916, 67, 63 });
		EspadaAnim.PushBack({ 229, 916, 67, 63 });
		EspadaAnim.PushBack({ 302, 916, 67, 63 });
		EspadaAnim.PushBack({ 375, 916, 67, 63 });
		EspadaAnim.PushBack({ 448, 916, 67, 63 });
		EspadaAnim.PushBack({ 521, 916, 67, 63 });
		EspadaAnim.speed = 0.5f;
		EspadaAnim.loop = false;


	return true;
}

bool Player::Update(float dt)
{

	currentAnimation = &idleAnim;

	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);


	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		//
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		//
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		vel = b2Vec2(-speed*dt, -GRAVITY_Y);
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		vel = b2Vec2(speed*dt, -GRAVITY_Y);
	}

	//Set the velocity of the pbody of the player
	pbody->body->SetLinearVelocity(vel);

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	

	currentAnimation->Update();

	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);


	return true;
}

bool Player::CleanUp()
{

	return true;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
}