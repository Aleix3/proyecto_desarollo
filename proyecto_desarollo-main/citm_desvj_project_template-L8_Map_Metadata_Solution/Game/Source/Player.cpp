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
}

Player::~Player() {

}

bool Player::Awake() {

	//L03: DONE 2: Initialize Player padsrameters
	position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());
    Animation idleAnimation;
    Animation jumpAnimation;
    Animation dieAnimation;

    SDL_Rect idleFrame1 = { 5, 5, 24, 27 };
    SDL_Rect idleFrame2 = { 28, 33, 24, 27 };

    idleAnimation.PushBack(idleFrame1);
    idleAnimation.PushBack(idleFrame2);

    idleAnimation.speed = 0.2f;
    idleAnimation.loop = true;
    animations[PlayerState::IDLE] = idleAnimation;

    currentState = PlayerState::IDLE;

	return true;
}

bool Player::Start() {

	texture = app->tex->Load(config.attribute("texturePath").as_string());
	// L07 TODO 5: Add physics to the player - initialize physics body
	app->tex->GetSize(texture, texW, texH);
	pbody = app->physics->CreateCircle(position.x, position.y, texW / 2, bodyType::DYNAMIC);

	// L07 TODO 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// L07 TODO 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	//initialize audio effect
	//pickCoinFxId = app->audio->LoadFx(config.attribute("coinfxpath").as_string());

	return true;
}

bool Player::Update(float dt) {

    b2Vec2 velocity = pbody->body->GetLinearVelocity();

    if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
        velocity.x = -0.2 * dt;
    }
    else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
        velocity.x = 0.2 * dt;
    }
    else {
        velocity.x = 0; // Detén al jugador cuando no se presiona ninguna tecla de movimiento
    }

    if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && jumpCount < 2)//&& isOnGround) {
    {
        jumpCount++;
        velocity.y = -jumpSpeed;
        pbody->body->SetLinearVelocity(velocity);
    }

    if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
    {
        velocity.x = -dashSpeed;
    }

    if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
    {
        velocity.x = dashSpeed;
    }

    pbody->body->SetLinearVelocity(velocity);
    b2Transform pbodyPos = pbody->body->GetTransform();
    position.x = METERS_TO_PIXELS(pbodyPos.p.x) - texW / 2;
    position.y = METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2;

    Animation& currentAnimation = animations[currentState];
    currentAnimation.Update();
    SDL_Rect& currentFrame = currentAnimation.GetCurrentFrame();
    app->render->DrawTexture(texture, position.x, position.y, &currentFrame);

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