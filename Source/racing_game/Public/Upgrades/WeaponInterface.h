#pragma once
#include "CoreMinimal.h"
#include "CarUpgrade.h"
#include "save.h"
#include "WeaponInterface.generated.h"

UCLASS(abstract)
class RACING_GAME_API AWeaponInterface : public ACarUpgrade {
	GENERATED_BODY()
protected:
	float reload_time;

	bool bTimerRunning = false;
	bool is_active;
public:
	AWeaponInterface();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void Activate();
	void Deactivate();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UChildActorComponent* BulletSpawnEffect;

	FTimerHandle TimerHandle;

	UFUNCTION(Category = "Behavior")
	virtual void PerformFireWeapon();

	virtual class ABulletInterface* create_next_bullet() PURE_VIRTUAL(AWeaponInterface::create_next_bullet, return nullptr;); //чистая виртуальная фун
	virtual float damage() const;
protected:
	void TimerCallback();
};
