#include "Component.h"
#include <cassert>

void Component::RootStart()
{
	//assert(!onStart && "Start() called twice on a Component");
	if (onStart) return;

	onStart = true;
	Start();
}

void Component::RootOnDestroy()
{
	//assert(!onDestroy && "OnDestroy() called twice on a Component");
	if (onDestroy) return;

	onDestroy = true;
	OnDestroy();
}
