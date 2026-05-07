#pragma once

#include <SFML/System/Vector2.hpp>
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

	virtual void Init(ni::AnimatedGraphicsComponent& graphics, CharacterPhysicsComponent& physics) = 0;
	virtual void Update() override;

protected:
	bool is_dead_ = false;
	ni::Id<ni::GameObjectTag> player_id_;

	float        distance_to_player_  = 0.0f;
	sf::Vector2f direction_to_player_ = {};

	void UpdatePlayerTrackingStatus();

private: 
	virtual void CollideSides()  = 0;
	virtual void CollideTop()    = 0;
	virtual void CollideBottom() = 0;

	void CheckCollisionWithPlayer();
};

