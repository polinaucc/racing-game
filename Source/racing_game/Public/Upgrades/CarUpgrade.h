#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Save.h"
#include "CarUpgrade.generated.h"

UCLASS()
class RACING_GAME_API ACarUpgrade : public AActor {
	GENERATED_BODY()
protected:
	EEquipementLevel m_level;
public:	
	ACarUpgrade();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	EEquipementLevel level() const { return m_level; }
	void update_level(EEquipementLevel new_level) {
		m_level = new_level;
		virtual_update_level(new_level);
	}
	virtual void virtual_update_level(EEquipementLevel new_level) {}
};
