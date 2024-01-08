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

	// El boss no tendra un path, atacara aleatoriamente cuando tenga al player cerca. Si el boss toca al player, el player pierde una vida
	// Si el Boss ataca y da al player, pierde una vida. Si el player se aleja mucho despues de haber combatido un rato, el Boss hara un salto en direccion al player.
	// Algo mas
	return true;
}

bool Boss::CleanUp() {
	return true;
}

void Boss::OnCollision(PhysBody* physA, PhysBody* physB) {

}
