#include "EnemyUpdateComponent.h"

#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/UpdateComponent.h>
#include <NiEngine/AnimatedGraphicsComponent.h>
#include <NiEngine/Animation.h>
#include <NiEngine/TransformComponent.h>
#include <NiEngine/MathUtility.h>

#include "PlatformerGameMode.h"
#include "CharacterPhysicsComponent.h"

EnemyUpdateComponent::EnemyUpdateComponent(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> owner_id) 
	: ni::UpdateComponent(component_locator)
{
	owner_id_ = owner_id;
}

void EnemyUpdateComponent::Init(ni::AnimatedGraphicsComponent& graphics, CharacterPhysicsComponent& physics)
{
	ni::Animation walk_animation;
	walk_animation.key_ = kWalkAnimationKey;
	walk_animation.start_frame = 1;
	walk_animation.frame_count = 2;
	walk_animation.animation_row = kAnimationRow;

	graphics.RegisterAnimation(walk_animation);
}

void EnemyUpdateComponent::Update()
{
	int dir = GetDirectionToPlayer();

	auto physics = static_cast<CharacterPhysicsComponent*>(component_locator_.GetPhysicsComponent(owner_id_));
	if (physics)
	{
		physics->Move(dir);
	}
	auto graphics = component_locator_.GetFirstAnimatedGraphicsComponent(owner_id_);
	if (!graphics)
	{
		return;
	}
	if (dir == 0)
	{
		graphics->SetFrame(kAnimationRow, 0);
		return;
	}
	graphics->Play(kWalkAnimationKey, .1, true);
}

int EnemyUpdateComponent::GetDirectionToPlayer()
{
	if (player_id_.id_ == -1) player_id_ = component_locator_.GetIdByTag(PlatformerGameMode::kPlayerTag);

	ni::TransformComponent* transform        = component_locator_.GetTransformComponent(owner_id_ );
	ni::TransformComponent* player_transform = component_locator_.GetTransformComponent(player_id_);

	int sign = ni::MathUtility::GetSign(player_transform->GetTransformable().getPosition().x - transform->GetTransformable().getPosition().x);

	return sign;
}
