#include "Summon.h"
#include "App.h"
#include "Textures.h"
#include "Physics.h"
#include "Scene.h"
#include "Boss.h"
#include "Map.h"
#include "Hud.h"
#include "Log.h"

Summon::Summon() : Entity(EntityType::SUMMON) {
    name.Create("Summon");

    spawnAnim.PushBack({ 0, 0, 47, 25 });
    spawnAnim.PushBack({ 47, 0, 47, 25 });
    spawnAnim.PushBack({ 94, 0, 47, 25 });
    spawnAnim.PushBack({ 141, 0, 47, 25 });
    spawnAnim.PushBack({ 188, 0, 47, 25 });
    spawnAnim.PushBack({ 235, 0, 47, 25 });
    spawnAnim.speed = 0.10f;

    chasingAnim.PushBack({ 0, 0, 47, 25 });
    chasingAnim.PushBack({ 47, 0, 47, 25 });
    chasingAnim.PushBack({ 94, 0, 47, 25 });
    chasingAnim.PushBack({ 141, 0, 47, 25 });
    chasingAnim.speed = 0.10f;

    dyingAnim.PushBack({ 0, 50, 47, 25 });
    dyingAnim.PushBack({ 47, 50, 47, 25 });
    dyingAnim.PushBack({ 94, 50, 47, 25 });
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
    currentAnimation = &spawnAnim;
    currentState = SummonState::IDLE;
    app->map->pathfinding->ClearLastPath();
    if (pbody != nullptr)
    velocity2 = pbody->body->GetLinearVelocity();
    velocity2.x = 0.0;

    if (app->scene->GetPlayer()->position.x > position.x)
    {
        faceleft = false;

        // Perseguir
        if (app->scene->GetPlayer()->position.x < position.x + 200 && abs(app->scene->GetPlayer()->position.y - position.y) < 50)
        {
            currentState = SummonState::CHASING;

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
                                currentAnimation = &chasingAnim;
                                velocity2.x = 0.1 * dt;
                            }
                        }
                    }
                }
            }
        }
    }

    if (app->scene->GetPlayer()->position.x < position.x)
    {
        faceleft = true;

        // Perseguir
        if (app->scene->GetPlayer()->position.x > position.x - 200 && abs(app->scene->GetPlayer()->position.y - position.y) < 50)
        {
            currentState = SummonState::CHASING;

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
                                currentAnimation = &chasingAnim;
                                velocity2.x = -0.1 * dt;

                            }
                        }
                    }
                }
            }
        }
    }

    if (app->scene->GetPlayer()->die == true)
    {
        app->physics->DestroyBody(pbody);
        position.x = parameters.attribute("x").as_int();
        position.y = parameters.attribute("y").as_int();

        pbody = app->physics->CreateCircle(position.x, position.y, 15, bodyType::DYNAMIC);
        pbody->listener = this;
        pbody->ctype = ColliderType::ENEMY;
    }
    if (die == true)
    {
        app->physics->DestroyBody(pbody);

        position.x = -1000;
        position.y = -1000;
        pbody = app->physics->CreateCircle(position.x, position.y, 15, bodyType::DYNAMIC);
        pbody->listener = this;
        pbody->ctype = ColliderType::ENEMY;
        die = false;
    }
    else
    {
        if (pbody != nullptr)
        {
            pbody->body->SetLinearVelocity(velocity2);
            b2Transform pbodyPos = pbody->body->GetTransform();

            position.x = METERS_TO_PIXELS(pbodyPos.p.x) - 11;
            position.y = METERS_TO_PIXELS(pbodyPos.p.y) - 11;
        }

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

bool Summon::CleanUp() {
    app->tex->UnLoad(texture);
	return true;
}

void Summon::OnCollision(PhysBody* physA, PhysBody* physB) {
    switch (physB->ctype)
    {
    case ColliderType::PLATFORM:
        LOG("Collision PLATFORM");
        break;
    case ColliderType::DIE:
        LOG("Collision DIE");
        die = true;
        app->hud->puntos += 50;
        break;
    case ColliderType::ABILITY:
        LOG("Collision ABILITY");
        die = true;
        app->hud->puntos += 50;
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
