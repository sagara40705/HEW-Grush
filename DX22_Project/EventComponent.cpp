// EventComponent.cpp
#include "EventComponent.h"

void EventComponent::OnDestroy()
{
    // Subject 側が消えるときに、リスナー情報を全破棄。
    // RAII 側 (EventSubscription) は weak_ptr 越しなので、
    // ここで Event が空になっても安全。
    onEvent.Clear();
}

void EventComponent::Trigger()
{
    onEvent.Invoke();
}
