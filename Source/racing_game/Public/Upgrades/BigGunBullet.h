#pragma once
#include <utility>
#include "CoreMinimal.h"
#include "BulletInterface.h"
#include "BigGunBullet.generated.h"

UCLASS()
class RACING_GAME_API ABigGunBullet : public ABulletInterface {
	GENERATED_BODY()
public:	
	ABigGunBullet();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};