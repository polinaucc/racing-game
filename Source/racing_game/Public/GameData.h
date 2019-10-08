#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameData.generated.h"

UENUM(BlueprintType)
enum class RaceType : uint8 {
	Time UMETA(DisplayName = "Time Race"),
	Duel UMETA(DisplayName = "Duel Race"),
	Normal UMETA(DisplayName = "Normal Race"),
	Survival UMETA(DisplayName = "Survival Race")
};

UCLASS()
class RACING_GAME_API UGameData : public UGameInstance {
	GENERATED_BODY()

public:
	UGameData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	RaceType type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 AI_car_number;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 lap_number;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float time_limit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float current_time;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 laps_left;
};
