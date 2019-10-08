#pragma once
#include "CoreMinimal.h"
#include "Save.h"

class RACING_GAME_API AbstractBulletFactory {
public:
	virtual class ABulletInterface* create(EEquipementLevel level, size_t counter, AActor *owner) = 0;
};