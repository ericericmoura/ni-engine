#pragma once

#include <vector>
#include <memory>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <NiEngine/UpdateComponent.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/Id.h>

#include "ObstacleCollisionComponent.h"

class ObstacleUpdateComponent : public ni::UpdateComponent
{
public:
	ObstacleUpdateComponent(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> id, sf::Vector2f collision_box_size);
	
	void RegisterCollisionComponent(std::unique_ptr<ObstacleCollisionComponent> collision_component);
	virtual void Update() override;

protected:
	std::vector<std::unique_ptr<ObstacleCollisionComponent>> collision_components_;

	ni::Id<ni::GameObjectTag> player_id_;
	sf::Vector2f collision_box_size_;
	
	virtual void CollideTop	  (sf::FloatRect collision_box);
	virtual void CollideBottom(sf::FloatRect collision_box);
	virtual void CollideFront (sf::FloatRect collision_box);
	virtual void CollideLeft  (sf::FloatRect collision_box);
	virtual void CollideRight (sf::FloatRect collision_box);

private:
	bool colliding_ = false;

	void HandleCollisions();
};

