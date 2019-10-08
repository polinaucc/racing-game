#pragma once
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "DestroyedCar.generated.h"

UCLASS()
class RACING_GAME_API ADestroyedCar : public AActor {
	GENERATED_BODY()

private:
	UParticleSystem* ExplosionEffectReference;
	UParticleSystem* FireEffectReference;
	class ADestructibleBox* DestructibleBox;

public:
	ADestroyedCar();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	UParticleSystemComponent* ExplosionParticleEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	UParticleSystemComponent* FireParticleEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundCue* ExplosionSoundEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundCue* FireSoundEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	UAudioComponent* FireAudioComponent;

	UFUNCTION()
	void ActivateEffect();

	UFUNCTION()
	void DeactivateEffect();
};