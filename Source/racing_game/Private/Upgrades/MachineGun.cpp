#include "MachineGun.h"
#include "BulletSpawnEffect.h"
AMachineGun::AMachineGun() {
	PrimaryActorTick.bCanEverTick = true;
}
void AMachineGun::BeginPlay() {
	Super::BeginPlay();
	left_or_right = true;
}
void AMachineGun::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

float AMachineGun::damage() const {
	return USave::ConvertDamage(level()) * damage_coefficient;
}

#include "BulletInterface.h"
#include "MachineGunBulletFactory.h"
ABulletInterface* AMachineGun::create_next_bullet() {
	MachineGunBulletFactory factory;
	auto ret = factory.create(level(), left_or_right, this);
	left_or_right = !left_or_right;
	return ret;
}

void AMachineGun::virtual_update_level(EEquipementLevel new_level) {
	reload_time = .12f / (1.f + .7 * switch_EEquipementLevel(new_level));
}