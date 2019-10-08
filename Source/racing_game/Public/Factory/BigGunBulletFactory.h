#pragma once
#include "CoreMinimal.h"
#include "AbstractBulletFactory.h"

class RACING_GAME_API BigGunBulletFactory : public AbstractBulletFactory {
public:
	virtual ABulletInterface* create(EEquipementLevel level, size_t counter, AActor *owner) override;
};