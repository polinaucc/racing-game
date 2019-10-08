#include "Car.h"
#include "DestroyedCar.h"
#include "WeaponInterface.h"
#include "Shield.h"
#include "WheeledVehicleMovementComponent.h" //PhysXVehicles
#include "WheeledVehicleMovementComponent4W.h" //PhysXVehicles
#include "TireConfig.h" //PhysXVehicles
#include "Components/InputComponent.h"
#include "Engine.h"
#include "Blueprint/UserWidget.h"
#include "Save.h"
#include "FrontWheel.h"
#include "BackWheel.h"
#include "TrackSpline.h"
#include "CarManager.h"
#include "MultiGun.h"
#include "MachineGun.h"
#include "BigGun.h"

ACar::ACar() : AWheeledVehicle() {
	PrimaryActorTick.bCanEverTick = true;

	OnActorHit.AddDynamic(this, &ACar::OnHit);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RootComponent = GetMesh();

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SpringArm->SetRelativeLocation(FVector(0, 0, 250));
	SpringArm->AddLocalRotation(FRotator(-10, 0, 0));
	SpringArm->TargetArmLength = 500.0f;

	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	MainCamera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	MainCamera->bUsePawnControlRotation = false;
	MainCamera->AddLocalRotation(FRotator(-10, 0, 0));

	Shield = CreateDefaultSubobject<UChildActorComponent>(TEXT("Shield"));
	Shield->SetChildActorClass(AShield::StaticClass());
	Shield->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	DestroyedCarEffect = CreateDefaultSubobject<UChildActorComponent>(TEXT("DestroyedCarEffect"));
	DestroyedCarEffect->SetChildActorClass(ADestroyedCar::StaticClass());
	DestroyedCarEffect->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	DestroyedCarEffect->Deactivate();
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skMesh(TEXT("SkeletalMesh'/Game/Vehicles/Doom/Mesh/SK_Buggy_Vehicle.SK_Buggy_Vehicle'"));
	GetMesh()->SetSkeletalMesh(skMesh.Object);

	static ConstructorHelpers::FClassFinder<UObject> doomAnimationClass(TEXT("/Game/Vehicles/Doom/Blueprints/BP_VehicleAnimation"));
	GetMesh()->SetAnimInstanceClass(doomAnimationClass.Class);

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBarComponent");
	static ConstructorHelpers::FClassFinder<UObject> healthBarFinder(TEXT("WidgetBlueprint'/Game/Widgets/HealthBar.HealthBar_C'"));
	HealthBarClass = healthBarFinder.Class;

	USave* Savefile = Cast<USave>(UGameplayStatics::CreateSaveGameObject(USave::StaticClass()));
	Savefile = Cast<USave>(UGameplayStatics::LoadGameFromSlot(Savefile->SaveSlotName, Savefile->UserIndex));
	if (!Savefile)
		Savefile = Cast<USave>(UGameplayStatics::CreateSaveGameObject(USave::StaticClass()));
	UWheel::CurrentMaximumSteerAngle = Savefile->GetMaximumSteerAngle();
	UWheel::CurrentFrictionScale = Savefile->GetFrictionScale();
	UWheel::CurrentLatStiffness = Savefile->GetLatStiffness();

	Weapon = CreateDefaultSubobject<UChildActorComponent>(TEXT("Weapon"));
	switch (Savefile->CurrentAmmo) {
		case 0:
			Weapon->SetChildActorClass(AMachineGun::StaticClass());
			break;
		case 1:
			Weapon->SetChildActorClass(AMultiGun::StaticClass());
			break;
		case 2:
			Weapon->SetChildActorClass(ABigGun::StaticClass());
			break;
		default:
			throw std::runtime_error("Unsupported enum value.");
	}
	Weapon->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	GetVehicleMovement()->WheelSetups.SetNum(4); //set number of wheels
	GetVehicleMovement()->WheelSetups[0].WheelClass = UFrontWheel::StaticClass();
	GetVehicleMovement()->WheelSetups[0].BoneName = FName("F_L_wheelJNT");
	GetVehicleMovement()->WheelSetups[1].WheelClass = UFrontWheel::StaticClass();
	GetVehicleMovement()->WheelSetups[1].BoneName = FName("F_R_wheelJNT");
	GetVehicleMovement()->WheelSetups[2].WheelClass = UBackWheel::StaticClass();
	GetVehicleMovement()->WheelSetups[2].BoneName = FName("B_L_wheelJNT");
	GetVehicleMovement()->WheelSetups[3].WheelClass = UBackWheel::StaticClass();
	GetVehicleMovement()->WheelSetups[3].BoneName = FName("B_R_wheelJNT");
	auto MovementComponent = const_cast<UWheeledVehicleMovementComponent*>(GetVehicleMovement());

	//MovementComponent->MaxEngineRPM = Savefile->GetEngineRPM();
	Cast<UWheeledVehicleMovementComponent4W>(MovementComponent)->EngineSetup.MaxRPM = Savefile->GetEngineRPM();
	Cast<UWheeledVehicleMovementComponent4W>(MovementComponent)->TransmissionSetup.GearSwitchTime = Savefile->GetGearSwitchTime();
	MovementComponent->Mass = Savefile->GetMass();
	MovementComponent->DragCoefficient = Savefile->GetDragCoefficient();
	MovementComponent->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Z);
	MovementComponent->SetPlaneConstraintEnabled(true);
	//MovementComponent->bUseRVOAvoidance = true;

	max_health = Savefile->GetMaxHealth();
	temp_color = Savefile->GetPaint();
	name = *Savefile->PlayerName;

	shield_level = Savefile->CurrentArmor;
	weapon_level = Savefile->CurrentDamage;
}
void ACar::BeginPlay() {
	Super::BeginPlay();

	GetMesh()->GetBodyInstance()->AngularDamping = 3;
	GetMesh()->GetBodyInstance()->UpdateDampingProperties();

	auto center = GetMesh()->GetCenterOfMass();
	GetMesh()->SetCenterOfMass(FVector(0, 0, -center.Z / 2));
	GetMesh()->GetBodyInstance()->COMNudge = FVector(0, 0, -center.Z / 2);
	GetMesh()->GetBodyInstance()->UpdateMassProperties();

	health = max_health;
	is_alive = true;
	is_invincible = false;

	auto weapon = Cast<AWeaponInterface>(Weapon->GetChildActor());
	weapon->SetOwner(this);
	weapon->update_level(weapon_level);

	auto shield = Cast<AShield>(Shield->GetChildActor());
	shield->SetOwner(this);
	shield->update_level(shield_level);

	HealthBar->SetWidgetClass(HealthBarClass);
	HealthBar->SetDrawSize(FVector2D(400.0f, 50.0f));
	HealthBar->SetVisibility(true);
	HealthBar->RegisterComponent();
	HealthBar->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	HealthBar->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBar->SetRelativeTransform(FTransform(FVector(0, 0, 250)));

	FString command = FString::Printf(TEXT("set_value %f"), health / max_health);
	static FOutputDeviceDebug temp;
	HealthBar->GetUserWidgetObject()->CallFunctionByNameWithArguments(*command, temp, this, true);
	update_name(name);

	auto Material = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), this);
	Material->SetVectorParameterValue(FName(TEXT("Paint Color")), temp_color);
	GetMesh()->SetMaterial(0, Material);
}
void ACar::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	//auto velocity = GetVelocity();
	//GetMesh()->AddImpulse(FVector(0, 0, -velocity.Z * GetVehicleMovement()->Mass));
}

void ACar::SetupPlayerInputComponent(class UInputComponent* InputComponent) {
	Super::SetupPlayerInputComponent(InputComponent);
	/*Movement Axes*/
	InputComponent->BindAxis("MoveForward", this, &ACar::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ACar::MoveRight);

	/*Action Key Mapping*/
	InputComponent->BindAction("Handbrake", IE_Pressed, this, &ACar::HandbrakeOn);
	InputComponent->BindAction("Handbrake", IE_Released, this, &ACar::HandbrakeOff);

	InputComponent->BindAction("Weapon", IE_Pressed, this, &ACar::StartFire);
	InputComponent->BindAction("Weapon", IE_Released, this, &ACar::StopFire);

	/* UI Mappings */
	//InputComponent->BindAction("UINavigationUp", IE_Pressed, this, &AMainCharacter::UINavigationUp).bExecuteWhenPaused = true;
	//InputComponent->BindAction("UINavigationDown", IE_Pressed, this, &AMainCharacter::UINavigationDown).bExecuteWhenPaused = true;
	//InputComponent->BindAction("UINavigationLeft", IE_Pressed, this, &AMainCharacter::UINavigationLeft).bExecuteWhenPaused = true;
	//InputComponent->BindAction("UINavigationRight", IE_Pressed, this, &AMainCharacter::UINavigationRight).bExecuteWhenPaused = true;
	//InputComponent->BindAction("UISelectElement", IE_Pressed, this, &AMainCharacter::UISelectElement).bExecuteWhenPaused = true;

	InputComponent->BindAction("Pause", IE_Pressed, this, &ACar::Pause);
	InputComponent->BindAction("Restart", IE_Pressed, this, &ACar::Restart);
}
void ACar::MoveForward(float value) {
	if (is_alive)
		GetVehicleMovement()->SetThrottleInput(value);
}
void ACar::MoveRight(float value) {
	if (is_alive)
		GetVehicleMovement()->SetSteeringInput(value);
}
void ACar::HandbrakeOn() {
	if (is_alive)
		GetVehicleMovement()->SetHandbrakeInput(true);
}
void ACar::HandbrakeOff() {
	if (is_alive)
		GetVehicleMovement()->SetHandbrakeInput(false);
}
void ACar::StartFire() {
	if (fire_allowed) {
		auto weapon = Cast<AWeaponInterface>(Weapon->GetChildActor());
		if (weapon)	weapon->Activate();
	}
}
void ACar::StopFire() {
	if (fire_allowed) {
		auto weapon = Cast<AWeaponInterface>(Weapon->GetChildActor());
		if (weapon) weapon->Deactivate();
	}
}
void ACar::Pause() {
	auto controller = Cast<APlayerController>(GetController());
	if (IsValid(PauseWidget) && controller) {
		controller->SetInputMode(FInputModeUIOnly{});
		PauseWidget->AddToViewport();
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		controller->bShowMouseCursor = true;
	}	
}
void ACar::Restart() {
	if (track && GetVelocity().Size() < 500) {
		auto location = track->m_spline->FindLocationClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World);
		auto rotation = track->m_spline->FindRotationClosestToWorldLocation(location, ESplineCoordinateSpace::World);
		while (!TeleportTo(location, rotation)) {
			auto direction = track->m_spline->FindDirectionClosestToWorldLocation(location, ESplineCoordinateSpace::World);
			location = track->m_spline->FindLocationClosestToWorldLocation(location - 50 * direction, ESplineCoordinateSpace::World);
			rotation = track->m_spline->FindRotationClosestToWorldLocation(location, ESplineCoordinateSpace::World);
		}
	}
}
void ACar::ApplyDamage(float value) {
	if (is_alive) {
		auto shield = Cast<AShield>(Shield->GetChildActor());
		if (shield)
			health -= (*shield)(value);
		else
			health -= value;

		const FString command = FString::Printf(TEXT("set_value %f"), health / max_health);
		static FOutputDeviceDebug temp;
		HealthBar->GetUserWidgetObject()->CallFunctionByNameWithArguments(*command, temp, this, true);

		if (health <= 0)
			Die();
		else
			if ((shield ? (*shield)(value) : value) > max_health / 10) {
				is_invincible = true;
				GetWorld()->GetTimerManager().SetTimer(invincibility_timer, this, &ACar::OnInvincibilityEnd, 0.2f, false);
			}
	}
}
void ACar::Die() {
	if (is_alive) {
		GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Yellow, FString::Printf(TEXT("%s has been killed."), *name.ToString()));

		DestroyedCarEffect->Activate();
		ADestroyedCar* carEffect = Cast<ADestroyedCar>(DestroyedCarEffect->GetChildActor());
		if (carEffect)
			carEffect->ActivateEffect();

		GetVehicleMovement()->SetSteeringInput(0.f);
		GetVehicleMovement()->SetThrottleInput(0.f);

		is_alive = false;
	
		//GetWorld()->GetTimerManager().SetTimer(_TimerHandle, this, &ACar::OnEffectFinished, 6.0f, false);
		m_manager->has_died(this);
	}
}
void ACar::OnEffectFinished() {
	//GetWorld()->GetTimerManager().ClearTimer(_TimerHandle);
	ADestroyedCar* carEffect = Cast<ADestroyedCar>(DestroyedCarEffect->GetChildActor());
	//carEffect->DeactivateEffect();
}

void ACar::OnInvincibilityEnd() {
	GetWorld()->GetTimerManager().ClearTimer(invincibility_timer);
	is_invincible = false;
}

void ACar::OnHit(AActor *SelfActor, AActor *OtherActor,
				 FVector NormalImpulse, FHitResult const& Hit) {
	auto shield = Cast<AShield>(Shield->GetChildActor());
	if (!is_invincible && NormalImpulse.Size() > 140000.f)
		ApplyDamage((shield ? (*shield)(NormalImpulse.Size()) : NormalImpulse.Size()) / 80000.f);
	virtual_on_hit(SelfActor, OtherActor, NormalImpulse, Hit);
}

float ACar::get_speed() {
	return GetVelocity().Size2D();
}
int ACar::get_gear() {
	return GetVehicleMovement()->GetCurrentGear();
}

void ACar::update_name(FName _name) {
	name = _name;
	if (HealthBar) {
		FString command = FString::Printf(TEXT("set_name %s"), *name.ToString());
		static FOutputDeviceDebug temp;
		HealthBar->GetUserWidgetObject()->CallFunctionByNameWithArguments(*command, temp, this, true);
	}
}