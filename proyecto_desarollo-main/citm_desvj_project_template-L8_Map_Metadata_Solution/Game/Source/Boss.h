#ifndef __BOSS_H__
#define __BOSS_H__

#include "Entity.h"
#include "Animation.h"
#include "SDL/include/SDL.h"

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


    Animation attackAnim;
    Animation moveAnim;
    Animation dieAnim;


    uint attackFx;
    uint dieFx;
};

#endif // __BOSS_H__
