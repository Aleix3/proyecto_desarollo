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
	return true;
}

bool Boss::CleanUp() {
	return true;
}

void Boss::OnCollision(PhysBody* physA, PhysBody* physB) {

}
