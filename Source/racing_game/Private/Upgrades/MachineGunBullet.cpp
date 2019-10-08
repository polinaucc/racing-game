#include "MachineGunBullet.h"
#include "UObject/ConstructorHelpers.h"
#include "WeaponInterface.h"
#include "Components/StaticMeshComponent.h"

AMachineGunBullet::AMachineGunBullet() {
	PrimaryActorTick.bCanEverTick = false;

	ConstructorHelpers::FObjectFinder<UStaticMesh> bulletMesh(TEXT("StaticMesh'/Game/Props/Bullet/762x51_bullet_Bullet.762x51_bullet_Bullet'"));
	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMesh->SetStaticMesh(bulletMesh.Object);
	BulletMesh->SetRelativeScale3D(FVector(bullet_scale, bullet_scale, bullet_scale));
	BulletMesh->SetRelativeLocation(FVector(0.f, 0.f, 200.f));
	BulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BulletMesh->SetCollisionProfileName(TEXT("OverlapAll"));
	RootComponent = BulletMesh;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetSphereRadius(150.0f);
	CollisionSphere->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	CollisionSphere->SetCollisionProfileName("Projectile");
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ABulletInterface::OnOverlap);
	CollisionSphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 4000.f;
	ProjectileMovement->MaxSpeed = 10000.f;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.05f;

	//Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}
void AMachineGunBullet::BeginPlay() {
	Super::BeginPlay();
	if (GetOwner()->IsA(AWeaponInterface::StaticClass())) {
		auto weapon = Cast<AWeaponInterface>(GetOwner());
		damage = weapon->damage();
		BulletMesh->SetRelativeRotation(weapon->GetOwner()->GetActorRotation() + FRotator(0, -90, 0));
	}
}
void AMachineGunBullet::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

FVector AMachineGunBullet::next_position(FVector location, FVector direction, bool left_or_right) {
	auto shift = 50.f * (left_or_right ? +1.f : -1.f) * FVector(direction.Y, -direction.X, 0);
	return location + shift;
}
