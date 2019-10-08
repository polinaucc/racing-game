#include "MultiGunBulletFactory.h"
#include "MultiGunBullet.h"
ABulletInterface* MultiGunBulletFactory::create(EEquipementLevel level, size_t counter, AActor *owner) {
	auto next_position = AMultiGunBullet::next_position(owner->GetActorLocation(), owner->GetActorRotation(), counter);
	FActorSpawnParameters bulletParams = FActorSpawnParameters();
	bulletParams.Owner = owner;
	return owner->GetWorld()->SpawnActor<AMultiGunBullet>(AMultiGunBullet::StaticClass(), next_position.first, FRotator(next_position.second.Pitch, next_position.second.Yaw, 0.f), bulletParams);
}
