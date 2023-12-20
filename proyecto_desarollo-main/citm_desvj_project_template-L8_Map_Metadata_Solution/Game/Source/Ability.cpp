#include "Ability.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Player.h"

#include "SDL_image/include/SDL_image.h"

Ability::Ability() : Module()
{
	name.Create("Ability");

}

Ability::~Ability() {}

bool Ability::Awake() {


	return true;
}

bool Ability::Start() {

	texture = app->tex->Load("Assets/Textures/character/Abilities/Fuego.png");
	fireballAnim.PushBack({ 416-64,480,32,32 });
	fireballAnim.PushBack({ 448-64,480,32,32 });
	fireballAnim.PushBack({ 480-64,480,32,32 });
	fireballAnim.PushBack({ 512-64,480,32,32 });
	fireballAnim.speed = 0.09f;
	fireballAnim.loop = true;
	

	return true;
}

bool Ability::Update(float dt)
{

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		if (app->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN && app->scene->GetPlayer()->Cooldown(5.0f))
		{
			position = app->scene->GetPlayer()->position;
			if (dispar != nullptr)
			{
				app->physics->DestroyBody(dispar);
			}
			
			b2Vec2 forceToApply(0.0f, -50.0f);
			b2Vec2 pointOfApplication(position.x + 50, position.y + 50);

			dispar = app->physics->CreateCircle(position.x + 50, position.y, 11, bodyType::DYNAMIC);
			currentAnimation = &fireballAnim;
			dispar->body->SetGravityScale(0.0f);

			dispar->body->ApplyForce(forceToApply, pointOfApplication, true);

		}
	}

	else if (app->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN && app->scene->GetPlayer()->Cooldown(5.0f))
	{
		position = app->scene->GetPlayer()->position;
		if (app->scene->GetPlayer()->left) direccionLanzamiento = false;
		else {
			direccionLanzamiento = true;
		}

		if (dispar != nullptr)
		{
			app->physics->DestroyBody(dispar);
		}

		b2Vec2 forceToApply;
		if (direccionLanzamiento) {
			forceToApply.Set(50.0f, 0.0f); // Derecha
		}
		else {
			forceToApply.Set(-50.0f, 0.0f); // Izquierda
		}
			b2Vec2 pointOfApplication(position.x + 50, position.y + 50);

		dispar = app->physics->CreateCircle(position.x + 50, position.y, 11, bodyType::DYNAMIC);
		currentAnimation = &fireballAnim;
		dispar->ctype = ColliderType::ABILITY;
		dispar->body->SetGravityScale(0.0f);
		dispar->body->ApplyForce(forceToApply, pointOfApplication, true);


	}


		

	if (dispar != nullptr)
	{
		b2Transform pbodyPos = dispar->body->GetTransform();
		position.x = METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2;
		position.y = METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2;
	}

	
	if (currentAnimation != nullptr)
	{
		currentAnimation->Update();
		SDL_Rect rect = (currentAnimation->GetCurrentFrame());

		if (direccionLanzamiento == false) {
			app->render->DrawTexture2(texture, position.x, position.y, SDL_FLIP_HORIZONTAL, &rect);
		}
		else {
			app->render->DrawTexture2(texture, position.x, position.y, SDL_FLIP_NONE, &rect);
		}
	}

	return true;
}

bool Ability::CleanUp()
{
	return true;
}

void Ability::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		app->physics->DestroyBody(dispar);
		break;
	case ColliderType::DIE:
		LOG("Collision DIE");
		app->physics->DestroyBody(dispar);
		break;
	case ColliderType::ENEMY:
		LOG("Collision ENEMY");
		app->physics->DestroyBody(dispar);
		break;
	case ColliderType::PLAYER:
		LOG("Collision PLAYER");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	default:
		break;
	}
}
