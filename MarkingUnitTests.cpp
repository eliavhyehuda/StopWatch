#include "EventQueue.h"
#include "gtest/gtest.h"
#include "Logging.h"
#include <unistd.h>

using namespace cs349;
using namespace std;

// We introduce a new namespace merely to avoid name collisions with
// the classes that are redefined here

namespace marking_namespace {

// This example/class partially taken from sample10_unittest.cc from gtest
class MemoryTrackingEvent : public Event {
public:
  static int num_allocated;
  static int num_dispatches;

  MemoryTrackingEvent() {
    LOG_INFO << "MemoryTrackingEvent allocated";
  }
  virtual ~MemoryTrackingEvent() {
    LOG_INFO << "MemoryTrackingEvent deallocated";
  }
  virtual void DispatchEvent() {
    LOG_INFO << "DispatchEvent called";
    num_dispatches++;
  }
  void* operator new(size_t allocation_size) {
    num_allocated++;
    return malloc(allocation_size);
  }
  void operator delete(void* block, size_t) {
    num_allocated--;
    free(block);
  }
  virtual string ToString() const {
    return "MemoryTrackingEvent";
  }
};

int MemoryTrackingEvent::num_allocated = 0;
int MemoryTrackingEvent::num_dispatches = 0;

TEST(A1MarkingEventQueueTests, EventQueuing) {
  // Test that events are queued properly
  EventQueue eventQueue;
  EXPECT_EQ(0, eventQueue.GetNumEventsInQueue());

  EXPECT_EQ(0, MemoryTrackingEvent::num_allocated);
  MemoryTrackingEvent* event = new MemoryTrackingEvent();
  EXPECT_EQ(1, MemoryTrackingEvent::num_allocated);
  EXPECT_EQ(0, MemoryTrackingEvent::num_dispatches);

  eventQueue.AddEventToQueue(event);
  EXPECT_EQ(1, eventQueue.GetNumEventsInQueue());

  eventQueue.ProcessNextEvent();
  EXPECT_EQ(0, eventQueue.GetNumEventsInQueue());
  EXPECT_EQ(0, MemoryTrackingEvent::num_allocated);
  EXPECT_EQ(1, MemoryTrackingEvent::num_dispatches);
}

TEST(A1MarkingEventQueueTests, EventQueueDestructorTest) {
  // Test that events are properly deleted on destruction
  EventQueue* eventQueue = new EventQueue();

  EXPECT_EQ(0, MemoryTrackingEvent::num_allocated);
  MemoryTrackingEvent* event = new MemoryTrackingEvent();
  EXPECT_EQ(1, MemoryTrackingEvent::num_allocated);

  eventQueue->AddEventToQueue(event);
  EXPECT_EQ(1, eventQueue->GetNumEventsInQueue());

  delete eventQueue;
  EXPECT_EQ(0, MemoryTrackingEvent::num_allocated);
}

class MemoryTrackingTimer : public Timer {
public:
  static int num_allocated;
  MemoryTrackingTimer(EventQueue* eventQueue, unsigned long delayMS, bool repeat, TimerListener* l) :
    Timer(eventQueue, delayMS, repeat, l)
  {
    LOG_INFO << "MemoryTrackingTimer allocated";
  }
  virtual ~MemoryTrackingTimer() {
    LOG_INFO << "MemoryTrackingTimer deallocated";
  }
  void* operator new(size_t allocation_size) {
    num_allocated++;
    return malloc(allocation_size);
  }
  void operator delete(void* block, size_t) {
    num_allocated--;
    free(block);
  }
};
int MemoryTrackingTimer::num_allocated = 0;

class TimerListenerCounter : public TimerListener {
public:
  int num_times_called;
  TimerListenerCounter() {
    num_times_called = 0;
  }
  virtual void HandleTimerEvent(const TimerEvent & e) {
    LOG_INFO << "HandleTimerEvent";
    num_times_called++;
  }
};

TEST(A1MarkingEventQueueTests, TimerTests) {
  // Test timers function as expected on event queue (e.g., that they
  // are serviced on a regular basis)
  const long SLEEP_TIME = 1000000/1000;
  EventQueue* eventQueue = new EventQueue();
  TimerListenerCounter repeatingListener;
  TimerListenerCounter oneShotListener;

  EXPECT_EQ(0, MemoryTrackingTimer::num_allocated);
  MemoryTrackingTimer* repeatingTimer = new MemoryTrackingTimer(eventQueue, 0, true, &repeatingListener);
  MemoryTrackingTimer* oneShotTimer   = new MemoryTrackingTimer(eventQueue, 0, false, &oneShotListener);
  EXPECT_EQ(2, MemoryTrackingTimer::num_allocated);

  EXPECT_EQ(0, eventQueue->GetNumTimers());
  EXPECT_EQ(0, repeatingListener.num_times_called);
  EXPECT_EQ(0, oneShotListener.num_times_called);
  EXPECT_FALSE(repeatingTimer->IsRunning());
  EXPECT_FALSE(oneShotTimer->IsRunning());

  EXPECT_FALSE(repeatingTimer->IsRunning());
  EXPECT_FALSE(oneShotTimer->IsRunning());

  repeatingTimer->Start();
  EXPECT_EQ(1, eventQueue->GetNumTimers());
  oneShotTimer->Start();
  EXPECT_EQ(2, eventQueue->GetNumTimers());

  EXPECT_TRUE(repeatingTimer->IsRunning());
  EXPECT_TRUE(oneShotTimer->IsRunning());

  EXPECT_EQ(0, repeatingListener.num_times_called);
  EXPECT_EQ(0, oneShotListener.num_times_called);

  usleep(SLEEP_TIME);

  EXPECT_EQ(0, eventQueue->GetNumEventsInQueue());
  eventQueue->ProcessNextEvent();
  EXPECT_EQ(1, eventQueue->GetNumEventsInQueue());

  EXPECT_EQ(1, repeatingListener.num_times_called);
  EXPECT_EQ(0, oneShotListener.num_times_called);
  EXPECT_EQ(1, eventQueue->GetNumTimers());
  EXPECT_EQ(2, MemoryTrackingTimer::num_allocated);

  EXPECT_TRUE(repeatingTimer->IsRunning());
  EXPECT_FALSE(oneShotTimer->IsRunning());

  eventQueue->ProcessNextEvent();
  EXPECT_EQ(1, oneShotListener.num_times_called);
  EXPECT_EQ(1, eventQueue->GetNumTimers());
  EXPECT_EQ(2, MemoryTrackingTimer::num_allocated);

  EXPECT_TRUE(repeatingTimer->IsRunning());
  EXPECT_FALSE(oneShotTimer->IsRunning());

  usleep(SLEEP_TIME);
  eventQueue->ProcessNextEvent();
  EXPECT_EQ(2, repeatingListener.num_times_called);
  EXPECT_EQ(1, oneShotListener.num_times_called);

  delete repeatingTimer;
  EXPECT_EQ(1, MemoryTrackingTimer::num_allocated);
  EXPECT_EQ(0, eventQueue->GetNumTimers());

  delete oneShotTimer;
  EXPECT_EQ(0, MemoryTrackingTimer::num_allocated);
  EXPECT_EQ(0, eventQueue->GetNumTimers());

  delete eventQueue;

}
}
