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
#include "Pathfinding.h"

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
     
    walkAnim.PushBack({ 18,  274,  26, 27 });
    walkAnim.PushBack({ 19,  338,  25, 27 });
    walkAnim.PushBack({ 81,  274,  26, 27 });
    walkAnim.PushBack({ 83,  338,  24, 27 });
    walkAnim.PushBack({ 146, 274,  25, 27 });
    walkAnim.PushBack({ 148, 338,  23, 27 });
    walkAnim.PushBack({ 210, 274,  26, 27 });
    walkAnim.PushBack({ 212, 338,  24, 27 });
    walkAnim.speed = 0.035f; 

    dieAnim.PushBack({ 20,  469, 22, 27 });
    dieAnim.PushBack({ 84,  470, 23, 27 });
    dieAnim.PushBack({ 148, 469, 24, 27 });
    dieAnim.PushBack({ 212, 469, 24, 27 });
    dieAnim.PushBack({ 276, 469, 24, 27 });
    dieAnim.PushBack({ 340 , 469, 24, 27 });
    dieAnim.PushBack({ 404, 469, 24, 27 });
    dieAnim.PushBack({ 468 , 469, 24, 27 });
    dieAnim.PushBack({ 532, 469, 24, 27 });
    dieAnim.PushBack({ 596, 469, 24, 27 });
    dieAnim.PushBack({ 660 , 469, 24, 27 });
    dieAnim.PushBack({ 725 , 469, 24, 27 });
    dieAnim.PushBack({ 789, 469, 23, 27 });
    dieAnim.PushBack({ 853, 469, 23, 27 });
    dieAnim.speed = 0.085f;
    dieAnim.loop = false;

    attackAnim.PushBack({ 20, 213, 40, 27 });
    attackAnim.PushBack({ 83, 214, 43, 26 });
    attackAnim.PushBack({ 147, 214, 45, 26 });
    attackAnim.PushBack({ 215, 20, 18, 27 });
    attackAnim.speed = 0.09f;
    
    //Animacion de ataque cuando el enemigo esta atras suya
    attackbAnim.PushBack({ 20, 151, 24, 25 });
    attackbAnim.PushBack({ 77, 153, 47, 23 });
    attackbAnim.PushBack({ 136, 152, 55, 24 });
    attackbAnim.PushBack({ 200, 152, 56, 24 });
    attackbAnim.speed = 0.09f;
    attackbAnim.loop = false;
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
    currentState = EnemyState::SEARCHING;
    app->map->pathfinding->ClearLastPath();
    velocity2 = pbody->body->GetLinearVelocity();
    velocity2.x = 0.0;

    if (app->scene->GetPlayer()->position.x > position.x)
    {
        faceleft = false;

        // Perseguir
        if (app->scene->GetPlayer()->position.x < position.x + 200 && abs(app->scene->GetPlayer()->position.y - position.y) < 50)
        {
            currentState = EnemyState::CHASING;

            iPoint playerMap = app->map->WorldToMap(app->scene->GetPlayer()->position.x, app->scene->GetPlayer()->position.y);
            iPoint origin = app->map->WorldToMap(position.x, position.y);
            app->map->pathfinding->CreatePath(origin, playerMap);

            if (app->map->pathfinding != NULL)
            {
                const DynArray<iPoint>* pathCopy = app->map->pathfinding->GetLastPath();
                {
                    if (pathCopy->Count() > 0)
                    {
                        const iPoint* nextPointPtr = pathCopy->At(0);
                        {
                            iPoint nextPoint = *nextPointPtr;

                            iPoint nextPos = app->map->MapToWorld(nextPoint.x, nextPoint.y);

                            if (position.x + 20 < app->scene->GetPlayer()->position.x)
                            {
                                currentAnimation = &walkAnim;
                                velocity2.x = 0.2 * dt;
                            }
                        }
                    }
                }
            }

            // Atacar
            if (app->scene->GetPlayer()->position.x < position.x + 30)
            {
                currentState = EnemyState::ATACKING;
                currentAnimation = &attackAnim;
            }
        }
    }

    if (app->scene->GetPlayer()->position.x < position.x)
    {
        faceleft = true;

        // Perseguir
        if (app->scene->GetPlayer()->position.x > position.x - 200 && abs(app->scene->GetPlayer()->position.y - position.y) < 50)
        {
            currentState = EnemyState::CHASING;

            iPoint playerMap = app->map->WorldToMap(app->scene->GetPlayer()->position.x, app->scene->GetPlayer()->position.y);
            iPoint origin = app->map->WorldToMap(position.x, position.y);
            app->map->pathfinding->CreatePath(origin, playerMap);

            if (app->map->pathfinding != NULL)
            {
                const DynArray<iPoint>* pathCopy = app->map->pathfinding->GetLastPath();
                {
                    if (pathCopy->Count() > 0)
                    {
                        const iPoint* nextPointPtr = pathCopy->At(0);
                        {
                            iPoint nextPoint = *nextPointPtr;

                            iPoint nextPos = app->map->MapToWorld(nextPoint.x, nextPoint.y);

                            if (position.x > app->scene->GetPlayer()->position.x + 20)
                            {
                                currentAnimation = &walkAnim;
                                velocity2.x = -0.2 * dt;

                            }
                        }
                    }
                }
            }

            // Atacar
            if (app->scene->GetPlayer()->position.x > position.x - 30)
            {
                
                    currentState = EnemyState::ATACKING;
                    currentAnimation = &attackAnim;
                
                
            }
        }
    }

    if (currentState == EnemyState::ATACKING && currentAnimation->HasFinished() && (app->scene->GetPlayer()->die = false))
    {
        currentAnimation->Reset();
    }



    if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
        velocity2.x = -0.2 * dt;
        
    }

    if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
        velocity2.x = 0.2 * dt;

    }

    
    if (die == true)
    {
        position.x = 700;
        position.y = 690;
        app->physics->DestroyBody(pbody);
        pbody = app->physics->CreateCircle(position.x, position.y, 15, bodyType::DYNAMIC);
        pbody->listener = this;
        pbody->ctype = ColliderType::ENEMY;
        die = false;
    }

    else
    {
        pbody->body->SetLinearVelocity(velocity2);
        b2Transform pbodyPos = pbody->body->GetTransform();

        position.x = METERS_TO_PIXELS(pbodyPos.p.x) - 11;
        position.y = METERS_TO_PIXELS(pbodyPos.p.y) - 11;
    }
    

    currentAnimation->Update();

    SDL_Rect rect = currentAnimation->GetCurrentFrame();


    
    if (faceleft) {
        app->render->DrawTexture2(texture, position.x, position.y, SDL_FLIP_HORIZONTAL, &rect);
    }
    else {
        app->render->DrawTexture2(texture, position.x, position.y, SDL_FLIP_NONE, &rect);
    }
    

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
        currentState = EnemyState::DYING;
        break;
    default:
        break;
    }
}