#pragma once
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Containers/StaticArray.h"
#include "Save.generated.h"

UENUM(BlueprintType)
enum class EEquipementLevel : uint8 {
	L0 UMETA(DisplayName = "Level 0"),
	L1 UMETA(DisplayName = "Level 1"),
	L2 UMETA(DisplayName = "Level 2"),
	L3 UMETA(DisplayName = "Level 3"),
	L4 UMETA(DisplayName = "Level 4"),
	L5 UMETA(DisplayName = "Level 5")
};
int switch_EEquipementLevel(EEquipementLevel level);
EEquipementLevel make_EEquipementLevel(int level);

UCLASS()
class RACING_GAME_API USave : public USaveGame {
	GENERATED_BODY()

public:
	USave();
	USave(EEquipementLevel default_level);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic")
	FString PlayerName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic")
	FString SaveSlotName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic")
	int32 UserIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic")
	float record;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Money")
	int32 Money;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Currently Equipped")
	EEquipementLevel CurrentEngineRPM;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bought Equipment")
	TArray<bool> EngineRPM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Currently Equipped")
	EEquipementLevel CurrentGearSwitchTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bought Equipment")
	TArray<bool> GearSwitchTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Currently Equipped")
	EEquipementLevel CurrentMass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bought Equipment")
	TArray<bool> Mass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Currently Equipped")
	EEquipementLevel CurrentDragCoefficient;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bought Equipment")
	TArray<bool> DragCoefficient;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Currently Equipped")
	EEquipementLevel CurrentMaximumSteerAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bought Equipment")
	TArray<bool> MaximumSteerAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Currently Equipped")
	EEquipementLevel CurrentFrictionScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bought Equipment")
	TArray<bool> FrictionScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Currently Equipped")
	EEquipementLevel CurrentLatStiffness;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bought Equipment")
	TArray<bool> LatStiffness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Currently Equipped")
	EEquipementLevel CurrentArmor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bought Equipment")
	TArray<bool> Armor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Currently Equipped")
	EEquipementLevel CurrentDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bought Equipment")
	TArray<bool> Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Currently Equipped")
	EEquipementLevel CurrentMaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bought Equipment")
	TArray<bool> MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Currently Equipped")
	int CurrentPaint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bought Equipment")
	TArray<bool> Paint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Currently Equipped")
	int CurrentAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bought Equipment")
	TArray<bool> Ammo;

	UFUNCTION(BlueprintCallable)
	static float ConvertEngineRPM(EEquipementLevel level);
	UFUNCTION(BlueprintCallable)
	static float ConvertGearSwitchTime(EEquipementLevel level);
	UFUNCTION(BlueprintCallable)
	static float ConvertMass(EEquipementLevel level);
	UFUNCTION(BlueprintCallable)
	static float ConvertDragCoefficient(EEquipementLevel level);
	UFUNCTION(BlueprintCallable)
	static float ConvertMaximumSteerAngle(EEquipementLevel level);
	UFUNCTION(BlueprintCallable)
	static float ConvertFrictionScale(EEquipementLevel level);
	UFUNCTION(BlueprintCallable)
	static float ConvertLatStiffness(EEquipementLevel level);
	UFUNCTION(BlueprintCallable)
	static float ConvertArmor(EEquipementLevel level);
	UFUNCTION(BlueprintCallable)
	static float ConvertDamage(EEquipementLevel level);
	UFUNCTION(BlueprintCallable)
	static float ConvertMaxHealth(EEquipementLevel level);
	UFUNCTION(BlueprintCallable)
	static FLinearColor ConvertPaint(int level);

	UFUNCTION(BlueprintCallable)
	static float Price(EEquipementLevel level);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Money")
	float PaintPrice;
	float AmmoPrice;
	
	UFUNCTION(BlueprintCallable)
	float GetEngineRPM() {
		return ConvertEngineRPM(CurrentEngineRPM);
	}
	UFUNCTION(BlueprintCallable)
	float GetGearSwitchTime() {
		return ConvertGearSwitchTime(CurrentGearSwitchTime);
	}
	UFUNCTION(BlueprintCallable)
	float GetMass() {
		return ConvertMass(CurrentMass);
	}
	UFUNCTION(BlueprintCallable)
	float GetDragCoefficient() {
		return ConvertDragCoefficient(CurrentDragCoefficient);
	}
	UFUNCTION(BlueprintCallable)
	float GetMaximumSteerAngle() {
		return ConvertMaximumSteerAngle(CurrentMaximumSteerAngle);
	}
	UFUNCTION(BlueprintCallable)
	float GetFrictionScale() {
		return ConvertFrictionScale(CurrentFrictionScale);
	}
	UFUNCTION(BlueprintCallable)
	float GetLatStiffness() {
		return ConvertLatStiffness(CurrentLatStiffness);
	}
	UFUNCTION(BlueprintCallable)
	float GetArmor() {
		return ConvertArmor(CurrentArmor);
	}
	UFUNCTION(BlueprintCallable)
	float GetDamage() {
		return ConvertDamage(CurrentDamage);
	}
	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() {
		return ConvertMaxHealth(CurrentMaxHealth);
	}
	UFUNCTION(BlueprintCallable)
	FLinearColor GetPaint() {
		return ConvertPaint(CurrentPaint);
	}

	UFUNCTION(BlueprintCallable)
	float GetByID(int id);
	UFUNCTION(BlueprintCallable)
	EEquipementLevel& CurrentByID(int id);
	UFUNCTION(BlueprintCallable)
	TArray<bool>& AvailabilityByID(int id);
};
