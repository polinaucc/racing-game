#include "BulletInterface.h"
#include "Car.h"

ABulletInterface::ABulletInterface() {
	PrimaryActorTick.bCanEverTick = true;
}
void ABulletInterface::BeginPlay() {
	Super::BeginPlay();	
}
void ABulletInterface::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ABulletInterface::OnOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
	AActor* weaponOwner = GetOwner();
	AActor* vehicleOwner = weaponOwner->GetOwner();

	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics() && (OtherActor != vehicleOwner)) {
		if (OtherActor->IsA(ACar::StaticClass())) {
			ACar* carHit = Cast<ACar>(OtherActor);
			carHit->ApplyDamage(damage);
		}
		OtherComp->AddImpulseAtLocation(GetVelocity() * 10.0f, GetActorLocation());
		Destroy();
	}
}