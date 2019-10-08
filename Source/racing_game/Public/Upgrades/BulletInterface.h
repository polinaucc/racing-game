#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "save.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BulletInterface.generated.h"

UCLASS()
class RACING_GAME_API ABulletInterface : public AActor {
	GENERATED_BODY()
protected:
	static EEquipementLevel level;
public:	
	ABulletInterface();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Damage")
	float damage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UArrowComponent* ArrowComponent; //задання вектору руху

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* BulletMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	USphereComponent* CollisionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	UProjectileMovementComponent* ProjectileMovement; //компонент, що керуэ перемыщенням

	UPROPERTY(BlueprintReadOnly, Category = "Material")
	UMaterialInstanceDynamic* DynamicInstance; //матеріал

	UFUNCTION()
	void OnOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
