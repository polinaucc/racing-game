#pragma once
#include "CoreMinimal.h"
#include "WeaponInterface.h"
#include "MachineGun.generated.h"

UCLASS()
class RACING_GAME_API AMachineGun : public AWeaponInterface {
	GENERATED_BODY()
protected:
	bool left_or_right;
public:	
	AMachineGun();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	float const damage_coefficient = 1.f;

	virtual class ABulletInterface* create_next_bullet() override;
	virtual float damage() const;

	virtual void virtual_update_level(EEquipementLevel new_level) override;
};
