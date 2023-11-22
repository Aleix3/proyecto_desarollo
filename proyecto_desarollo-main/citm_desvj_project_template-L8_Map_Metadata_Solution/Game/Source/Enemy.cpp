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
    name.Create("Enemy");

    runAnim.PushBack({ 4, 36,  23, 28 });
    runAnim.PushBack({ 37,35,  22, 28 });
    runAnim.PushBack({ 68, 36, 23, 27 });
    runAnim.PushBack({ 101, 35, 22, 28 });
    runAnim.speed = 0.17f;

    idleAnim.PushBack({ 4, 4, 23, 27 });
    idleAnim.PushBack({ 37, 3, 22, 28 });
    idleAnim.speed = 0.05f;

    jumpAnim.PushBack({ 4, 131, 23, 28 });
    jumpAnim.PushBack({ 35, 132, 24, 27 });
    jumpAnim.PushBack({ 70, 130, 22, 29 });
    jumpAnim.PushBack({ 102, 128, 21, 30 });
    jumpAnim.PushBack({ 134, 128, 21, 28 });
    jumpAnim.PushBack({ 164, 129, 23, 28 });
    jumpAnim.PushBack({ 196, 130, 23, 29 });
    jumpAnim.PushBack({ 228, 132, 24, 27 });
    jumpAnim.PushBack({ 259, 132, 24, 27 });
    jumpAnim.PushBack({ 292, 132, 23, 27 });
    jumpAnim.speed = 0.15f;

    dieAnim.PushBack({ 4, 196, 23, 27 });
    dieAnim.PushBack({ 37, 195, 32, 32 });
    dieAnim.PushBack({ 68, 196, 23, 27 });
    dieAnim.PushBack({ 100, 198, 23, 25 });
    dieAnim.PushBack({ 132, 199, 23, 24 });
    dieAnim.PushBack({ 164, 200, 24, 23 });
    dieAnim.PushBack({ 194, 202, 27, 21 });
    dieAnim.PushBack({ 226, 204, 28, 19 });
    dieAnim.PushBack({ 258, 205, 29, 18 });
    dieAnim.PushBack({ 290, 206, 29, 17 });
    dieAnim.speed = 0.17f;
    dieAnim.loop = false;

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

    /*textureleft = app->tex->Load(config.attribute("texturePath").as_string()); //EL PLAYER A LA IZZQUIERDA
    app->tex->GetSize(textureleft, texWl, texHl);*/

    pbody = app->physics->CreateCircle(position.x, position.y, 11, bodyType::DYNAMIC);
    pbody->listener = this;
    pbody->ctype = ColliderType::PLAYER;
    return true;
}


bool Enemy::Update(float dt) {

    currentAnimation = &idleAnim;

    b2Vec2 velocity = pbody->body->GetLinearVelocity();
    if (die == true)
    {
        currentAnimation = &dieAnim;
        if (dieAnim.HasFinished())
        {
            position.x = 250;
            position.y = 672;
            app->physics->DestroyBody(pbody);
            pbody = app->physics->CreateCircle(position.x, position.y, 11, bodyType::DYNAMIC);
            pbody->listener = this;
            pbody->ctype = ColliderType::PLAYER;
            die = false;
            dieAnim.Reset();
        }
    }
    else
    {
        velocity.x = 0; // Det�n al jugador cuando no se presiona ninguna tecla de movimiento

        pbody->body->SetLinearVelocity(velocity);
        b2Transform pbodyPos = pbody->body->GetTransform();

        position.x = METERS_TO_PIXELS(pbodyPos.p.x) - 11;
        position.y = METERS_TO_PIXELS(pbodyPos.p.y) - 15;
    }
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
        jumpCount = 0;
        LOG("Collision PLATFORM");
        break;
    case ColliderType::ITEM:
        LOG("Collision ITEM");
        app->audio->PlayFx(pickCoinFxId);
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
        break;
    default:
        break;
    }
}