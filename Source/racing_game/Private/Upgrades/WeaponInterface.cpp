#include "WeaponInterface.h"
#include "BulletInterface.h"
#include "BulletSpawnEffect.h"
AWeaponInterface::AWeaponInterface() : reload_time(0.12f) {
	PrimaryActorTick.bCanEverTick = true;

	BulletSpawnEffect = CreateDefaultSubobject<UChildActorComponent>(TEXT("BulletSpawnEffect"));
	BulletSpawnEffect->SetChildActorClass(ABulletSpawnEffect::StaticClass());
	BulletSpawnEffect->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}
void AWeaponInterface::BeginPlay() {
	Super::BeginPlay();
	SetActorTickEnabled(true);
	is_active = false;
}
void AWeaponInterface::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (is_active && !bTimerRunning) {
		bTimerRunning = true;
		PerformFireWeapon();
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AWeaponInterface::TimerCallback, reload_time, false);
	}
}

void AWeaponInterface::Activate() {
	is_active = true;
}
void AWeaponInterface::Deactivate() {
	is_active = false;
}
void AWeaponInterface::TimerCallback() {
	bTimerRunning = false;
}
void AWeaponInterface::PerformFireWeapon() {
	auto bullet = create_next_bullet();
	if (BulletSpawnEffect) {
		ABulletSpawnEffect* bulletSpawnEffect = Cast<ABulletSpawnEffect>(BulletSpawnEffect->GetChildActor());
		if (bulletSpawnEffect) {
			bulletSpawnEffect->SetActorLocation(bullet->GetActorLocation());
			bulletSpawnEffect->ActivateEffect();
		}
	}
}
float AWeaponInterface::damage() const {
	return USave::ConvertDamage(level());
}