#include "Boss.h"
#include "App.h"
#include "Textures.h"
#include "Physics.h"
#include "Log.h"
#include "Scene.h"
#include "Hud.h"
#include "Audio.h"

Boss::Boss() : Entity(EntityType::BOSS) {
    name.Create("Boss");
    //animaciones
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
    // Carga de texturas y efectos de sonido, y configuración del cuerpo físico
    texture = app->tex->Load(parameters.attribute("texturePath").as_string());
    app->tex->GetSize(texture, texW, texH);
    golpefx = app->audio->LoadFx("Assets/Audio/Fx/Golpeghostfx.wav");
    espadafx = app->audio->LoadFx("Assets/Audio/Fx/Espadafx.wav");
    pbody = app->physics->CreateCircle(position.x, position.y, 20, bodyType::STATIC);
    pbody->listener = this;
    pbody->ctype = ColliderType::BOSS;
	return true;
}

bool Boss::Update(float dt) {

    if (pbody != nullptr) 
    {
        currentAnimation = &idleAnim; // Establece la animación actual

        velocity2 = pbody->body->GetLinearVelocity(); //Pone la velocidad del boss a 0
        velocity2.x = 0.0;

        if (vida <= 0)
        {
            Morir(); // Si la vida es inferior o igual a 0, muere
        }

        distance = DistanceToPlayer(); // se calcula la distancia del player y el boss
        
        if (distance <= spawnDistance && Cooldown(spawnCooldown)) {
            SpawnBicho(); // Se spawnea un bicho si se cumplen los requisitos
        }

        //Logica de ataque
        if (app->scene->GetPlayer()->position.x > position.x) {
            faceleft = false;
            if (app->scene->GetPlayer()->position.x < position.x + 100)
            {
                currentState = BossState::ATACKING;
                currentAnimation = &attackAnim;
                app->audio->PlayFx(espadafx);
            }
            
        }
        //Logica de ataque
        if (app->scene->GetPlayer()->position.x < position.x + 20)
        {
            faceleft = true;
            if (app->scene->GetPlayer()->position.x > position.x + 100)
            {
                currentState = BossState::ATACKING;
                currentAnimation = &attackAnim;
                app->audio->PlayFx(espadafx);
            }
        }

        else
        {
            if (pbody != nullptr)
            {
                pbody->body->SetLinearVelocity(velocity2);
                b2Transform pbodyPos = pbody->body->GetTransform();
                position.x = METERS_TO_PIXELS(pbodyPos.p.x) - 55;
                position.y = METERS_TO_PIXELS(pbodyPos.p.y) - 50;
            }
            
        }

        if (currentAnimation == &attackAnim && !comprovacionAnim && attackAnim.HasFinished()) {
            comprovacionAnim = true;
            currentAnimation = &idleAnim; // Cambia la animación a inactivo una vez finalizada la de ataque
        }
    }
    //render
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
    // Limpieza de recursos al destruir el boss
    app->tex->UnLoad(texture); // Descarga la textura del boss
	return true;
}

void Boss::OnCollision(PhysBody* physA, PhysBody* physB) {
    // Manejo de colisiones del boss con otros objetos
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
        app->audio->PlayFx(golpefx);
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
    // Control de tiempo para el cooldown de habilidades
    auto ahora = std::chrono::steady_clock::now();
    float tiempo_transcurrido = std::chrono::duration<float>(ahora - ultimo_uso).count();

    if (tiempo_transcurrido >= cooldown) {
        ultimo_uso = ahora;
        return true;
    }
    else {
        return false;
    }
}

void Boss::Morir()
{
    // Lógica para manejar la muerte del jefe
    if (pbody != nullptr) {
        app->physics->DestroyBody(pbody); // Destruye el cuerpo físico
        pbody = nullptr;
    }
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
    currentState = BossState::DYING;
    
}

void Boss::SpawnBicho() {
    // Método para hacer aparecer al summon
    currentAnimation = &spawnAnim;
    app->spawn = true;
    
    app->scene->GetSummon()->spawn();
 
    currentState = BossState::SPAWNING; // Cambia el estado del boss a "SPAWNING"
}

float Boss::DistanceToPlayer() {
    // Cálculo de la distancia al jugador
    Point<int> playerPos = app->scene->GetPlayer()->position;
    Point<int> bossPos = position;

    int dx = playerPos.x - bossPos.x;
    int dy = playerPos.y - bossPos.y;

    return sqrt(dx * dx + dy * dy);
}
