#include "Shield.h"
AShield::AShield() {
	PrimaryActorTick.bCanEverTick = false;
}
void AShield::BeginPlay() {
	Super::BeginPlay();
}
void AShield::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
float AShield::operator()(float damage) {
	return damage / USave::ConvertArmor(level());
}