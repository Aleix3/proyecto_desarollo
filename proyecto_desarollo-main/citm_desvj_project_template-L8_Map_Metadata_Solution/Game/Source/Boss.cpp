#include "Boss.h"
#include "App.h"
#include "Textures.h"
#include "Physics.h"

Boss::Boss() : Entity(EntityType::BOSS) {

}

Boss::~Boss() {

}

bool Boss::Awake() {
	name.Create("Boss");

    idleAnim.PushBack({ 0, 700, 125, 100 });
    idleAnim.PushBack({ 125, 700, 125, 100 });
    idleAnim.PushBack({ 250, 700, 125, 100 });
    idleAnim.PushBack({ 375, 700, 125, 100 });
    idleAnim.PushBack({ 0, 800, 125, 100 });
    idleAnim.PushBack({ 125, 800, 125, 100 });
    idleAnim.PushBack({ 250, 800, 125, 100 });
    idleAnim.PushBack({ 375, 800, 125, 100 });
    idleAnim.PushBack({ 0, 900, 125, 100 });
    idleAnim.PushBack({ 125, 900, 125, 100 });
    idleAnim.PushBack({ 250, 900, 125, 100 });
    idleAnim.PushBack({ 375, 900, 125, 100 });
    idleAnim.speed = 0.10f;

    damageAnim.PushBack({ 0, 0, 125, 100 });
    damageAnim.PushBack({ 125, 0, 125, 100 });
    damageAnim.PushBack({ 250, 0, 125, 100 });
    damageAnim.PushBack({ 375, 0, 125, 100 });
    damageAnim.PushBack({ 500, 0, 125, 100 });
    damageAnim.PushBack({ 625, 0, 125, 100 });
    damageAnim.PushBack({ 0, 100, 125, 100 });
    damageAnim.PushBack({ 125, 100, 125, 100 });
    damageAnim.PushBack({ 250, 100, 125, 100 });
    damageAnim.PushBack({ 375, 100, 125, 100 });
    damageAnim.PushBack({ 500, 100, 125, 100 });
    damageAnim.PushBack({ 625, 100, 125, 100 });
    damageAnim.speed = 0.10f;

    attackAnim.PushBack({ 0, 200, 125, 100 });
    attackAnim.PushBack({ 125, 200, 125, 100 });
    attackAnim.PushBack({ 250, 200, 125, 100 });
    attackAnim.PushBack({ 375, 200, 125, 100 });
    attackAnim.PushBack({ 500, 200, 125, 100 });
    attackAnim.PushBack({ 625, 200, 125, 100 });
    attackAnim.PushBack({ 194, 200, 125, 100 });
    attackAnim.speed = 0.17f;
    attackAnim.loop = false;

    spawnAnim.PushBack({ 0, 1000, 125, 100 });
    spawnAnim.PushBack({ 125, 1000, 125, 100 });
    spawnAnim.PushBack({ 250, 1000, 125, 100 });
    spawnAnim.PushBack({ 375, 1000, 125, 100 });
    spawnAnim.PushBack({ 0, 1100, 125, 100 });
    spawnAnim.loop = false;

    ultimo_uso = std::chrono::steady_clock::now();

	return true;
}

bool Boss::Start() {
    texture = app->tex->Load(config.attribute("texturePath").as_string());
    app->tex->GetSize(texture, texW, texH);

    pbody = app->physics->CreateCircle(position.x, position.y, 11, bodyType::DYNAMIC);
    pbody->listener = this;
    pbody->ctype = ColliderType::BOSS;
	return true;
}

bool Boss::Update(float dt) {
    currentAnimation = &idleAnim;
	// El boss no se movera, sequedará quieto y irá spwaneando varios bichos cada cierto tiempo. Los bichos seguirán al player con el path
	// Si el player se acerca mucho al Boss, el Boss atacará con su guadaña. El Boss tiene una cierta cantidad de vida
	return true;
}

bool Boss::CleanUp() {
	return true;
}

void Boss::OnCollision(PhysBody* physA, PhysBody* physB) {

}
