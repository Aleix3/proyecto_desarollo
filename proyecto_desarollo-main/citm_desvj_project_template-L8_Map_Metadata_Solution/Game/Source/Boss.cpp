#include "Boss.h"

Boss::Boss() : Entity(EntityType::BOSS) {

}

Boss::~Boss() {

}

bool Boss::Awake() {
	name.Create("Boss");
	return true;
}

bool Boss::Start() {
	return true;
}

bool Boss::Update(float dt) {

	// El boss no se movera, sequedará quieto y irá spwaneando varios bichos cada cierto tiempo. Los bichos seguirán al player con el path
	// Si el player se acerca mucho al Boss, el Boss atacará con su guadaña. El Boss tiene una cierta cantidad de vida
	return true;
}

bool Boss::CleanUp() {
	return true;
}

void Boss::OnCollision(PhysBody* physA, PhysBody* physB) {

}
