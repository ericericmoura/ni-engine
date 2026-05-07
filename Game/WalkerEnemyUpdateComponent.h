#pragma once

#include <string>

#include <NiEngine/AnimatedGraphicsComponent.h>

#include "EnemyUpdateComponent.h"
#include "CharacterPhysicsComponent.h"

#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>

class WalkerEnemyUpdateComponent : public EnemyUpdateComponent
{
public:
	WalkerEnemyUpdateComponent(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> owner_id, int start_movement_direction);
	void Init(ni::AnimatedGraphicsComponent& graphics, CharacterPhysicsComponent& physics) override;
	void Update() override;

private:
	inline static const int kAnimationRow = 17;
	inline static const std::string kWalkAnimationKey = "walk";

	bool played_death_animation_ = false;
	bool airborne_ = false;
	int movement_direction = 1;

	void CollideSides() override;
	void CollideTop() override;
	void CollideBottom() override;

	void KillPlayer();
	void PlayDeathAnimation(CharacterPhysicsComponent& physics);
};

