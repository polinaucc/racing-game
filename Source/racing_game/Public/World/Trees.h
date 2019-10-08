#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Trees.generated.h"

UCLASS()
class RACING_GAME_API ATrees : public AActor {
	GENERATED_BODY()
	
public:
	ATrees();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TArray<UStaticMesh*> TreeMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Data")
	class ATrackSpline *Track;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Data")
	class ALandscape *Landscape;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Data")
	int32 NumberOfTrees;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Data")
	float FillPercent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Data")
	int32 TrackAvoidanceRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Data")
	bool Autofill;

	UFUNCTION()
	void generate();
};
