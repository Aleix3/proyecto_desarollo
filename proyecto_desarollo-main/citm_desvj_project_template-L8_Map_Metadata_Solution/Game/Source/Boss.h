#ifndef __BOSS_H__
#define __BOSS_H__

#include "Entity.h"
#include "Animation.h"
#include "Physics.h"
#include "Summon.h"
#include "SDL/include/SDL.h"
#include <chrono>
#include "EnemySamurai.h"

struct SDL_Texture;

enum class BossState {
    IDLE,
    ATACKING,
    SPAWNING,
    DAMAGE,
    DYING
};

class Boss : public Entity {
public:
    Boss();
    virtual ~Boss();

    bool Awake();
    bool Start();
    bool Update(float dt);
    bool CleanUp();
    void OnCollision(PhysBody* physA, PhysBody* physB);
    bool Cooldown(float cooldown);
    void Morir();
    void SpawnBicho();
    float DistanceToPlayer();

public:
    BossState currentState;
    pugi::xml_node config;

    SDL_Texture* texture = NULL;
    uint texW, texH;
    uint texWl, texHl;
    uint golpefx = 0;
    uint espadafx = 0;

    Animation* currentAnimation = nullptr;
    Animation idleAnim;
    Animation spawnAnim;
    Animation attackAnim;
    Animation damageAnim;

    uint attackFx;
    uint dieFx;

    PhysBody* pbody;// Cuerpo f�sico para la f�sica


    Summon* bicho;

    std::chrono::steady_clock::time_point ultimo_uso;

    float distance;
    float spawnDistance = 200; // Distancia para invocar criaturas
    int spawnCooldown = 5.5f; // Tiempo de cooldown para invocaci�n
    bool faceleft; // Indica si el jefe est� mirando a la izquierda
    bool die; // Indica si el jefe debe morir
    bool comprovacionAnim; // Verificaci�n para la animaci�n

    int vida;

    b2Vec2 velocity2;

    EnemySamurai* enemySamurai;
};

#endif // __BOSS_H__
