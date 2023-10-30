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

int Salto = 0;

bool Player::Awake() {

	//L03: DONE 2: Initialize Player parameters
	position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());

	// idle animation
	idleAnim.PushBack({ 5, 5, 23, 28 });
	idleAnim.PushBack({ 28, 33, 23, 28 });
	idleAnim.speed = 0.2f;
	idleAnim.loop = true;

	


	return true;
}

bool Player::Start() {

	texture = app->tex->Load(config.attribute("texturePath").as_string());

	

	// L07 TODO 5: Add physics to the player - initialize physics body
	app->tex->GetSize(texture, texW, texH);
	pbody = app->physics->CreateCircle(position.x, position.y, texW / 2, bodyType::DYNAMIC);

	// L07 TODO 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// L07 TODO 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	//initialize audio effect
	pickCoinFxId = app->audio->LoadFx(config.attribute("coinfxpath").as_string());


	return true;
}

bool Player::Update(float dt)
{
	// L07 TODO 5: Add physics to the player - updated player position using physics

	//L03: DONE 4: render the player texture and modify the position of the player using WSAD keys and render the texture
	


	currentAnimation = &idleAnim;

	b2Vec2 velocity = b2Vec2(0, -GRAVITY_Y);

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		velocity.x = -0.2*dt;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		velocity.x = 0.2*dt;
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && Salto == 0)	//&& IsOnGround() == true)
	{
		Salto = 2;
		velocity.y = -jumpSpeed;

		if (Salto == 2)
		{

			velocity.y += GRAVITY_Y * dt;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && Salto == 1)	//&& IsOnGround() == true)
	{
		Salto = 0;
		velocity.y = -jumpSpeed;

		if (Salto == 0)
		{

			velocity.y += GRAVITY_Y * dt;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		velocity.x = -dashSpeed;
		//if (pbody->body->GetAngle() == 0)
		//{
		//	velocity.x = dashSpeed;
		//}
		//if (pbody->body->GetAngle() == 180)
		//{
		//	velocity.x = -dashSpeed;
		//}
	}

	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		velocity.x = dashSpeed;
		//if (pbody->body->GetAngle() == 0)
		//{
		//	velocity.x = dashSpeed;
		//}
		//if (pbody->body->GetAngle() == 180)
		//{
		//	velocity.x = -dashSpeed;
		//}
	}



	
		
	pbody->body->SetLinearVelocity(velocity);
	b2Transform pbodyPos = pbody->body->GetTransform();
	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2;

	app->render->DrawTexture(texture,position.x,position.y);




	return true;
}

bool Player::CleanUp()
{
	return true;
}

// L07 TODO 6: Define OnCollision function for the player. 
void Player::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		Salto = 1;
		LOG("Collision PLATFORM");
		break;
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	default:
		break;
	}
}

bool Player::IsOnGround() 
{
	

	return false;
}