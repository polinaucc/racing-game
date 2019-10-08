#pragma once
#include "CoreMinimal.h"
#include "CarUpgrade.h"
#include "save.h"
#include "Shield.generated.h"

UCLASS()
class RACING_GAME_API AShield : public ACarUpgrade {
	GENERATED_BODY()
public:	
	AShield();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	float operator()(float damage);
};
