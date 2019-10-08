#include "TrackSpline.h"
ATrackSpline::ATrackSpline() {
	m_spline = CreateDefaultSubobject<USplineComponent>("TrackSplineComponent");
	PrimaryActorTick.bCanEverTick = false;
	//generate(15, 7000, 10000);
	//randomize();
	m_spline->SetClosedLoop(true);
}

auto const M_PI = 3.14159265358979323846;

#include <cmath>
#include <random>
#include "Components/SplineMeshComponent.h"
void ATrackSpline::generate_component(std::string name, UStaticMesh *mesh,
									  FVector &start_location, FVector &start_tangent,
									  FVector &end_location, FVector &end_tangent) {
	auto *temp_mesh = NewObject<USplineMeshComponent>(this, name.c_str());
	temp_mesh->RegisterComponent();
	temp_mesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;

	temp_mesh->SetMobility(EComponentMobility::Type::Movable);
	temp_mesh->SetStaticMesh(mesh);
	temp_mesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);

	temp_mesh->SetStartAndEnd(start_location, start_tangent, end_location, end_tangent);
}

#include <string>
void ATrackSpline::BeginPlay() {
	Super::BeginPlay();
	if (Autofill)
		generate();
}
void ATrackSpline::generate() {
	int points = 15;
	float min_dist = 7000, max_dist = 10000;

	std::mt19937_64 random(std::random_device{}());
	std::uniform_real_distribution<float> number(min_dist, max_dist);

	m_spline->ClearSplinePoints();
	float const step = 2.f * M_PI / points;
	m_spline->AddSplinePoint(FVector(min_dist * cos(0), min_dist * sin(0), 0), ESplineCoordinateSpace::World);
	for (float f = step; f < 2.f * M_PI; f += step)
		m_spline->AddSplinePoint(FVector(number(random) * cos(f), number(random) * sin(f), 0), ESplineCoordinateSpace::World);

	m_spline->UpdateSpline();

	FVector end_location, end_tangent;
	m_spline->GetLocationAndTangentAtSplinePoint(m_spline->GetNumberOfSplinePoints() - 1,
												 end_location, end_tangent,
												 ESplineCoordinateSpace::World);
	for (int i = 0; i < m_spline->GetNumberOfSplinePoints(); i++) {
		FVector temp_location, temp_tangent;
		m_spline->GetLocationAndTangentAtSplinePoint(i, temp_location, temp_tangent,
													 ESplineCoordinateSpace::World);

		generate_component("TrackSplineMeshComponent" + std::to_string(i), road_mesh,
						   end_location, end_tangent, temp_location, temp_tangent);
		generate_component("LeftSplineMeshComponent" + std::to_string(i), left_rail_mesh,
						   end_location, end_tangent, temp_location, temp_tangent);
		generate_component("RightSplineMeshComponent" + std::to_string(i), right_rail_mesh,
						   end_location, end_tangent, temp_location, temp_tangent);

		end_location = temp_location;
		end_tangent = temp_tangent;
	}
}