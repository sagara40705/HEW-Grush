#include "GameObject.h"
#include "World.h"
#include "Transform.h"
#include "Camera.h"
#include "MeshFilter.h"
#include "MeshRender.h"
#include "BoxCollider.h"
#include "Rigidbody.h"
#include "DebugLog.h"

void GameObject::Destroy(float time)
{
	world->RequestDestroy(id);
}
