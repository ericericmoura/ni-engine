#pragma once

#include <string>

#include <NiEngine/AnimatedGraphicsComponent.h>

#include "EnemyUpdateComponent.h"
#include "CharacterPhysicsComponent.h"

class WalkerEnemyUpdateComponent : public EnemyUpdateComponent
{
public:
	WalkerEnemyUpdateComponent(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> owner_id);
	void Init(ni::AnimatedGraphicsComponent& graphics, CharacterPhysicsComponent& physics) override;
	void Update() override;

private:
	inline static const int kAnimationRow = 17;
	inline static const std::string kWalkAnimationKey = "walk";

	void CollideSides() override;
	void CollideTop() override;
	void CollideBottom() override;

	void KillPlayer();
};

