#include "WalkerEnemyUpdateComponent.h"

#include <NiEngine/AnimatedGraphicsComponent.h>
#include <NiEngine/Animation.h>
#include <NiEngine/UpdateComponent.h>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/MathUtility.h>

#include "CharacterPhysicsComponent.h"
#include "PlatformerGameMode.h"
#include "PlayerUpdateComponent.h"
#include "EnemyUpdateComponent.h"
#include <NiEngine/TransformComponent.h>

WalkerEnemyUpdateComponent::WalkerEnemyUpdateComponent(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> owner_id) 
	: EnemyUpdateComponent(component_locator, owner_id)
{}

void WalkerEnemyUpdateComponent::Init(ni::AnimatedGraphicsComponent& graphics, CharacterPhysicsComponent& physics)
{
	ni::Animation walk_animation;
	walk_animation.key_ = kWalkAnimationKey;
	walk_animation.start_frame = 1;
	walk_animation.frame_count = 2;
	walk_animation.animation_row = kAnimationRow;

	graphics.RegisterAnimation(walk_animation);
}

void WalkerEnemyUpdateComponent::Update()
{
	auto graphics = component_locator_.GetFirstAnimatedGraphicsComponent(owner_id_);
	auto physics  = static_cast<CharacterPhysicsComponent*>(component_locator_.GetPhysicsComponent(owner_id_));
	if (is_dead_)
	{
		physics->Move(0);
		graphics->SetFrame(kAnimationRow, 4);
		return;
	}
	UpdatePlayerTrackingStatus();
	if (distance_to_player_ < 16.0f)
	{
		EnemyUpdateComponent::Update();
		return;
	}
	if (horizontal_distance_to_player_ < 8.0f)
	{
		return;
	}
	int dir = ni::MathUtility::GetSign(direction_to_player_.x);

	if (physics)
	{
		physics->Move(dir);
	}
	if (dir == 0)
	{
		graphics->SetFrame(kAnimationRow, 0);
		return;
	}
	graphics->Play(kWalkAnimationKey, .1, true);
}

void WalkerEnemyUpdateComponent::CollideSides()
{	
	KillPlayer();
}

void WalkerEnemyUpdateComponent::CollideTop()
{
	auto player_physics = static_cast<CharacterPhysicsComponent*>(component_locator_.GetPhysicsComponent(player_id_));

	if (player_physics->GetVelocity().y < 0)
	{
		return;
	}
	is_dead_ = true;
	player_physics->ForceJump();
}

void WalkerEnemyUpdateComponent::CollideBottom()
{
	KillPlayer();
}

void WalkerEnemyUpdateComponent::KillPlayer()
{
	if (player_id_.id_ == -1)
	{
		player_id_ = component_locator_.GetIdByTag(PlatformerGameMode::kPlayerTag);
	}
	ni::UpdateComponent* update = component_locator_.GetUpdateComponent(player_id_);
	auto player_update = static_cast<PlayerUpdateComponent*>(update);
	player_update->Die();
}
