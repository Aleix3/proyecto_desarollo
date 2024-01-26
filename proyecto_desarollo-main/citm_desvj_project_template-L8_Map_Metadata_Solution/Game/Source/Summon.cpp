#include "Summon.h"
#include "App.h"
#include "Textures.h"
#include "Physics.h"
#include "Scene.h"
#include "Boss.h"

Summon::Summon() : Entity(EntityType::SUMMON) {
    name.Create("Summon");

    spawnAnim.PushBack({ 0, 0, 41, 25 });
    spawnAnim.PushBack({ 41, 0, 41, 25 });
    spawnAnim.PushBack({ 82, 0, 41, 25 });
    spawnAnim.PushBack({ 123, 0, 41, 25 });
    spawnAnim.PushBack({ 164, 0, 41, 25 });
    spawnAnim.PushBack({ 185, 0, 41, 25 });
    spawnAnim.speed = 0.10f;

    chasingAnim.PushBack({ 0, 0, 125, 100 });
    chasingAnim.PushBack({ 125, 0, 125, 100 });
    chasingAnim.PushBack({ 250, 0, 125, 100 });
    chasingAnim.PushBack({ 375, 0, 125, 100 });
    chasingAnim.PushBack({ 500, 0, 125, 100 });
    chasingAnim.PushBack({ 625, 0, 125, 100 });
    chasingAnim.PushBack({ 0, 100, 125, 100 });
    chasingAnim.PushBack({ 125, 100, 125, 100 });
    chasingAnim.PushBack({ 250, 100, 125, 100 });
    chasingAnim.PushBack({ 375, 100, 125, 100 });
    chasingAnim.PushBack({ 500, 100, 125, 100 });
    chasingAnim.PushBack({ 625, 100, 125, 100 });
    chasingAnim.speed = 0.10f;

    dyingAnim.PushBack({ 0, 200, 41, 25 });
    dyingAnim.PushBack({ 125, 200, 41, 25 });
    dyingAnim.PushBack({ 250, 200, 41, 25 });
    dyingAnim.PushBack({ 375, 200, 41, 25 });
    dyingAnim.speed = 0.17f;
    dyingAnim.loop = false;

    ultimo_uso = std::chrono::steady_clock::now();
}

Summon::~Summon() {

}

bool Summon::Awake() {
    texture = app->tex->Load(parameters.attribute("texturePath").as_string());
    app->tex->GetSize(texture, texW, texH);
    position.x = parameters.attribute("x").as_int();
    position.y = parameters.attribute("y").as_int();
	return true;
}

bool Summon::Start() {


    pbody = app->physics->CreateCircle(position.x, position.y, 15, bodyType::DYNAMIC);
    pbody->listener = this;
    pbody->ctype = ColliderType::ENEMY;
	return true;
}

bool Summon::Update(float dt) {
	return true;
}

bool Summon::CleanUp() {
	return true;
}

void Summon::OnCollision(PhysBody* physA, PhysBody* physB) {

}

void Summon::spawn()
{
   
    app->physics->DestroyBody(pbody);
    if (pbody != nullptr)
    {

    }
    position.x = 10722; 
    position.y = 1632;
    pbody = app->physics->CreateCircle(position.x, position.y, 15, bodyType::DYNAMIC);
    pbody->listener = this;
    pbody->ctype = ColliderType::ENEMY;

}
