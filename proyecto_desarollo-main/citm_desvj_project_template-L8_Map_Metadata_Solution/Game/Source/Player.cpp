#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");

    runAnim.PushBack({ 4, 36,  23, 28 });
    runAnim.PushBack({ 37,35,  22, 28 });
    runAnim.PushBack({ 68, 36, 23, 27 });
    runAnim.PushBack({ 101, 35, 22, 28 });
    runAnim.speed = 0.17f;

    idleAnim.PushBack({ 4, 4, 23, 27 });
    idleAnim.PushBack({ 37, 3, 22, 28 });
    idleAnim.speed = 0.05f;

    jumpAnim.PushBack({ 4, 131, 23, 28 });
    jumpAnim.PushBack({ 35, 132, 24, 27 });
    jumpAnim.PushBack({ 70, 130, 22, 29 });
    jumpAnim.PushBack({ 102, 128, 21, 30 });
    jumpAnim.PushBack({ 134, 128, 21, 28 });
    jumpAnim.PushBack({ 164, 129, 23, 28 });
    jumpAnim.PushBack({ 196, 130, 23, 29 });
    jumpAnim.PushBack({ 228, 132, 24, 27 });
    jumpAnim.PushBack({ 259, 132, 24, 27 });
    jumpAnim.PushBack({ 292, 132, 23, 27 });
    jumpAnim.speed = 0.15f; 


}

Player::~Player() {

}

bool Player::Awake() {

	//L03: DONE 2: Initialize Player padsrameters
	position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());
   

    

   

	return true;
}

bool Player::Start() {

	texture = app->tex->Load(config.attribute("texturePath").as_string());
	// L07 TODO 5: Add physics to the player - initialize physics body
	app->tex->GetSize(texture, texW, texH);
	pbody = app->physics->CreateCircle(position.x, position.y, 11, bodyType::DYNAMIC);

	// L07 TODO 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// L07 TODO 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	//initialize audio effect
	//pickCoinFxId = app->audio->LoadFx(config.attribute("coinfxpath").as_string());

	return true;
}
bool tocasuelo = true;

bool Player::Update(float dt) {


    currentAnimation = &idleAnim;

    b2Vec2 velocity = pbody->body->GetLinearVelocity();

    if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
        velocity.x = -0.2 * dt;
        currentAnimation = &runAnim;
        left = true;
    }
    else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
        velocity.x = 0.2 * dt;
        currentAnimation = &runAnim;
        left = false;
    }
    else {
        velocity.x = 0; // Detén al jugador cuando no se presiona ninguna tecla de movimiento
    }

    if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && jumpCount < 2)//&& isOnGround) {
    {
        jumpCount++;
        velocity.y = -jumpSpeed;
        pbody->body->SetLinearVelocity(velocity);
        currentAnimation = &jumpAnim;
        tocasuelo == false;
    }
    if (velocity.y < 0 && tocasuelo == true)
    {
        currentAnimation = &jumpAnim;
    }
    //if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
    //{
    //    velocity.x = -dashSpeed;
    //}

    //if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
    //{
    //    velocity.x = dashSpeed;
    //}

    

    pbody->body->SetLinearVelocity(velocity);
    b2Transform pbodyPos = pbody->body->GetTransform();

    position.x = METERS_TO_PIXELS(pbodyPos.p.x) - 11;
    position.y = METERS_TO_PIXELS(pbodyPos.p.y) - 15;

    
    currentAnimation->Update();

    SDL_Rect rect = currentAnimation->GetCurrentFrame();
    
    
        app->render->DrawTexture(texture, position.x, position.y, &rect);
    

    return true;
}

bool Player::CleanUp()
{
    return true;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {
    switch (physB->ctype)
    {
    case ColliderType::PLATFORM:
        jumpCount = 0;
        LOG("Collision PLATFORM");
        break;
    case ColliderType::ITEM:
        LOG("Collision ITEM");
        app->audio->PlayFx(pickCoinFxId);
        break;
    case ColliderType::UNKNOWN:
        LOG("Collision UNKNOWN");
        break;
    default:
        break;
    }
}



//void Player::EndContact(PhysBody* physA, PhysBody* physB) {
//    if (physB->ctype == ColliderType::PLATFORM) {
//        isOnGround = false; // El jugador ya no est� en el suelo
//    }
//}
//
//bool Player::IsOnGround()
//{
//    return isOnGround;
//}

//bool Player::IsJumping()
//{
//    return;
//}