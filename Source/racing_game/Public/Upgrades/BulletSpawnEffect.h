#pragma once
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
#include "BulletSpawnEffect.generated.h"

UCLASS()
class RACING_GAME_API ABulletSpawnEffect : public AActor {
	GENERATED_BODY()

private:
	UParticleSystem* ParticleEffectReference;

public:
	ABulletSpawnEffect();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Particle")
	UParticleSystemComponent* ParticleEffect;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundCue* SoundCueEffect;
		
	UFUNCTION()
	void ActivateEffect();
};