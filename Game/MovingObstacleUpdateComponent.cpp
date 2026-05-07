#include "MovingObstacleUpdateComponent.h"

#include <cmath>
#include <algorithm>
#include <cstdlib>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/TransformComponent.h>
#include <NiEngine/Engine.h>

#include "ObstacleUpdateComponent.h"
#include "PlatformerGameMode.h"

MovingObstacleUpdateComponent::MovingObstacleUpdateComponent(
	ni::ComponentLocator& component_locator, 
	ni::TransformComponent& transform,
	ni::Id<ni::GameObjectTag> id, 
	sf::Vector2i position_movement_offset, 
	float movement_trigger_distance,
	sf::Vector2f collision_box_size,
	float delay_in_seconds
) : ObstacleUpdateComponent(component_locator, id, collision_box_size)
{
	delay_in_seconds_ = delay_in_seconds;
	movement_position_offset_  = position_movement_offset;
	movement_trigger_distance_ = movement_trigger_distance;
	
	sf::Vector2f current_position = transform.GetTransformable().getPosition();
	target_position_ = current_position + sf::Vector2f(movement_position_offset_);	
	start_position_  = current_position;
}

void MovingObstacleUpdateComponent::Update()
{
	if (moving_)
	{
		Move();
	}
	else if (!moved_)
	{
		LocatePlayer();
	}
	ObstacleUpdateComponent::Update();
}

void MovingObstacleUpdateComponent::CollideTop(sf::FloatRect collision_box)
{	
	for (auto& collision : collision_components_)
	{
		collision->SolveTopCollision(collision_box, component_locator_, player_id_);
	}
	
	if (moving_)
	{
		ni::TransformComponent* player_transform = component_locator_.GetTransformComponent(player_id_);
		if (player_transform) player_transform->GetTransformable().move({ amount_moved_x_, 0});
	}
}

void MovingObstacleUpdateComponent::LocatePlayer()
{
	if (player_id_.id_ == -1)
	{
		player_id_ = component_locator_.GetIdByTag(PlatformerGameMode::kPlayerTag);
	}
	ni::TransformComponent* transform        = component_locator_.GetTransformComponent(owner_id_);
	ni::TransformComponent* player_transform = component_locator_.GetTransformComponent(player_id_);	

	float position_x		= transform->GetTransformable().getPosition().x + collision_box_size_.x / 2.0f;
	float player_position_x = player_transform->GetTransformable().getPosition().x;

	float distance = std::max(position_x, player_position_x) - std::min(position_x, player_position_x);

	if (distance <= movement_trigger_distance_)
	{
		TriggerMovement();
	}
}

void MovingObstacleUpdateComponent::TriggerMovement()
{
	moving_ = true;
	moved_  = true;
	time_since_movement_started_ = ni::Engine::time_elapsed;
}

void MovingObstacleUpdateComponent::Move()
{
	ni::TransformComponent* transform = component_locator_.GetTransformComponent(owner_id_);
	if (target_position_ == transform->GetTransformable().getPosition())
	{
		return;
	}
	float time_passed = (ni::Engine::time_elapsed - time_since_movement_started_).asSeconds();
	if (time_passed > delay_in_seconds_)
	{
		moving_ = false;
	}
	sf::Vector2f old_pos = transform->GetTransformable().getPosition();
	sf::Vector2f new_pos = transform->GetTransformable().getPosition();

	new_pos.x = std::lerp(start_position_.x, target_position_.x, time_passed / delay_in_seconds_);
	new_pos.y = std::lerp(start_position_.y, target_position_.y, time_passed / delay_in_seconds_);

	amount_moved_x_ = new_pos.x - old_pos.x;

	transform->GetTransformable().setPosition(new_pos);
}
