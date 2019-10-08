#include "Trees.h"
#include "Classes/Landscape.h"
#include <random>
#include "EngineUtils.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "InstancedFoliageActor.h"
#include "TrackSpline.h"
#include "Engine/World.h"
#include <thread>

ATrees::ATrees() {
	PrimaryActorTick.bCanEverTick = true;
	NumberOfTrees = 500;
	FillPercent = 1.0;
	TrackAvoidanceRadius = 1000;
	Autofill = false;
}

void ATrees::BeginPlay() {
	Super::BeginPlay();
	if (Autofill)
		generate();
}

void ATrees::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ATrees::generate() {
	TActorIterator<AInstancedFoliageActor> foliageIterator(GetWorld());
	AInstancedFoliageActor* foliageActor = *foliageIterator;

	TArray<UInstancedStaticMeshComponent*> components;
	components.Reserve(TreeMeshes.Num());
	for (int32 i = 0; i < TreeMeshes.Num(); i++) {
		UInstancedStaticMeshComponent* temp = NewObject<UInstancedStaticMeshComponent>(foliageActor, UInstancedStaticMeshComponent::StaticClass(), NAME_None, RF_Transactional);
		temp->AttachToComponent(foliageActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		temp->SetStaticMesh(TreeMeshes[i]);
		temp->RegisterComponent();
		components.Add(temp);
	}

	FVector origin;
	FVector boxExtent;
	Landscape->GetActorBounds(false, origin, boxExtent);

	static std::mt19937_64 random(std::random_device{}());
	std::uniform_int_distribution<> x_margins(-boxExtent.X * FillPercent + 1, +boxExtent.X * FillPercent - 1);
	std::uniform_int_distribution<> y_margins(-boxExtent.Y * FillPercent + 1, +boxExtent.Y * FillPercent - 1);
	std::uniform_int_distribution<> tree_mesh(0, TreeMeshes.Num() - 1);
	std::uniform_int_distribution<> angle(0, 359);

	for (int32 i = 0; i < NumberOfTrees; i++) {
		FVector location(x_margins(random), y_margins(random), 0);
		auto vector = Track->m_spline->FindLocationClosestToWorldLocation(location, ESplineCoordinateSpace::World) - location;

		if (vector.Size2D() >= TrackAvoidanceRadius) {
			FHitResult hit_result;
			if (GetWorld()->LineTraceSingleByChannel(hit_result, FVector(location.X, location.Y, 1000), FVector(location.X, location.Y, -1000), ECollisionChannel::ECC_WorldStatic)) {
				location = hit_result.Location;

				FTransform t;
				t.SetRotation(FRotator(0, angle(random), 0).Quaternion());
				t.SetLocation(location);

				components[tree_mesh(random)]->AddInstance(t);
			} else
				i--;
		} else
			i--;
	}
}