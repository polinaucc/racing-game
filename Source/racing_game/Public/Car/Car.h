#pragma once
#include "WheeledVehicle.h" //PhysXVehicles
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/WidgetComponent.h"
#include "Save.h"
#include "Car.generated.h"

UCLASS()
class RACING_GAME_API ACar : public AWheeledVehicle {
	GENERATED_BODY()

protected:
	float health;
	float max_health;
	bool is_alive;
	EEquipementLevel shield_level;
	EEquipementLevel weapon_level;
	FLinearColor temp_color;

	FTimerHandle invincibility_timer;
	bool is_invincible;

	class ACarManager *m_manager;
public:
	ACar();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UChildActorComponent *Weapon;
	UChildActorComponent *Shield;
	TArray<UChildActorComponent*> Upgrades;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User")
	FName name = TEXT("Unnamed");

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent *SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent *MainCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	UChildActorComponent *DestroyedCarEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> HealthBarClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UWidgetComponent *HealthBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget *PauseWidget;

	UPROPERTY(EditAnywhere, Category = "TrackData")
	class ATrackSpline *track;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User")
	bool fire_allowed;

	void MoveForward(float value);
	void MoveRight(float value);
	void HandbrakeOn();
	void HandbrakeOff();
	void StartFire();
	void StopFire();
	void Pause();
	void Restart();

	void ApplyDamage(float value);
	void Die();

	UFUNCTION()
	void OnEffectFinished();

	UFUNCTION()
	void OnInvincibilityEnd();

	UFUNCTION()
	void OnHit(AActor *SelfActor, AActor *OtherActor,
			   FVector NormalImpulse, FHitResult const& Hit);

	virtual void virtual_on_hit(AActor *SelfActor, AActor *OtherActor,
								FVector NormalImpulse, FHitResult const& Hit) {}

	float get_speed();
	int get_gear();

	UFUNCTION()
	void EnableManagement(class ACarManager *manager) { m_manager = manager; }

	void update_name(FName _name);
};