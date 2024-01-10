#ifndef __BOSS_H__
#define __BOSS_H__

#include "Entity.h"
#include "Animation.h"
#include "SDL/include/SDL.h"
#include <chrono>

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

public:

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

    std::chrono::steady_clock::time_point ultimo_uso;
};

#endif // __BOSS_H__
