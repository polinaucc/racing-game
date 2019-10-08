#include "MultiGunBullet.h"
#include "UObject/ConstructorHelpers.h"
#include "WeaponInterface.h"
#include "Components/StaticMeshComponent.h"

AMultiGunBullet::AMultiGunBullet() {
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
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 10000.f;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.05f;

	//Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}
void AMultiGunBullet::BeginPlay() {
	Super::BeginPlay();
	if (GetOwner()->IsA(AWeaponInterface::StaticClass())) {
		auto weapon = Cast<AWeaponInterface>(GetOwner());
		damage = weapon->damage();
		BulletMesh->SetRelativeRotation(weapon->GetOwner()->GetActorRotation() + FRotator(0, -90, 0));
	}
}
void AMultiGunBullet::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

std::pair<FVector, FRotator> AMultiGunBullet::next_position(FVector location, FRotator rotation, size_t counter) {
	float angle = 3.f * (counter % 2 ? float(counter) : -float(counter));
	return std::make_pair(location, rotation + FRotator(0.f, angle, 0.f));
}
