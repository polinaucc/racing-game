#include "BigGunBullet.h"
#include "UObject/ConstructorHelpers.h"
#include "WeaponInterface.h"
#include "Components/StaticMeshComponent.h"

ABigGunBullet::ABigGunBullet() {
	PrimaryActorTick.bCanEverTick = false;
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> bulletMesh(TEXT("StaticMesh'/Game/Props/Bullet/762x51_bullet_Bullet.762x51_bullet_Bullet'"));
	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMesh->SetStaticMesh(bulletMesh.Object);
	BulletMesh->SetRelativeScale3D(FVector(.4f, .4f, .4f));
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
	ProjectileMovement->InitialSpeed = 1500.f;
	ProjectileMovement->MaxSpeed = 10000.f;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.05f;

	//Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}
void ABigGunBullet::BeginPlay() {
	Super::BeginPlay();
	if (GetOwner()->IsA(AWeaponInterface::StaticClass())) {
		auto weapon = Cast<AWeaponInterface>(GetOwner());
		damage = weapon->damage();
		BulletMesh->SetRelativeRotation(weapon->GetOwner()->GetActorRotation() + FRotator(0, -90, 0));
	}

	float bullet_scale = .4f;
	auto weapon = Cast<AWeaponInterface>(GetOwner());
	if (weapon)
		bullet_scale += switch_EEquipementLevel(weapon->level()) * .1f;

	auto mesh = Cast<UStaticMeshComponent>(RootComponent);
	if (mesh)
		mesh->SetRelativeScale3D(FVector(bullet_scale, bullet_scale, bullet_scale));
}
void ABigGunBullet::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}