#include "CarAI.h"
#include "WheeledVehicleMovementComponent.h" //PhysXVehicles
#include "WheeledVehicleMovementComponent4W.h" //PhysXVehicles
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Math/Vector.h"
#include "Kismet/GameplayStatics.h"
#include "FrontWheel.h"
#include "BackWheel.h"
#include "TrackSpline.h"

EEquipementLevel ACarAI::current_level = EEquipementLevel::L3;

ACarAI::ACarAI() : ACar() {
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	UWheel::CurrentMaximumSteerAngle = USave::ConvertMaximumSteerAngle(current_level);
	UWheel::CurrentFrictionScale = USave::ConvertFrictionScale(current_level);
	UWheel::CurrentLatStiffness = USave::ConvertLatStiffness(current_level);

	GetVehicleMovement()->WheelSetups.SetNum(4); //set number of wheels
	GetVehicleMovement()->WheelSetups[0].WheelClass = UFrontWheel::StaticClass();
	GetVehicleMovement()->WheelSetups[0].BoneName = FName("F_L_wheelJNT");
	GetVehicleMovement()->WheelSetups[1].WheelClass = UFrontWheel::StaticClass();
	GetVehicleMovement()->WheelSetups[1].BoneName = FName("F_R_wheelJNT");
	GetVehicleMovement()->WheelSetups[2].WheelClass = UBackWheel::StaticClass();
	GetVehicleMovement()->WheelSetups[2].BoneName = FName("B_L_wheelJNT");
	GetVehicleMovement()->WheelSetups[3].WheelClass = UBackWheel::StaticClass();
	GetVehicleMovement()->WheelSetups[3].BoneName = FName("B_R_wheelJNT");
	auto MovementComponent = const_cast<UWheeledVehicleMovementComponent*>(GetVehicleMovementComponent());
	//auto MovementComponent = GetVehicleMovementComponent();

	//MovementComponent->MaxEngineRPM = USave::ConvertEngineRPM(current_level);
	Cast<UWheeledVehicleMovementComponent4W>(MovementComponent)->EngineSetup.MaxRPM = USave::ConvertEngineRPM(current_level);
	Cast<UWheeledVehicleMovementComponent4W>(MovementComponent)->TransmissionSetup.GearSwitchTime = USave::ConvertGearSwitchTime(current_level);
	Cast<UWheeledVehicleMovementComponent4W>(MovementComponent)->DifferentialSetup.DifferentialType = EVehicleDifferential4W::LimitedSlip_FrontDrive;
	MovementComponent->Mass = USave::ConvertMass(current_level);
	MovementComponent->DragCoefficient = USave::ConvertDragCoefficient(current_level);

	max_health = USave::ConvertMaxHealth(current_level);
	shield_level = current_level;
	weapon_level = current_level;
}
void ACarAI::BeginPlay() {
	Super::BeginPlay();
	//GetMesh()->SetAllMassScale(0.8);
	near_wall = 0.0;
}

float calculate_angle(FVector const& first, FVector const& second) {
	return atan2(first.X * second.Y - first.Y * second.X, 
				 first.X * second.X + first.Y * second.Y);
}

void ACarAI::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	auto car_position = GetActorLocation();
	auto car_velocity = GetVelocity();
	auto car_speed = car_velocity.Size2D();
	auto car_direction = car_velocity;
	car_direction.Normalize();

	if (car_speed <= 50)
		Restart();

	auto track_point = track->m_spline->FindLocationClosestToWorldLocation(car_position, ESplineCoordinateSpace::World);
	auto track_vector = track->m_spline->FindDirectionClosestToWorldLocation(car_position, ESplineCoordinateSpace::World);

	auto distance_to_the_center = (car_position - track_point);

	auto lookup_point = track->m_spline->FindLocationClosestToWorldLocation(track_point + track_vector * car_speed, ESplineCoordinateSpace::World);
	auto lookup_vector = lookup_point - car_position;

	//DrawDebugSphere(GetWorld(), lookup_point + FVector(0, 0, 200), 30, 20, FColor(255, 0, 0));
	//DrawDebugLine(GetWorld(), lookup_point + FVector(0, 0, 200), track_point + FVector(0, 0, 200), FColor(255, 0, 255), false, -1, '\000', 3);
	//DrawDebugLine(GetWorld(), car_position + FVector(0, 0, 200), lookup_point + FVector(0, 0, 200), FColor(255, 0, 255), false, -1, '\000', 3);

	float steering_value, throttle_value;
	
		lookup_vector.Normalize();
		steering_value = calculate_angle(car_direction, lookup_vector);
		throttle_value = FMath::GetMappedRangeValueUnclamped({0, 5}, {1, 0}, fabs(steering_value));

	if (near_wall > 0.f) {
		//steering_value = -steering_value;
		//throttle_value = -throttle_value;

		near_wall -= DeltaTime;
	}

	if (is_alive) {
		GetVehicleMovement()->SetSteeringInput(steering_value);
		GetVehicleMovement()->SetThrottleInput(throttle_value);
	}
}

void ACarAI::virtual_on_hit(AActor *SelfActor, AActor *OtherActor,
							FVector NormalImpulse, FHitResult const& Hit) {
	if ((OtherActor != NULL) && (OtherActor != this))
		if (OtherActor->IsA(ATrackSpline::StaticClass()))
			near_wall += 0.4;
}