#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/AnimatedGraphicsComponent.h>

#include "EnemyUpdateComponent.h"
#include "CharacterPhysicsComponent.h"

class JumperEnemyUpdateComponent : public EnemyUpdateComponent
{
public:
	JumperEnemyUpdateComponent(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> owner_id, sf::Vector2f enemy_size);
	void Init(ni::AnimatedGraphicsComponent& graphics, CharacterPhysicsComponent& physics) override;
	void Update() override;

private:
	inline static const int kAnimationRow = 18;

	bool upside_down_ = false;
	bool dead_ = false;

	int last_facing_direction_ = 0;

	float jump_cooldown_ = 2.0f;
	sf::Time last_time_since_jumping_ = {};

	void CollideSides() override;
	void CollideTop() override;
	void CollideBottom() override;
};

