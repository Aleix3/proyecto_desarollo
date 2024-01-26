#ifndef __SUMMON_H__
#define __SUMMON_H__

#include "Entity.h"
#include "Animation.h"
#include "Physics.h"
#include "SDL/include/SDL.h"
#include <chrono>

struct SDL_Texture;

enum class SummonState {
    IDLE,
    CHASING,
    DYING
};

class Summon : public Entity {
public:
    Summon();
    virtual ~Summon();

    bool Awake();
    bool Start();
    bool Update(float dt);
    bool CleanUp();
    void OnCollision(PhysBody* physA, PhysBody* physB);
    void spawn();

public:
    SummonState currentState;
    pugi::xml_node config;

    SDL_Texture* texture = NULL;
    uint texW, texH;
    uint texWl, texHl;

    Animation* currentAnimation = nullptr;
    Animation spawnAnim;
    Animation chasingAnim;
    Animation dyingAnim;

    uint attackFx;
    uint dieFx;

    PhysBody* pbody;
    b2Vec2 velocity2;
    bool faceleft;
    bool die = false;
    bool hasspawned = false;

    std::chrono::steady_clock::time_point ultimo_uso;
};

#endif // __SUMMON_H__
