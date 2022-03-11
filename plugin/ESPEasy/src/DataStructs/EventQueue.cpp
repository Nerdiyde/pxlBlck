#include "EventQueue.h"

EventQueueStruct::EventQueueStruct() {}

void EventQueueStruct::add(const String& event)
{
  _eventQueue.push_back(event);
}

void EventQueueStruct::addMove(String&& event)
{
  if (!event.length()) return;
  #ifdef USE_SECOND_HEAP
  HeapSelectIram ephemeral;
  if (!mmu_is_iram(&(event[0]))) {
    // Wrap in String constructor to make sure it is stored in the 2nd heap.
    _eventQueue.push_back(String(event));
    return;
  }
  #endif
  _eventQueue.emplace_back(std::move(event));
}

bool EventQueueStruct::getNext(String& event)
{
  if (_eventQueue.empty()) {
    return false;
  }
  event = _eventQueue.front();
  _eventQueue.pop_front();
  return true;
}

void EventQueueStruct::clear()
{
  _eventQueue.clear();
}

bool EventQueueStruct::isEmpty() const
{
  return _eventQueue.empty();
}