#include "EnemyUpdateComponent.h"

#include <cstdlib>

#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/UpdateComponent.h>
#include <NiEngine/TransformComponent.h>

#include "PlatformerGameMode.h"
#include "CharacterPhysicsComponent.h"

EnemyUpdateComponent::EnemyUpdateComponent(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> owner_id) 
	: ni::UpdateComponent(component_locator)
{
	owner_id_ = owner_id;
}

void EnemyUpdateComponent::Update()
{
	CheckCollisionWithPlayer();
}

void EnemyUpdateComponent::CheckCollisionWithPlayer()
{
	if (is_dead_)
	{
		return;
	}
	if (player_id_.id_ == -1)
	{
		player_id_ = component_locator_.GetIdByTag(PlatformerGameMode::kPlayerTag);
	}
	auto physics        = static_cast<CharacterPhysicsComponent*>(component_locator_.GetPhysicsComponent(owner_id_ ));
	auto player_physics = static_cast<CharacterPhysicsComponent*>(component_locator_.GetPhysicsComponent(player_id_));
	
	ni::TransformComponent* transform = component_locator_.GetTransformComponent(owner_id_);
	ni::TransformComponent* player_transform = component_locator_.GetTransformComponent(player_id_);	

	if (physics->GetFeetBounds(transform->GetTransformable().getPosition()).findIntersection(player_physics->GetHeadBounds(player_transform->GetTransformable().getPosition())))
	{
		CollideBottom();
	}
	if (physics->GetSideBounds(transform->GetTransformable().getPosition(),  1).findIntersection(player_physics->GetSideBounds(player_transform->GetTransformable().getPosition(), -1)) ||
		physics->GetSideBounds(transform->GetTransformable().getPosition(), -1).findIntersection(player_physics->GetSideBounds(player_transform->GetTransformable().getPosition(), 1)))
	{
		CollideSides();
	}
	if (physics->GetHeadBounds(transform->GetTransformable().getPosition()).findIntersection(player_physics->GetFeetBounds(player_transform->GetTransformable().getPosition())))
	{
		CollideTop();
	}
}

void EnemyUpdateComponent::UpdatePlayerTrackingStatus()
{
	if (player_id_.id_ == -1)
	{
		player_id_ = component_locator_.GetIdByTag(PlatformerGameMode::kPlayerTag);
	}
	ni::TransformComponent* transform        = component_locator_.GetTransformComponent(owner_id_);
	ni::TransformComponent* player_transform = component_locator_.GetTransformComponent(player_id_);

	direction_to_player_ = player_transform->GetTransformable().getPosition() - transform->GetTransformable().getPosition();
	
	distance_to_player_  = direction_to_player_.length();
	horizontal_distance_to_player_ = std::abs(direction_to_player_.x);
	vertical_distance_to_player_   = std::abs(direction_to_player_.y);

	direction_to_player_ = direction_to_player_.normalized();
}
