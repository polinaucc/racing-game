#pragma once
#include "VehicleWheel.h"
#include "Wheel.generated.h"

UCLASS()
class RACING_GAME_API UWheel : public UVehicleWheel {
	GENERATED_BODY()
public:
	UWheel();

	static float CurrentMaximumSteerAngle;
	static float CurrentFrictionScale;
	static float CurrentLatStiffness;
};