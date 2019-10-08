#pragma once
#include <utility>
#include "CoreMinimal.h"
#include "BulletInterface.h"
#include "MachineGunBullet.generated.h"

UCLASS()
class RACING_GAME_API AMachineGunBullet : public ABulletInterface {
	GENERATED_BODY()
protected:
	const float bullet_scale = 0.3f;
public:	
	AMachineGunBullet();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	static FVector next_position(FVector location, FVector direction, bool left_or_right);
};
