// A1 Unit Tests

#include "gtest/gtest.h"
#include "StopWatch.h"
#include "XApplication.h"

using namespace cs349;

TEST(A1UnitTestDemos, Construction) {
  StopWatch* watch = new StopWatch(XApplication::GetInstance()->GetEventQueue(), Rectangle(0, 0, 200, 75));
  EXPECT_FALSE(watch == NULL);
  EXPECT_FALSE(watch->IsRunning());

  EventQueue* queue = XApplication::GetInstance()->GetEventQueue();
  EXPECT_FALSE(queue == NULL);
  queue->ClearEventQueue(); // Need to clear the event queue of any
                            // events our watch window has generated,
                            // or else subsequent processing of those
                            // events will fail miserably (i.e., core
                            // dump) if events from the event queue
                            // are processed after the watch pointer
                            // is deleted below.

  delete watch;
}

TEST(StartButtonTest, Interaction) {
  StopWatch* watch = new StopWatch(XApplication::GetInstance()->GetEventQueue(), Rectangle(0, 0, 700, 400));
  EXPECT_FALSE(watch == NULL);

  // Create a synthetic mouse event to test whether watch responds to it
  // or not. Note that this assumes that clicking in the location
  // specified amounts to pressing the start/stop button. Your actual
  // interaction will likely be different, making this test useless.
  // However, this should provide a template for how to do unit tests
  // for interaction.
  EXPECT_FALSE(watch->GetParentWindow() == NULL);
  MouseEvent* e = new MouseEvent(watch->GetParentWindow(), MouseEvent::mouseUp, Point(205, 325));

  EventQueue* queue = XApplication::GetInstance()->GetEventQueue();
  EXPECT_FALSE(queue == NULL);

  EXPECT_FALSE(watch->IsRunning());
  queue->AddEventToQueue(e);
  unsigned int max_num_tries_to_flush_queue = 10;
  while (max_num_tries_to_flush_queue-- > 0
         && queue->GetNumEventsInQueue() > 0
         && !watch->IsRunning())
    {
      queue->ProcessNextEvent();
    }
  EXPECT_TRUE(watch->IsRunning());

  queue->ClearEventQueue();
  delete watch;
  // We do not need to delete the mouse event that we created, because
  // it will be deleted automatically by the EventQueue.
}
