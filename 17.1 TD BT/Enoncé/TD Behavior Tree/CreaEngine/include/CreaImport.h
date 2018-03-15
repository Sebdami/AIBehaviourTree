#ifndef _CREAIMPORT_H
#define _CREAIMPORT_H

namespace crea
{

#   ifdef CREAENGINE_EXPORTS
#       define CREAENGINE_API __declspec(dllexport)
#   else
#       define CREAENGINE_API __declspec(dllimport)
#   endif

} // namespace crea

#define INVALID_OBJECT_ID 0
typedef unsigned int objectID;

#include "Core\GameManager.h"
#include "Core\SceneManager.h"
#include "Core\DataManager.h"
#include "Core\EntityManager.h"
#include "Core\Scene.h"
#include "Core\Math.h"
#include "Data\Entity.h"
#include "Data\Map.h"
#include "Data\Animation.h"
#include "Data\Node.h"
#include "Data\Cluster.h"
#include "Graphics\Color.h"
#include "Graphics\Font.h"
#include "Graphics\Text.h"
#include "Graphics\Texture.h"
#include "Graphics\Sprite.h"
#include "Graphics\Shape.h"
#include "Graphics\SpriteRenderer.h"
#include "Graphics\TextRenderer.h"
#include "Graphics\MapRenderer.h"
#include "Graphics\Animator.h"
#include "AI\Agent.h"
#include "AI\ActionTable.h"
#include "AI\Steering\Steering.h"
#include "AI\Steering\Vehicle.h"
#include "AI\BehaviorTree\Behavior.h"
#include "AI\BehaviorTree\BehaviorTree.h"
#include "Core\PhysicsManager.h"
#include "Physics\Collider.h"

#   endif // _CREAIMPORT_H