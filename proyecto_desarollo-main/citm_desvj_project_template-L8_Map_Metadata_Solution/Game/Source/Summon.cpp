#include "Summon.h"
#include "App.h"
#include "Textures.h"
#include "Physics.h"
#include "Scene.h"

Summon::Summon() : Entity(EntityType::SUMMON) {
    name.Create("Summon");

    //idleAnim.PushBack({ 0, 700, 125, 100 });
    //idleAnim.PushBack({ 125, 700, 125, 100 });
    //idleAnim.PushBack({ 250, 700, 125, 100 });
    //idleAnim.PushBack({ 375, 700, 125, 100 });
    //idleAnim.PushBack({ 0, 800, 125, 100 });
    //idleAnim.PushBack({ 125, 800, 125, 100 });
    //idleAnim.PushBack({ 250, 800, 125, 100 });
    //idleAnim.PushBack({ 375, 800, 125, 100 });
    //idleAnim.PushBack({ 0, 900, 125, 100 });
    //idleAnim.PushBack({ 125, 900, 125, 100 });
    //idleAnim.PushBack({ 250, 900, 125, 100 });
    //idleAnim.PushBack({ 375, 900, 125, 100 });
    //idleAnim.speed = 0.10f;

    //chasingAnim.PushBack({ 0, 0, 125, 100 });
    //chasingAnim.PushBack({ 125, 0, 125, 100 });
    //chasingAnim.PushBack({ 250, 0, 125, 100 });
    //chasingAnim.PushBack({ 375, 0, 125, 100 });
    //chasingAnim.PushBack({ 500, 0, 125, 100 });
    //chasingAnim.PushBack({ 625, 0, 125, 100 });
    //chasingAnim.PushBack({ 0, 100, 125, 100 });
    //chasingAnim.PushBack({ 125, 100, 125, 100 });
    //chasingAnim.PushBack({ 250, 100, 125, 100 });
    //chasingAnim.PushBack({ 375, 100, 125, 100 });
    //chasingAnim.PushBack({ 500, 100, 125, 100 });
    //chasingAnim.PushBack({ 625, 100, 125, 100 });
    //chasingAnim.speed = 0.10f;

    //dyingAnim.PushBack({ 0, 200, 125, 100 });
    //dyingAnim.PushBack({ 125, 200, 125, 100 });
    //dyingAnim.PushBack({ 250, 200, 125, 100 });
    //dyingAnim.PushBack({ 375, 200, 125, 100 });
    //dyingAnim.PushBack({ 500, 200, 125, 100 });
    //dyingAnim.PushBack({ 625, 200, 125, 100 });
    //dyingAnim.PushBack({ 194, 200, 125, 100 });
    //dyingAnim.speed = 0.17f;
    dyingAnim.loop = false;

    ultimo_uso = std::chrono::steady_clock::now();
}

Summon::~Summon() {

}

bool Summon::Awake() {
	
    position.x = app->scene->GetBoss()->position.x;
    position.y = app->scene->GetBoss()->position.y;
   

	return true;
}

bool Summon::Start() {
    texture = app->tex->Load(config.attribute("texturePath").as_string());
    app->tex->GetSize(texture, texW, texH);

    pbody = app->physics->CreateCircle(position.x, position.y, 11, bodyType::DYNAMIC);
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
