#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Hud.h"
#include "Heart.h"

Heart::Heart() : Entity(EntityType::HEART)
{
	name.Create("item");
}

Heart::~Heart() {}

bool Heart::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Heart::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	app->tex->GetSize(texture, texW, texH);
	pbody = app->physics->CreateCircle(position.x + texH / 2, position.y + texH / 2, texH / 2, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ITEM;


	return true;
}

bool Heart::Update(float dt)
{
	if (isPicked == true)
	{
		position.x = -1000;
		position.y = -1000;
		app->physics->DestroyBody(pbody);
		pbody = app->physics->CreateCircle(position.x + texH / 2, position.y + texH / 2, texH / 2, bodyType::DYNAMIC);
		pbody->listener = this;
		pbody->ctype = ColliderType::ITEM;

		isPicked = false;
	}


	b2Transform pbodyPos = pbody->body->GetTransform();
	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2;



	app->render->DrawTexture(texture, position.x, position.y);


	return true;
}

bool Heart::CleanUp()
{
	return true;
}

void Heart::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;

	case ColliderType::PLAYER:
		LOG("Collision PLAYER");
		if (app->hud->lives < 3)
		{
			isPicked = true;
			app->hud->lives += 1;
		}
		
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	default:
		break;
	}
}