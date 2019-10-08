#include "BulletSpawnEffect.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ABulletSpawnEffect::ABulletSpawnEffect() {
	ConstructorHelpers::FObjectFinder<UParticleSystem> flashEmitter(TEXT("ParticleSystem'/Game/Particles/MuzzleFlash.MuzzleFlash'"));
	ParticleEffectReference = flashEmitter.Object;
	ParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleEffect"));
	ParticleEffect->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
	RootComponent = ParticleEffect;

	ConstructorHelpers::FObjectFinder<USoundCue> bulletSpawnSound(TEXT("SoundCue'/Game/Audio/SoundCues/MachineGunShotSound_Cue.MachineGunShotSound_Cue'"));
	SoundCueEffect = bulletSpawnSound.Object;
}
void ABulletSpawnEffect::BeginPlay() {
	Super::BeginPlay();
}
void ABulletSpawnEffect::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ABulletSpawnEffect::ActivateEffect() {
	ParticleEffect->SetTemplate(ParticleEffectReference);
	UGameplayStatics::PlaySoundAtLocation(this, SoundCueEffect, GetActorLocation());
}