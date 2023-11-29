#include "Enemy.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "map.h"

Enemy::Enemy() : Entity(EntityType::ENEMY)
{
    name.Create("EnemySamurai");

    idleAnim.PushBack({ 23, 20, 18, 27 });
    idleAnim.PushBack({ 87, 20, 18, 27 });
    idleAnim.PushBack({ 151, 20, 18, 27 });
    idleAnim.PushBack({ 215, 20, 18, 27 });
    idleAnim.PushBack({ 279, 20, 18, 27 });
    idleAnim.PushBack({ 343, 20, 18, 27 });
    idleAnim.speed = 0.05f;
}

Enemy::~Enemy() {

}

bool Enemy::Awake() {

    position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());

    return true;
}

bool Enemy::Start() {

    texture = app->tex->Load(config.attribute("texturePath").as_string());
    app->tex->GetSize(texture, texW, texH);

    pbody = app->physics->CreateCircle(position.x, position.y, 15, bodyType::DYNAMIC);
    pbody->listener = this;
    pbody->ctype = ColliderType::ENEMY;
    return true;
}


bool Enemy::Update(float dt) {

    currentAnimation = &idleAnim;

    b2Vec2 velocity = pbody->body->GetLinearVelocity();
    if (die == true)
    {
        position.x = 300;
        position.y = 672;
        app->physics->DestroyBody(pbody);
        pbody = app->physics->CreateCircle(position.x, position.y, 15, bodyType::DYNAMIC);
        pbody->listener = this;
        pbody->ctype = ColliderType::ENEMY;
        die = false;
    }
    b2Transform pbodyPos = pbody->body->GetTransform();

    position.x = METERS_TO_PIXELS(pbodyPos.p.x) - 11;
    position.y = METERS_TO_PIXELS(pbodyPos.p.y) - 11;

    currentAnimation->Update();

    SDL_Rect rect = currentAnimation->GetCurrentFrame();

    app->render->DrawTexture(texture, position.x, position.y, &rect);

    return true;
}

bool Enemy::CleanUp()
{
    app->tex->UnLoad(texture);
    return true;
}

void Enemy::OnCollision(PhysBody* physA, PhysBody* physB) {
    switch (physB->ctype)
    {
    case ColliderType::PLATFORM:
        LOG("Collision PLATFORM");
        break;
    case ColliderType::DIE:
        LOG("Collision DIE");
        die = true;
        break;
    case ColliderType::UNKNOWN:
        LOG("Collision UNKNOWN");
        break;
    case ColliderType::PLAYER:
        LOG("Collision UNKNOWN");
        die = true;
        break;
    default:
        break;
    }
}