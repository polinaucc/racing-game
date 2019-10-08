#include "BigGunBulletFactory.h"
#include "BigGunBullet.h"
ABulletInterface* BigGunBulletFactory::create(EEquipementLevel level, size_t counter, AActor *owner) {
	FActorSpawnParameters bulletParams = FActorSpawnParameters();
	bulletParams.Owner = owner;
	return owner->GetWorld()->SpawnActor<ABigGunBullet>(ABigGunBullet::StaticClass(), owner->GetActorLocation(), FRotator(owner->GetActorRotation().Pitch, owner->GetActorRotation().Yaw, 0.f), bulletParams);
}
