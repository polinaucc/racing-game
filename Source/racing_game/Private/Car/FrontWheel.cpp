#include "FrontWheel.h"
#include "TireConfig.h" //PhysXVehicles
UFrontWheel::UFrontWheel() {
	ConstructorHelpers::FObjectFinder<UStaticMesh> wheelMesh(TEXT("StaticMesh'/Engine/EngineMeshes/Cylinder.Cylinder'"));
	CollisionMesh = wheelMesh.Object;
	ShapeRadius = 52.f;
	ShapeWidth = 23.0f;
	bAffectedByHandbrake = false;
	SteerAngle = CurrentMaximumSteerAngle;
	TireConfig = CreateDefaultSubobject<UTireConfig>("FrontTireConfig");
	TireConfig->SetFrictionScale(CurrentFrictionScale);
	LatStiffValue = CurrentLatStiffness;
}