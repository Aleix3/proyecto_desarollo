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

	PortalAnim.PushBack({ 512, 256,31,31 });
	PortalAnim.PushBack({ 544 ,255,30,31 });
	PortalAnim.PushBack({ 575,256,32,30 });
	PortalAnim.PushBack({ 608,256,30,30 });
	PortalAnim.speed = 0.09f;
	PortalAnim.loop = true;
	
	fireballFX = app->audio->LoadFx("Assets/Audio/Fx/FireBall.wav");

	portalfx = app->audio->LoadFx("Assets/Audio/Fx/Portalfx.wav");

	//colision portales
	rectangle = app->physics->CreateRectangle(4944, 1954, 32, 32, STATIC);
	rectangle->ctype = ColliderType::PORTAL;
	rectangle2 = app->physics->CreateRectangle(10142, 936, 32, 32, STATIC);
	rectangle2->ctype = ColliderType::PORTAL;
	
	return true;
}

bool Ability::Update(float dt)
{
	
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN && app->scene->GetPlayer()->Cooldown(5.0f))
		{
			app->audio->PlayFx(fireballFX);
			arriba = true;
			position = app->scene->GetPlayer()->position;
			if (dispar != nullptr)
			{
				app->physics->DestroyBody(dispar);
			}
			
			b2Vec2 forceToApply(0.0f, -50.0f);
			b2Vec2 pointOfApplication(position.x, position.y);

			dispar = app->physics->CreateCircle(position.x, position.y, 11, bodyType::DYNAMIC);
			currentAnimation = &fireballAnim;
			dispar->ctype = ColliderType::ABILITY;
			dispar->body->SetGravityScale(0.0f);

			dispar->body->ApplyForce(forceToApply, pointOfApplication, true);

		}
	}

	else if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN && app->scene->GetPlayer()->Cooldown(5.0f))
	{
		app->audio->PlayFx(fireballFX);
		arriba = false;
		position = app->scene->GetPlayer()->position;
		if (dispar != nullptr)
		{
			app->physics->DestroyBody(dispar);
		}

		if (app->scene->GetPlayer()->left == true)
		{
			left = true;
			b2Vec2 forceToApply(-50.0f, 0.0f);
			b2Vec2 pointOfApplication(position.x - 50, position.y);

			dispar = app->physics->CreateCircle(position.x - 50, position.y, 11, bodyType::DYNAMIC);
			currentAnimation = &fireballAnim;
			dispar->ctype = ColliderType::ABILITY;
			dispar->body->SetGravityScale(0.0f);

			dispar->body->ApplyForce(forceToApply, pointOfApplication, true);
		}

		else
		{
			left = false;
			b2Vec2 forceToApply(50.0f, 0.0f);
			b2Vec2 pointOfApplication(position.x + 50, position.y);

			dispar = app->physics->CreateCircle(position.x + 50, position.y, 11, bodyType::DYNAMIC);
			currentAnimation = &fireballAnim;
			dispar->ctype = ColliderType::ABILITY;
			dispar->body->SetGravityScale(0.0f);

			dispar->body->ApplyForce(forceToApply, pointOfApplication, true);
		}


	}
	if (dispar != nullptr)
	{
		b2Transform pbodyPos = dispar->body->GetTransform();
		position.x = METERS_TO_PIXELS(pbodyPos.p.x) - 16;
		position.y = METERS_TO_PIXELS(pbodyPos.p.y) - 16;
	}

	
	if (currentAnimation != nullptr)
	{
		currentAnimation->Update();
		SDL_Rect rect = (currentAnimation->GetCurrentFrame());

		if (arriba)
		{
			app->render->DrawTexture2(texture, position.x, position.y, SDL_FLIP_NONE, &rect, 1, -90);
		}
		else
		{
			if (left) {
				app->render->DrawTexture2(texture, position.x, position.y, SDL_FLIP_HORIZONTAL, &rect);
			}
			else {
				app->render->DrawTexture2(texture, position.x, position.y, SDL_FLIP_NONE, &rect);

			}
		}
		
	}

	currentportal = &PortalAnim;
	SDL_Rect recta = (currentportal->GetCurrentFrame());

	app->render->DrawTexture2(texture, 4944, 1954, SDL_FLIP_NONE, &recta);
	app->render->DrawTexture2(texture, 10122, 926, SDL_FLIP_NONE, &recta);
	
	currentportal->Update();
	

	return true;
}

bool Ability::CleanUp()
{
	return true;
}

