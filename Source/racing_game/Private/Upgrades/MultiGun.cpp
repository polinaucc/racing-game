#include "MultiGun.h"
#include "BulletSpawnEffect.h"
AMultiGun::AMultiGun() {
	PrimaryActorTick.bCanEverTick = true;
}
void AMultiGun::BeginPlay() {
	Super::BeginPlay();
	counter = 0;
}
void AMultiGun::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

float AMultiGun::damage() const {
	return USave::ConvertDamage(level()) * damage_coefficient;
}

#include "BulletInterface.h"
#include "MultiGunBulletFactory.h"
ABulletInterface* AMultiGun::create_next_bullet() {
	MultiGunBulletFactory factory;
	auto ret = factory.create(level(), counter, this);
	if (counter++ >= switch_EEquipementLevel(level()) + 3)
		counter = 0;
	return ret;
}

void AMultiGun::virtual_update_level(EEquipementLevel new_level) {
	reload_time = .12f / (1.f + .7 * switch_EEquipementLevel(new_level));
}