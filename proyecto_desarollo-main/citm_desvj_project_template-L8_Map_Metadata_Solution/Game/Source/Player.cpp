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
#include "map.h"
#include "Enemy.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");

    runAnim.PushBack({ 4, 36,  23, 28 });
    runAnim.PushBack({ 37,35,  22, 28 });
    runAnim.PushBack({ 68, 36, 23, 27 });
    runAnim.PushBack({ 101, 35, 22, 28 });
    runAnim.speed = 0.10f;

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

    dieAnim.PushBack({ 4, 196, 23, 27 });
    dieAnim.PushBack({ 37, 195, 32, 32 });
    dieAnim.PushBack({ 68, 196, 23, 27 });
    dieAnim.PushBack({ 100, 198, 23, 25 });
    dieAnim.PushBack({ 132, 199, 23, 24 });
    dieAnim.PushBack({ 164, 200, 24, 23 });
    dieAnim.PushBack({ 194, 202, 27, 21 });
    dieAnim.PushBack({ 226, 204, 28, 19 });
    dieAnim.PushBack({ 258, 205, 29, 18 });
    dieAnim.PushBack({ 290, 206, 29, 17 });
    dieAnim.speed = 0.17f;
    dieAnim.loop = false;

    dashAnim1.PushBack({ 4, 165, 23, 27 });
    dashAnim1.PushBack({ 36, 164, 23, 27 });
    dashAnim1.PushBack({ 70, 165, 20, 26 });
    dashAnim1.PushBack({ 105, 167, 17, 19 });

    dashAnim1.speed = 0.17f;
    dashAnim1.loop = false;

    dashAnim2.PushBack({ 137, 167, 18, 19 });
    dashAnim2.PushBack({ 166, 166, 21, 25 });
    dashAnim2.PushBack({ 196, 166, 26, 26 });
    dashAnim2.PushBack({ 228, 164, 23, 27 });
    dashAnim2.speed = 0.17f;
    dashAnim2.loop = false;

    fireballAnim.PushBack({ 384,480,32,32 });
    fireballAnim.PushBack({ 416,480,32,32 });
    fireballAnim.PushBack({ 448,480,32,32 });
    fireballAnim.PushBack({ 480,480,32,32 });
    fireballAnim.speed = 0.5f;
    fireballAnim.loop = false;

    ultimo_uso = std::chrono::steady_clock::now();

}

Player::~Player() {

}

bool Player::Awake() {

	position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());
  
	return true;
}

bool Player::Start() {

	texture = app->tex->Load(config.attribute("texturePath").as_string());
	app->tex->GetSize(texture, texW, texH);
    
    texture2 = app->tex->Load("Textures/character/Free  Effect Bullet Impact Explosion 32x32 V1.png");
    app->tex->GetSize(texture2, texW, texH);

	pbody = app->physics->CreateCircle(position.x, position.y, 11, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;
	return true;
}


bool Player::Update(float dt) {

    currentAnimation = &idleAnim;
    currentFireAnim = &fireballAnim;

    velocity = pbody->body->GetLinearVelocity();

    if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
    {
        godmode = true;
    }
    if (godmode == true )
    {
        if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
            position.y -= 6;

        if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
            position.x -= 6;

        if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
            position.y += 6;

        if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
            position.x += 6;
    }
    else
    {
        if (die == true)
        {
            currentAnimation = &dieAnim;
            if (dieAnim.HasFinished())
            {
                position.x = 250;
                position.y = 672;
                app->physics->DestroyBody(pbody);
                pbody = app->physics->CreateCircle(position.x, position.y, 11, bodyType::DYNAMIC);
                pbody->listener = this;
                pbody->ctype = ColliderType::PLAYER;
                die = false;
                dieAnim.Reset();
            }
        }
        else
        {
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

            if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
            {
                if (app->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN && Cooldown(5.0f))
                {
                    currentFireAnim = &fireballAnim;
                    if (dispar != nullptr)
                    {
                        app->physics->DestroyBody(dispar);
                        currentFireAnim = nullptr;
                    }

                    b2Vec2 forceToApply(0.0f, -400.0f);
                    b2Vec2 pointOfApplication(position.x + 50, position.y + 50);

                    dispar = app->physics->CreateCircle(position.x + 50, position.y, 11, bodyType::DYNAMIC);
                    dispar->ctype = ColliderType::ABILITY;
                    dispar->body->SetGravityScale(0.0f);

                    dispar->body->ApplyForce(forceToApply, pointOfApplication, true);
                }
            }

            else if (app->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN && Cooldown(5.0f))
            {
                currentFireAnim = &fireballAnim;
                if (dispar != nullptr)
                {
                    app->physics->DestroyBody(dispar);
                    currentFireAnim = nullptr;
                }

                if (left == true)
                {
                    b2Vec2 forceToApply(-400.0f, 0.0f);
                    b2Vec2 pointOfApplication(position.x + 50, position.y + 50);

                    dispar = app->physics->CreateCircle(position.x + 50, position.y, 11, bodyType::DYNAMIC);
                    dispar->ctype = ColliderType::ABILITY;
                    dispar->body->SetGravityScale(0.0f);

                    dispar->body->ApplyForce(forceToApply, pointOfApplication, true);
                }
                
                else
                {
                    b2Vec2 forceToApply(400.0f, 0.0f);
                    b2Vec2 pointOfApplication(position.x + 50, position.y + 50);

                    dispar = app->physics->CreateCircle(position.x + 50, position.y, 11, bodyType::DYNAMIC);
                    dispar->ctype = ColliderType::ABILITY;
                    dispar->body->SetGravityScale(0.0f);

                    dispar->body->ApplyForce(forceToApply, pointOfApplication, true);
                }
                

            }

            

            if (dash)
            {
                currentAnimation = &dashAnim1;
                if (dashAnim1.HasFinished())
                {
                    if (left == false)
                    {
                        velocity.x = velocity.x + 200;
                        dash2 = true;
                    }

                    if (left == true)
                    {
                        velocity.x = velocity.x - 200;
                        dash2 = true;
                    }

                    dash = false;
                    dashAnim1.Reset();
                }
            }

            if (dash2)
            {
                currentAnimation = &dashAnim2;

                if (dashAnim2.HasFinished())
                {
                    dash2 = false;
                    dashAnim2.Reset();
                }
            }

            if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && left == false) {
                
                dash = true;
                
            }

            if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && left == true) {
                
                dash = true;

            }

            

            if (velocity.y < 0 && tocasuelo == true)
            {
                currentAnimation = &jumpAnim;
            }

            pbody->body->SetLinearVelocity(velocity);
            b2Transform pbodyPos = pbody->body->GetTransform();

            position.x = METERS_TO_PIXELS(pbodyPos.p.x) - 15;
            position.y = METERS_TO_PIXELS(pbodyPos.p.y) - 15;
        }
    }
    if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN && jumpCount < 2)//&& isOnGround) {
    {
        position.x = 250;
        position.y = 672;
        app->physics->DestroyBody(pbody);
        pbody = app->physics->CreateCircle(position.x, position.y, 11, bodyType::DYNAMIC);
        pbody->listener = this;
        pbody->ctype = ColliderType::PLAYER;
    }
    if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
    {
        godmode = false;
        position.x = 250;
        position.y = 672;
        app->physics->DestroyBody(pbody);
        pbody = app->physics->CreateCircle(position.x, position.y, 11, bodyType::DYNAMIC);
        pbody->listener = this;
        pbody->ctype = ColliderType::PLAYER;

    }
    currentAnimation->Update();
    currentFireAnim->Update();

    SDL_Rect rect = currentAnimation->GetCurrentFrame();
    SDL_Rect rect2 = currentFireAnim->GetCurrentFrame();

    if (left) {
        app->render->DrawTexture2(texture, position.x, position.y, SDL_FLIP_HORIZONTAL, &rect);
        app->render->DrawTexture2(texture2, position.x, position.y, SDL_FLIP_HORIZONTAL, &rect2);
    }
    else {
        app->render->DrawTexture2(texture, position.x, position.y, SDL_FLIP_NONE, &rect);
        app->render->DrawTexture2(texture2, position.x, position.y, SDL_FLIP_NONE, &rect2);
    }

    return true;
}

bool Player::CleanUp()
{
    app->tex->UnLoad(texture);
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
        app->entityManager->DestroyEntity(physB->listener);
        physB->body->SetActive(false);
        app->scene->visible = false;

        break;
    case ColliderType::DIE:
        LOG("Collision DIE");
        die = true;
        break;
    case ColliderType::ENEMY:
        LOG("Collision DIE");
        die = true;
        break;
    case ColliderType::ABILITY:
        LOG("Collision ABILITY");
        
        break;
    case ColliderType::UNKNOWN:
        LOG("Collision UNKNOWN");
        break;
    default:
        break;
    }
}

bool Player::Cooldown(float cooldown)
{
    auto ahora = std::chrono::steady_clock::now();
    float tiempo_transcurrido = std::chrono::duration<float>(ahora - ultimo_uso).count();

    if (tiempo_transcurrido >= cooldown) {
        ultimo_uso = ahora; // reiniciar el contador
        return true;
    }
    else {
        return false;
    }
}