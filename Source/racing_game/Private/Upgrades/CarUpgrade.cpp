#include "CarUpgrade.h"

ACarUpgrade::ACarUpgrade() {
	PrimaryActorTick.bCanEverTick = false;
}
void ACarUpgrade::BeginPlay() {
	Super::BeginPlay();
}
void ACarUpgrade::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}