#include "Boss.h"
#include "App.h"
#include "Textures.h"
#include "Physics.h"
#include "Log.h"
#include "Scene.h"

Boss::Boss() : Entity(EntityType::BOSS) {
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
    idleAnim.speed = 0.05f;

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
    damageAnim.speed = 0.30f;

    attackAnim.PushBack({ 0, 200, 125, 100 });
    attackAnim.PushBack({ 125, 200, 125, 100 });
    attackAnim.PushBack({ 250, 200, 125, 100 });
    attackAnim.PushBack({ 375, 200, 125, 100 });
    attackAnim.PushBack({ 500, 200, 125, 100 });
    attackAnim.PushBack({ 625, 200, 125, 100 });
    attackAnim.PushBack({ 194, 200, 125, 100 });
    attackAnim.speed = 0.37f;

    spawnAnim.PushBack({ 0, 1000, 125, 100 });
    spawnAnim.PushBack({ 125, 1000, 125, 100 });
    spawnAnim.PushBack({ 250, 1000, 125, 100 });
    spawnAnim.PushBack({ 375, 1000, 125, 100 });
    spawnAnim.PushBack({ 0, 1100, 125, 100 });
    spawnAnim.speed = 0.37f;

    ultimo_uso = std::chrono::steady_clock::now();
}

Boss::~Boss() {

}

bool Boss::Awake() {

    position.x = parameters.attribute("x").as_int();
    position.y = parameters.attribute("y").as_int();
	return true;
}

bool Boss::Start() {
    texture = app->tex->Load(parameters.attribute("texturePath").as_string());
    app->tex->GetSize(texture, texW, texH);

    pbody = app->physics->CreateCircle(position.x, position.y, 25, bodyType::DYNAMIC);
    pbody->listener = this;
    pbody->ctype = ColliderType::BOSS;
	return true;
}

bool Boss::Update(float dt) {
    currentAnimation = &idleAnim;
	// El boss no se movera, sequedará quieto y irá spwaneando varios bichos cada cierto tiempo. Los bichos seguirán al player con el path
	// Si el player se acerca mucho al Boss, el Boss atacará con su guadaña. El Boss tiene una cierta cantidad de vida

    if (pbody != nullptr)
    {
        velocity2 = pbody->body->GetLinearVelocity();
        velocity2.x = 0.0;

        if (app->scene->GetPlayer()->position.x > position.x)
        {
            faceleft = false;
        }
        if (app->scene->GetPlayer()->position.x < position.x)
        {
            faceleft = true;
        }

        if (app->scene->GetPlayer()->die == true)
        {
            app->physics->DestroyBody(pbody);
            position.x = parameters.attribute("x").as_int();
            position.y = parameters.attribute("y").as_int();

            pbody = app->physics->CreateCircle(position.x, position.y, 15, bodyType::DYNAMIC);
            pbody->listener = this;
            pbody->ctype = ColliderType::BOSS;
        }

        else
        {
            pbody->body->SetLinearVelocity(velocity2);
            b2Transform pbodyPos = pbody->body->GetTransform();
            position.x = METERS_TO_PIXELS(pbodyPos.p.x) - 60;
            position.y = METERS_TO_PIXELS(pbodyPos.p.y) - 60;
        }
    }
    
    currentAnimation->Update();

    SDL_Rect rect = currentAnimation->GetCurrentFrame();

    if (faceleft) {
        app->render->DrawTexture2(texture, position.x, position.y, SDL_FLIP_NONE, &rect);
    }
    else {

        app->render->DrawTexture2(texture, position.x, position.y, SDL_FLIP_HORIZONTAL, &rect);
    }
	return true;
}

bool Boss::CleanUp() {
    app->tex->UnLoad(texture);
	return true;
}

void Boss::OnCollision(PhysBody* physA, PhysBody* physB) {
    switch (physB->ctype)
    {
    case ColliderType::PLATFORM:
        LOG("Collision PLATFORM");
        break;
    case ColliderType::DIE:
        LOG("Collision DIE");
        break;
    case ColliderType::ABILITY:
        LOG("Collision ABILITY");
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
