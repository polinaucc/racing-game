#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
//#include "Car.h"
#include <chrono>
#include "Templates/Tuple.h"
#include "CarManager.generated.h"

class ACar;
class ACarAI;

UCLASS()
class RACING_GAME_API ACarManager : public AActor {
	GENERATED_BODY()
public:	
	ACarManager();

protected:
	virtual void BeginPlay() override;
	 
	FTimerHandle TimerHandle;
	std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
	class UGameData *m_data;

public:	
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, Category = "TrackData")
	class ATrackSpline *track;

	UPROPERTY(EditAnywhere, Category = "TreeData")
	class ATrees *trees;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget *PauseWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget *HUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget *VictoryWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget *DefeatWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USphereComponent* sphere;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicles")
	TArray<TTuple<ACar*, int32>> cars;

	UFUNCTION()
	void OnOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, 
				   int32 OtherBodyIndex, bool bFromSweep, FHitResult const& SweepResult);

	UFUNCTION()
	void OutOfTime();

	UFUNCTION()
	void Win(ACar *car);
	UFUNCTION()
	void Lose();
	UFUNCTION()
	void has_died(ACar *car);
	UFUNCTION()
	void end_level();
	
	UFUNCTION(BlueprintImplementableEvent)
	void WinEvent();
	UFUNCTION(BlueprintImplementableEvent)
	void LoseEvent();

	UFUNCTION()
	class ACar* spawn_a_car(int index, bool ai = false);
};
