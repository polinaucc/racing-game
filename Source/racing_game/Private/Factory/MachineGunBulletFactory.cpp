#include "MachineGunBulletFactory.h"
#include "MachineGunBullet.h"
ABulletInterface* MachineGunBulletFactory::create(EEquipementLevel level, size_t counter, AActor *owner) {
	auto next_position = AMachineGunBullet::next_position(owner->GetActorLocation(), owner->GetActorForwardVector(), bool(counter));
	FActorSpawnParameters bulletParams = FActorSpawnParameters();
	bulletParams.Owner = owner;
	return owner->GetWorld()->SpawnActor<AMachineGunBullet>(AMachineGunBullet::StaticClass(), next_position, FRotator(owner->GetActorRotation().Pitch, owner->GetActorRotation().Yaw, 0.f), bulletParams);
}
