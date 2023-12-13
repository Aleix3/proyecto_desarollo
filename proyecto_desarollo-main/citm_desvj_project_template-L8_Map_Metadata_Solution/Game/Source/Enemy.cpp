#include "Enemy.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "map.h"
#include "Pathfinding.h"

Enemy::Enemy() : Entity(EntityType::ENEMY)
{

} 
 
Enemy::~Enemy() {

}

bool Enemy::Awake() {

    return true;
}

bool Enemy::Start() {
    
    return true;
}


bool Enemy::Update(float dt) {

    return true;
}

bool Enemy::CleanUp()
{
    app->tex->UnLoad(texture);
    return true;
}

void Enemy::OnCollision(PhysBody* physA, PhysBody* physB) {

}