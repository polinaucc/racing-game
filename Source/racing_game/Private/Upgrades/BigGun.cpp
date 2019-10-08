#include "BigGun.h"
#include "BulletSpawnEffect.h"
#include "BulletInterface.h"
#include "BigGunBulletFactory.h"

ABigGun::ABigGun() {
	PrimaryActorTick.bCanEverTick = true;
}
void ABigGun::BeginPlay() {
	Super::BeginPlay();
	reload_time = .72f;
}
void ABigGun::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

float ABigGun::damage() const {
	return USave::ConvertDamage(level()) * damage_coefficient;
}

ABulletInterface* ABigGun::create_next_bullet() {
	BigGunBulletFactory factory;
	return factory.create(level(), 0, this);
}