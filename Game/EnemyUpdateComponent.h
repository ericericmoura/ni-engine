#pragma once

#include <string>

#include <NiEngine/UpdateComponent.h>
#include <NiEngine/AnimatedGraphicsComponent.h>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>

#include "CharacterPhysicsComponent.h"

class EnemyUpdateComponent : public ni::UpdateComponent
{
public:
	EnemyUpdateComponent(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> owner_id);

	void Init(ni::AnimatedGraphicsComponent& graphics, CharacterPhysicsComponent& physics);
	void Update() override;

	int  GetDirectionToPlayer();

private: 
	inline static const int kAnimationRow = 17;
	inline static const std::string kWalkAnimationKey = "walk";

	ni::Id<ni::GameObjectTag> player_id_;

	virtual void CollideSides();
	virtual void CollideTop();
	virtual void CollideBottom();

	void CheckCollisionWithPlayer();
};

