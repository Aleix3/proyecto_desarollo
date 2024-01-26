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

    Animation* currentAnimation = nullptr;
    Animation idleAnim;
    Animation spawnAnim;
    Animation attackAnim;
    Animation damageAnim;

    uint attackFx;
    uint dieFx;

    PhysBody* pbody;

    Summon* bicho;

    std::chrono::steady_clock::time_point ultimo_uso;

    float distance;
    float spawnDistance = 200;
    int spawnCooldown = 6.0f;
    bool faceleft;
    bool die;
    bool comprovacionAnim;
    float scaleFactor = 1.2f;

    int vida;

    b2Vec2 velocity2;

    EnemySamurai* enemySamurai;
};

#endif // __BOSS_H__
