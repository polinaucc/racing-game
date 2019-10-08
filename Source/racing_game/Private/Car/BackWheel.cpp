#include "BackWheel.h"
#include "TireConfig.h" //PhysXVehicles
UBackWheel::UBackWheel() {
	ConstructorHelpers::FObjectFinder<UStaticMesh> wheelMesh(TEXT("StaticMesh'/Engine/EngineMeshes/Cylinder.Cylinder'"));
	CollisionMesh = wheelMesh.Object;
	ShapeRadius = 57.f;
	ShapeWidth = 30.0f;
	bAffectedByHandbrake = true;
	SteerAngle = 0.0f;
	TireConfig = CreateDefaultSubobject<UTireConfig>("BackTireConfig");
	TireConfig->SetFrictionScale(CurrentFrictionScale);
	LatStiffValue = CurrentLatStiffness;
}