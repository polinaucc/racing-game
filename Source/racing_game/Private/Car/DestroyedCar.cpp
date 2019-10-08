#include "DestroyedCar.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ADestroyedCar::ADestroyedCar() {
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	ConstructorHelpers::FObjectFinder<UParticleSystem> fireEmitter(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Fire.P_Fire'"));
	FireEffectReference = fireEmitter.Object;

	FireParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireParticleEffect"));
	FireParticleEffect->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
	FireParticleEffect->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	ConstructorHelpers::FObjectFinder<UParticleSystem> flashEmitter(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	ExplosionEffectReference = flashEmitter.Object;

	ExplosionParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionParticleEffect"));
	ExplosionParticleEffect->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
	ExplosionParticleEffect->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	ConstructorHelpers::FObjectFinder<USoundCue> explosionSoundCue(TEXT("SoundCue'/Game/StarterContent/Audio/Explosion_Cue.Explosion_Cue'"));
	ExplosionSoundEffect = explosionSoundCue.Object;

	ConstructorHelpers::FObjectFinder<USoundCue> fireSoundCue(TEXT("SoundCue'/Game/StarterContent/Audio/Fire01_Cue.Fire01_Cue'"));
	FireSoundEffect = fireSoundCue.Object;

	FireAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FireAudioComponent"));
	FireAudioComponent->SetVolumeMultiplier(2.0f);
	FireAudioComponent->SetRelativeScale3D(FVector(20.0f, 20.0f, 20.0f));
	FireAudioComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}
void ADestroyedCar::BeginPlay() {
	Super::BeginPlay();
}
void ADestroyedCar::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ADestroyedCar::ActivateEffect() {
	FireParticleEffect->SetTemplate(FireEffectReference);
	ExplosionParticleEffect->SetTemplate(ExplosionEffectReference);

	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSoundEffect, GetActorLocation());
	FireAudioComponent->SetSound(FireSoundEffect);
	FireAudioComponent->Play();
}

void ADestroyedCar::DeactivateEffect() {
	if (FireAudioComponent)	{
		FireAudioComponent->Stop();
	}
}