#pragma once
#include <utility>
#include "CoreMinimal.h"
#include "BulletInterface.h"
#include "MultiGunBullet.generated.h"

UCLASS()
class RACING_GAME_API AMultiGunBullet : public ABulletInterface {
	GENERATED_BODY()
protected:
	const float bullet_scale = 0.2f;
public:	
	AMultiGunBullet();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	static std::pair<FVector, FRotator> next_position(FVector location, FRotator rotation, size_t counter);
};
