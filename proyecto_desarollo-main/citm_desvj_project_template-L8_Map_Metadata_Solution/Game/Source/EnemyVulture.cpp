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
#include "EnemyVulture.h"
#include "Ability.h"


EnemyVulture::EnemyVulture() : Enemy()
{
    name.Create("EnemyVulture");

    idleAnim.PushBack({ 0, 0, 32, 32 });
    idleAnim.PushBack({ 32, 0, 32, 32 });
    idleAnim.PushBack({ 64, 0, 32, 32 });
    idleAnim.PushBack({ 96, 0, 32, 32 });
    idleAnim.PushBack({ 128, 0, 32, 32 });
    idleAnim.speed = 0.05f;
    //idleAnim.PushBack({ 160, 0, 32, 32 });
    //idleAnim.PushBack({ 192, 0, 32, 32 });
    //idleAnim.PushBack({ 224, 0, 32, 32 });

    flyAnim.PushBack({ 0, 0, 32, 32 });
    flyAnim.PushBack({ 32, 0, 32, 32 });
    flyAnim.PushBack({ 64, 0, 32, 32 });
    flyAnim.PushBack({ 96, 0, 32, 32 });
    flyAnim.PushBack({ 128, 0, 32, 32 });
    flyAnim.PushBack({ 160, 0, 32, 32 });
    flyAnim.PushBack({ 192, 0, 32, 32 });
    flyAnim.PushBack({ 224, 0, 32, 32 });
    flyAnim.speed = 0.05f;
} 
 
EnemyVulture::~EnemyVulture() {

}

bool EnemyVulture::Awake() {

    position.x = parameters.attribute("x").as_int();
    position.y = parameters.attribute("y").as_int();

    return true;
}

bool EnemyVulture::Start() {

    texture = app->tex->Load(parameters.attribute("texturePath").as_string());
    app->tex->GetSize(texture, texW, texH);

    pbody = app->physics->CreateCircle(position.x, position.y, 15, bodyType::DYNAMIC);
    pbody->listener = this;
    pbody->ctype = ColliderType::ENEMY;
    pbody->body->SetGravityScale(0.0f);
    return true;
}


bool EnemyVulture::Update(float dt) {

    iPoint origin = app->map->WorldToMap(position.x, position.y);

    if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) //Aqui meter la distancia del enemy al player
    {
        app->map->pathfinding->CreatePath(origin, app->scene->playerMap);

    }

    const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();

    for (uint i = 0; i < path->Count(); ++i)
    {
        iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
        app->render->DrawTexture(app->scene->mouseTileTex, pos.x, pos.y);
    }

    currentAnimation = &idleAnim;

    currentState = EnemyState::SEARCHING;
    app->map->pathfinding->ClearLastPath();
    velocity2 = pbody->body->GetLinearVelocity();
    velocity2.x = 0.0;

    if (app->scene->GetPlayer()->position.x > position.x)
    {
        faceleft = false;

        // Perseguir
        if (app->scene->GetPlayer()->position.x < position.x + 200 && app->scene->GetPlayer()->position.y < position.y + 200 && app->scene->GetPlayer()->position.y > position.y - 200)
        {
            currentState = EnemyState::CHASING;

            iPoint playerMap = app->map->WorldToMap(app->scene->GetPlayer()->position.x, app->scene->GetPlayer()->position.y);
            iPoint origin = app->map->WorldToMap(position.x, position.y);
            app->map->pathfinding->CreatePath(origin, playerMap);

            if (app->map->pathfinding != NULL)
            {
                const DynArray<iPoint>* pathCopy = app->map->pathfinding->GetLastPath();
                for (int i = 0; i < pathCopy->Count(); ++i)
                {
                    const iPoint* nextPointPtr = pathCopy->At(i);
                    iPoint nextPoint = *nextPointPtr;
                    iPoint nextPos = app->map->MapToWorld(nextPoint.x, nextPoint.y);

                    float directionVectorx = (nextPos.x - position.x);
                    float directionVectory = (nextPos.y - position.y);

                    float distance = sqrtf(static_cast<float>(directionVectorx * directionVectorx + directionVectory * directionVectory));

                    if (distance > 0)
                    {
                        directionVectorx /= distance;
                        directionVectory /= distance;


                        float constantSpeed = 0.1f;


                        velocity2.x = directionVectorx * 0.1 * dt;
                        velocity2.y = directionVectory * 0.1 * dt;


                        currentAnimation = &flyAnim;
                    }
                }
            }

            // Atacar
            /*if (app->scene->GetPlayer()->position.x < position.x + 30)
            {
                currentState = EnemyState::ATACKING;
                currentAnimation = &attackAnim;
            }*/
        }
    }

    if (app->scene->GetPlayer()->position.x < position.x)
    {
        faceleft = true;

        // Perseguir
        if (app->scene->GetPlayer()->position.x > position.x - 200 && app->scene->GetPlayer()->position.y < position.y + 200 && app->scene->GetPlayer()->position.y > position.y - 200)
        {
            currentState = EnemyState::CHASING;

            iPoint playerMap = app->map->WorldToMap(app->scene->GetPlayer()->position.x, app->scene->GetPlayer()->position.y);
            iPoint origin = app->map->WorldToMap(position.x, position.y);
            app->map->pathfinding->CreatePath(origin, playerMap);

            if (app->map->pathfinding != NULL)
            {
                const DynArray<iPoint>* pathCopy = app->map->pathfinding->GetLastPath();
                for (int i = 0; i < pathCopy->Count(); ++i)
                {
                    const iPoint* nextPointPtr = pathCopy->At(i);
                    iPoint nextPoint = *nextPointPtr;
                    iPoint nextPos = app->map->MapToWorld(nextPoint.x, nextPoint.y);

                    

                    float directionVectorx = (nextPos.x - position.x);
                    float directionVectory = (nextPos.y - position.y);

                    float distance = sqrtf(static_cast<float>(directionVectorx * directionVectorx + directionVectory * directionVectory));

                    if (distance > 0)
                    {
                        directionVectorx /= distance;
                        directionVectory /= distance;
                           
                        
                        float constantSpeed = 0.1f;

                        
                        position.y += directionVectory * constantSpeed * dt;
                    }

                    
                     velocity2.x = directionVectorx * 0.1 * dt;
                     velocity2.y = directionVectory * 0.1 * dt;

                   
                    currentAnimation = &flyAnim;
                }
            }
            
            

            // Atacar
            /*if (app->scene->GetPlayer()->position.x > position.x - 30)
            {
                
                    currentState = EnemyState::ATACKING;
                    currentAnimation = &attackAnim;
                
                
            }*/
        }
    }

    /*if (currentState == EnemyState::ATACKING && currentAnimation->HasFinished() && (app->scene->GetPlayer()->die = false))
    {
        currentAnimation->Reset();
    }*/



    if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
        velocity2.x = -0.2 * dt;
        
    }

    if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
        velocity2.x = 0.2 * dt;

    }

    
    
    if (die == true)
    {
        position.x = parameters.attribute("x").as_int();
        position.y = parameters.attribute("y").as_int();
        app->physics->DestroyBody(pbody);
        pbody = app->physics->CreateCircle(position.x, position.y, 15, bodyType::DYNAMIC);
        pbody->listener = this;
        pbody->ctype = ColliderType::ENEMY;
        pbody->body->SetGravityScale(0.0f);
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
        app->render->DrawTexture2(texture, position.x, position.y, SDL_FLIP_NONE, &rect);
    }
    else {
        
        app->render->DrawTexture2(texture, position.x, position.y, SDL_FLIP_HORIZONTAL, &rect);
    }
    
    
    return true;
}

bool EnemyVulture::CleanUp()
{
    app->tex->UnLoad(texture);
    return true;
}

void EnemyVulture::OnCollision(PhysBody* physA, PhysBody* physB) {
    switch (physB->ctype)
    {
    case ColliderType::PLATFORM:
        LOG("Collision PLATFORM");
        break;
    case ColliderType::DIE:
        LOG("Collision DIE");
        die = true;
        break;
    case ColliderType::ABILITY:
        LOG("Collision ABILITY");
        die = true;
        break;
    case ColliderType::PLAYER:
        LOG("Collision PLAYER");
        app->scene->GetPlayer()->die = true;
        break;
    case ColliderType::UNKNOWN:
        LOG("Collision UNKNOWN");
        break;
    default:
        break;
    }
}