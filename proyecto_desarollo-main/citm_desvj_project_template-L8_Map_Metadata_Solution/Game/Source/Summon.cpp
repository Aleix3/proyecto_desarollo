#include "Summon.h"
#include "App.h"
#include "Textures.h"
#include "Physics.h"
#include "Scene.h"
#include "Render.h"
#include "Boss.h"
#include "Map.h"
#include "Hud.h"
#include "Log.h"
#include "Audio.h"

Summon::Summon() : Entity(EntityType::SUMMON) {
    name.Create("Summon");
    // Definición de las animaciones
    spawnAnim.PushBack({ 0, 0, 47, 25 });
    spawnAnim.PushBack({ 47, 0, 47, 25 });
    spawnAnim.PushBack({ 94, 0, 47, 25 });
    spawnAnim.PushBack({ 141, 0, 47, 25 });
    spawnAnim.PushBack({ 188, 0, 47, 25 });
    spawnAnim.speed = 0.05f;
    spawnAnim.loop = false;

    chasingAnim.PushBack({ 0, 50, 47, 25 });
    chasingAnim.PushBack({ 47, 50, 47, 25 });
    chasingAnim.PushBack({ 94, 50, 47, 25 });
    chasingAnim.PushBack({ 141, 50, 47, 25 });
    chasingAnim.speed = 0.10f;

    dyingAnim.PushBack({ 0, 25, 47, 25 });
    dyingAnim.PushBack({ 47, 25, 47, 25 });
    dyingAnim.PushBack({ 94, 25, 47, 25 });
    dyingAnim.speed = 0.17f;
    dyingAnim.loop = false;

    ultimo_uso = std::chrono::steady_clock::now();
}

Summon::~Summon() {

}

bool Summon::Awake() {
    // Inicialización de la posición de la entidad
    position.x = parameters.attribute("x").as_int();
    position.y = parameters.attribute("y").as_int();
	return true;
}

bool Summon::Start() {
    // Carga de la textura, los sonidos y creación del cuerpo físico
    texture = app->tex->Load(parameters.attribute("texturePath").as_string());
    app->tex->GetSize(texture, texW, texH);
    spawnfx = app->audio->LoadFx("Assets/Audio/Fx/Spawnfx.wav");
    pbody = app->physics->CreateCircle(position.x, position.y, 15, bodyType::DYNAMIC);
    pbody->listener = this;
    pbody->ctype = ColliderType::ENEMY;
    currentState = SummonState::IDLE;
    currentAnimation = &spawnAnim;
	return true;
}

bool Summon::Update(float dt) {
    // Limpia el último camino calculado por el sistema de pathfinding
    app->map->pathfinding->ClearLastPath();
    // Obtén y resetea la velocidad
    velocity2 = pbody->body->GetLinearVelocity();
    velocity2.x = 0.0;

    // Lógica para manejar el comportamiento después de la animación de aparición o si ya ha aparecido
    if (spawnAnim.HasFinished() || hasspawned == true)
    {
        hasspawned = true;// Indica que la entidad ya ha aparecido
        // Lógica para la persecución del jugador
        if (app->scene->GetPlayer()->position.x > position.x)
        {
            faceleft = false; // Establece la orientación de la entidad

            // Perseguir
            // Condición para iniciar la persecución
            if (app->scene->GetPlayer()->position.x < position.x + 200 && abs(app->scene->GetPlayer()->position.y - position.y) < 50)
            {
                currentState = SummonState::CHASING;

                // Conversión de posiciones del jugador y la entidad a coordenadas del mapa
                iPoint playerMap = app->map->WorldToMap(app->scene->GetPlayer()->position.x, app->scene->GetPlayer()->position.y);
                iPoint origin = app->map->WorldToMap(position.x, position.y);

                // Crea un path desde la posición actual hasta la del jugador
                app->map->pathfinding->CreatePath(origin, playerMap);

                // Verifica si se ha obtenido un camino válido
                if (app->map->pathfinding != NULL)
                {
                    const DynArray<iPoint>* pathCopy = app->map->pathfinding->GetLastPath();
                    {
                        if (pathCopy->Count() > 0)
                        {
                            const iPoint* nextPointPtr = pathCopy->At(0);
                            {
                                iPoint nextPoint = *nextPointPtr;
                                // Calcula la próxima posición en el mundo basada en el camino
                                iPoint nextPos = app->map->MapToWorld(nextPoint.x, nextPoint.y);

                                // Lógica para actualizar la velocidad y la animación basada en la posición del jugador
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

        // Lógica similar para cuando el jugador está a la izquierda del Summon
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
    }
   
    // Lógica para manejar la muerte del jugador
    if (app->scene->GetPlayer()->die == true)
    {
        app->physics->DestroyBody(pbody);// Destruye el cuerpo físico actual
        // Reinicializa la posición del summon
        position.x = parameters.attribute("x").as_int();
        position.y = parameters.attribute("y").as_int();

        // Crea un nuevo cuerpo físico
        pbody = app->physics->CreateCircle(position.x, position.y, 15, bodyType::DYNAMIC);
        pbody->listener = this;
        pbody->ctype = ColliderType::ENEMY;
    }

    // Lógica para manejar la muerte de la entidad Summon
    if (die == true)
    {
        app->physics->DestroyBody(pbody); // Destruye el cuerpo físico actual

        position.x = -1000;
        position.y = -1000;
        pbody = app->physics->CreateCircle(position.x, position.y, 15, bodyType::DYNAMIC);
        pbody->listener = this;
        pbody->ctype = ColliderType::ENEMY;
        die = false;
    }
    else
    {
        // Actualiza la posición del cuerpo físico si aún existe
        if (pbody != nullptr)
        {
            pbody->body->SetLinearVelocity(velocity2);
            b2Transform pbodyPos = pbody->body->GetTransform();

            // Actualiza la posición en el mundo basada en la del cuerpo físico
            position.x = METERS_TO_PIXELS(pbodyPos.p.x) - 11;
            position.y = METERS_TO_PIXELS(pbodyPos.p.y) - 11;
        }

    }

    // Actualiza la animación actual y dibuja la entidad
    currentAnimation->Update();
    SDL_Rect rect = currentAnimation->GetCurrentFrame();
    app->render->DrawTexture2(texture, position.x, position.y, SDL_FLIP_NONE, &rect);

	return true;
}

bool Summon::CleanUp() {
    // Limpieza de recursos al destruir el Summon
    app->tex->UnLoad(texture);
	return true;
}

void Summon::OnCollision(PhysBody* physA, PhysBody* physB) {
    // Manejo de colisiones de los Summons con otros objetos
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
    //Se crean los Summons
    if (pbody != nullptr)
    {

    }
    app->audio->PlayFx(spawnfx);
    position.x = 10336;
    position.y = 864;
    app->physics->DestroyBody(pbody);
    pbody = app->physics->CreateCircle(position.x, position.y, 15, bodyType::DYNAMIC);
    pbody->listener = this;
    pbody->ctype = ColliderType::ENEMY;

}
