#include "Boss.h"
#include "App.h"
#include "Textures.h"
#include "Physics.h"
#include "Log.h"
#include "Scene.h"
#include "Hud.h"

Boss::Boss() : Entity(EntityType::BOSS) {
    name.Create("Boss");

    idleAnim.PushBack({ 0, 700, 100, 100 });
    idleAnim.PushBack({ 100, 700, 100, 100 });
    idleAnim.PushBack({ 200, 700, 100, 100 });
    idleAnim.PushBack({ 300, 700, 100, 100 });
    idleAnim.PushBack({ 0, 800, 100, 100 });
    idleAnim.PushBack({ 100, 800, 100, 100 });
    idleAnim.PushBack({ 200, 800, 100, 100 });
    idleAnim.PushBack({ 300, 800, 100, 100 });
    idleAnim.PushBack({ 0, 900, 100, 100 });
    idleAnim.PushBack({ 100, 900, 100, 100 });
    idleAnim.PushBack({ 200, 900, 100, 100 });
    idleAnim.PushBack({ 300, 900, 100, 100 });
    idleAnim.PushBack({ 0, 1000, 100, 100 });
    idleAnim.speed = 0.05f;

    damageAnim.PushBack({ 100, 100, 100, 100 });
    damageAnim.PushBack({ 200, 100, 100, 100 });
    damageAnim.PushBack({ 300, 100, 100, 100 });
    damageAnim.PushBack({ 400, 100, 100, 100 });
    damageAnim.speed = 0.15f;

    attackAnim.PushBack({ 0, 200, 100, 100 });
    attackAnim.PushBack({ 100, 200, 100, 100 });
    attackAnim.PushBack({ 200, 200, 100, 100 });
    attackAnim.PushBack({ 300, 200, 100, 100 });
    attackAnim.PushBack({ 400, 200, 100, 100 });
    attackAnim.PushBack({ 500, 200, 100, 100 });
    attackAnim.speed = 0.1f;

    spawnAnim.PushBack({ 0, 1000, 100, 100 });
    spawnAnim.PushBack({ 100, 1000, 100, 100 });
    spawnAnim.PushBack({ 200, 1000, 100, 100 });
    spawnAnim.PushBack({ 300, 1000, 100, 100 });
    spawnAnim.PushBack({ 0, 1100, 100, 100 });
    spawnAnim.speed = 0.05f;

    ultimo_uso = std::chrono::steady_clock::now();
}

Boss::~Boss() {

}

bool Boss::Awake() {

    position.x = parameters.attribute("x").as_int();
    position.y = parameters.attribute("y").as_int();
    vida = 3;
	return true;
}

bool Boss::Start() {
    texture = app->tex->Load(parameters.attribute("texturePath").as_string());
    app->tex->GetSize(texture, texW, texH);

    pbody = app->physics->CreateCircle(position.x, position.y, 20, bodyType::STATIC);
    pbody->listener = this;
    pbody->ctype = ColliderType::BOSS;
	return true;
}

bool Boss::Update(float dt) {

	// El boss no se movera, sequedará quieto y irá spwaneando varios bichos cada cierto tiempo. Los bichos seguirán al player con el path
	// Si el player se acerca mucho al Boss, el Boss atacará con su guadaña. El Boss tiene una cierta cantidad de vida
    
    if (pbody != nullptr)
    {
        currentAnimation = &idleAnim;

        velocity2 = pbody->body->GetLinearVelocity();
        velocity2.x = 0.0;


        if (vida <= 0)
        {
            Morir();
            
        }

        distance = DistanceToPlayer();
        
        if (distance <= spawnDistance && Cooldown(spawnCooldown)) {
            SpawnBicho();
        }

        if (app->scene->GetPlayer()->position.x > position.x) {
            faceleft = false;
            if (app->scene->GetPlayer()->position.x < position.x + 100)
            {
                currentState = BossState::ATACKING;
                currentAnimation = &attackAnim;
            }
            
        }
        if (app->scene->GetPlayer()->position.x < position.x + 20)
        {
            faceleft = true;
            if (app->scene->GetPlayer()->position.x > position.x + 100)
            {
                currentState = BossState::ATACKING;
                currentAnimation = &attackAnim;
            }
        }

        else
        {
            if(pbody != nullptr)
            pbody->body->SetLinearVelocity(velocity2);
            b2Transform pbodyPos = pbody->body->GetTransform();
            position.x = METERS_TO_PIXELS(pbodyPos.p.x) - 55;
            position.y = METERS_TO_PIXELS(pbodyPos.p.y) - 70;
        }

        if (currentAnimation == &attackAnim && !comprovacionAnim && attackAnim.HasFinished()) {
            comprovacionAnim = true;
            currentAnimation = &idleAnim;
        }
    }
    
    currentAnimation->Update();

    SDL_Rect rect = currentAnimation->GetCurrentFrame();

    if (faceleft == false) {
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
        vida--;
        break;
    case ColliderType::ABILITY:
        LOG("Collision ABILITY");
        vida--;
        if (physA->ctype == ColliderType::ENEMY)
        {
            if (app->scene->GetEnemySamurai()->appear == false)
            {
                app->scene->GetEnemySamurai()->die = true;
            }
           
        }
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

bool Boss::Cooldown(float cooldown)
{
    auto ahora = std::chrono::steady_clock::now();
    float tiempo_transcurrido = std::chrono::duration<float>(ahora - ultimo_uso).count();

    if (tiempo_transcurrido >= cooldown) {
        ultimo_uso = ahora; // reiniciar el contador
        return true;
    }
    else {
        return false;
    }
}

void Boss::Morir()
{
    if (pbody != nullptr) {
        app->physics->DestroyBody(pbody);
        pbody = nullptr;
    }
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
    currentState = BossState::DYING;
    
}

void Boss::SpawnBicho() {
    
    currentAnimation = &spawnAnim;
    app->spawn = true;
    
    app->scene->GetSummon()->spawn();
 
    currentState = BossState::SPAWNING;
}

float Boss::DistanceToPlayer() {
    Point<int> playerPos = app->scene->GetPlayer()->position;
    Point<int> bossPos = position;

    int dx = playerPos.x - bossPos.x;
    int dy = playerPos.y - bossPos.y;

    return sqrt(dx * dx + dy * dy);
}
