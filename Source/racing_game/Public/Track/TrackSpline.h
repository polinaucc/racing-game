#pragma once
#include <string>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"

#include "TrackSpline.generated.h"

class UStaticMesh;
struct FVector;

UCLASS()
class RACING_GAME_API ATrackSpline : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "MeshData")
	UStaticMesh *road_mesh;
	UPROPERTY(EditAnywhere, Category = "MeshData")
	UStaticMesh *left_rail_mesh;
	UPROPERTY(EditAnywhere, Category = "MeshData")
	UStaticMesh *right_rail_mesh;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SplineData")
	USplineComponent *m_spline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Data")
	bool Autofill;

public:
	ATrackSpline();

	virtual void BeginPlay() override;
	void generate_component(std::string name, UStaticMesh *mesh,
							FVector &start_location, FVector &start_tangent,
							FVector &end_location, FVector &end_tangent);

	UFUNCTION()
	void generate();
};
