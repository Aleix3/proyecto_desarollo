#ifndef __SUMMON_H__
#define __SUMMON_H__

#include "Entity.h"
#include "Animation.h"
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

public:
    SummonState currentState;
    pugi::xml_node config;

    SDL_Texture* texture = NULL;
    uint texW, texH;
    uint texWl, texHl;

    Animation* currentAnimation = nullptr;
    Animation idleAnim;
    Animation chasingAnim;
    Animation dyingAnim;

    uint attackFx;
    uint dieFx;

    PhysBody* pbody;

    std::chrono::steady_clock::time_point ultimo_uso;
};

#endif // __SUMMON_H__
