#include "ObstacleUpdateComponent.h"

#include <memory>
#include <utility>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/UpdateComponent.h>
#include <NiEngine/TransformComponent.h>

#include "CharacterPhysicsComponent.h"
#include "ObstacleCollisionComponent.h"
#include "PlatformerGameMode.h"

ObstacleUpdateComponent::ObstacleUpdateComponent(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> id, sf::Vector2f collision_box_size) 
	: ni::UpdateComponent(component_locator)
{
	owner_id_ = id;
	collision_box_size_ = collision_box_size;	
}

void ObstacleUpdateComponent::RegisterCollisionComponent(std::unique_ptr<ObstacleCollisionComponent> collision_component)
{
	collision_components_.push_back(std::move(collision_component));
}

void ObstacleUpdateComponent::Update()
{
	HandleCollisions();
}

void ObstacleUpdateComponent::CollideTop(sf::FloatRect collision_box)
{
	for (auto& collision : collision_components_)
	{
		collision->SolveTopCollision(collision_box, component_locator_, player_id_);
	}
}

void ObstacleUpdateComponent::CollideBottom(sf::FloatRect collision_box)
{
	for (auto& collision : collision_components_)
	{
		collision->SolveBottomCollision(collision_box, component_locator_, player_id_);
	}
}

void ObstacleUpdateComponent::CollideFront(sf::FloatRect collision_box)
{
	for (auto& collision : collision_components_)
	{
		collision->SolveFrontCollision(collision_box, component_locator_, player_id_);
	}
}

void ObstacleUpdateComponent::CollideLeft(sf::FloatRect collision_box)
{
	for (auto& collision : collision_components_)
	{
		collision->SolveLeftCollision(collision_box, component_locator_, player_id_);
	}
}

void ObstacleUpdateComponent::CollideRight(sf::FloatRect collision_box)
{
	for (auto& collision : collision_components_)
	{
		collision->SolveRightCollision(collision_box, component_locator_, player_id_);
	}
}

void ObstacleUpdateComponent::HandleCollisions()
{
	if (player_id_.id_ == -1)
	{
		player_id_ = component_locator_.GetIdByTag(PlatformerGameMode::kPlayerTag);
	}

	ni::TransformComponent* transform        = component_locator_.GetTransformComponent(owner_id_ );
	ni::TransformComponent* player_transform = component_locator_.GetTransformComponent(player_id_);
	auto player_physics = static_cast<CharacterPhysicsComponent*>(component_locator_.GetPhysicsComponent(player_id_));

	sf::FloatRect collision_box;
	collision_box.position = transform->GetTransformable().getPosition();
	collision_box.size     = collision_box_size_;

	if (collision_box.findIntersection(player_physics->GetFeetBounds(player_transform->GetTransformable().getPosition())))
	{		
		CollideTop(collision_box);

		colliding_ = true;
	}
	else if (colliding_)
	{
		for (auto& collision : collision_components_)
		{
			collision->SolveTopCollisionLost(collision_box, component_locator_, player_id_);
		}
		colliding_ = false;
	}

	if (collision_box.findIntersection(player_physics->GetFrontBounds(player_transform->GetTransformable().getPosition())))
	{
		CollideFront(collision_box);
	}
	else if (collision_box.findIntersection(player_physics->GetSideBounds(player_transform->GetTransformable().getPosition(), -1)))
	{
		CollideLeft(collision_box);
	}
	else if (collision_box.findIntersection(player_physics->GetSideBounds(player_transform->GetTransformable().getPosition(), 1)))
	{
		CollideRight(collision_box);
	}
	if (collision_box.findIntersection(player_physics->GetHeadBounds(player_transform->GetTransformable().getPosition())))
	{
		CollideBottom(collision_box);
	}
}
