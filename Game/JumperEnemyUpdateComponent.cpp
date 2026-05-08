#include "JumperEnemyUpdateComponent.h"

#include <SFML/System/Vector2.hpp>
#include <NiEngine/AnimatedGraphicsComponent.h>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/MathUtility.h>
#include <NiEngine/Engine.h>

#include "EnemyUpdateComponent.h"
#include "CharacterPhysicsComponent.h"

JumperEnemyUpdateComponent::JumperEnemyUpdateComponent(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> owner_id, sf::Vector2f enemy_size)
	: EnemyUpdateComponent(component_locator, owner_id, enemy_size)
{}

void JumperEnemyUpdateComponent::Init(ni::AnimatedGraphicsComponent& graphics, CharacterPhysicsComponent& physics)
{
	physics.OnLanding([this]() {
		if (is_dead_)
		{
			played_death_animation_ = true;
		}
		auto graphics = component_locator_.GetFirstAnimatedGraphicsComponent(owner_id_);
		graphics->SetFrame(kAnimationRow, 4);
	});

	physics.OnJumping([this]() {
		auto graphics = component_locator_.GetFirstAnimatedGraphicsComponent(owner_id_);
		graphics->SetFrame(kAnimationRow, 3);
	});
}

void JumperEnemyUpdateComponent::Update()
{
	auto physics = static_cast<CharacterPhysicsComponent*>(component_locator_.GetPhysicsComponent(owner_id_));

	if (dead_ || upside_down_)
	{
		if (!played_death_animation_)
		{
			PlayKnockbackAnimation(*physics, -last_facing_direction_);
		}
		return;
	}
	UpdatePlayerTrackingStatus();	

	float time_elapsed_since_last_jump = (ni::Engine::time_elapsed - last_time_since_jumping_).asSeconds();
	if (time_elapsed_since_last_jump > jump_cooldown_)
	{
		physics->Jump();
		last_time_since_jumping_ = ni::Engine::time_elapsed;
	}

	if (airborne_)
	{
		last_facing_direction_ = ni::MathUtility::GetSign(direction_to_player_.x);
		physics->Move(last_facing_direction_);
	}
}

void JumperEnemyUpdateComponent::CollideSides()
{
	KillPlayer();
}

void JumperEnemyUpdateComponent::CollideTop()
{
	if (dead_)
	{
		return;
	}
	auto graphics = component_locator_.GetFirstAnimatedGraphicsComponent(owner_id_);
	if (!upside_down_)
	{
		graphics->SetFrame(kAnimationRow, 6);
		upside_down_ = true;
		return;
	}
	graphics->SetFrame(kAnimationRow, 5);
	dead_ = true;
}

void JumperEnemyUpdateComponent::CollideBottom()
{
	KillPlayer();
}
