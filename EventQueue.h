// Author: Michael Terry

#ifndef __EVENT_QUEUE__
#define __EVENT_QUEUE__

#include <vector>
#include "Event.h"

namespace cs349
{
  class EventQueue;
}
#include "Timer.h"


using namespace std;

namespace cs349
{

  /**
   * In an interactive application, we need a standardized way to
   * respond to events, such as mouse movements and clicks, keyboard
   * presses/releases, timer events, etc. The EventQueue and Event
   * classes provide a standardized way to queue up and process events.
   *
   * EventQueue maintains a queue of all the events that have been
   * received by the application. This is a first-in-first-out (FIFO)
   * queue, so events are processed in the order they are received.
   *
   * When an event is taken off of the event queue, DispatchEvent() is
   * called, indicating the Event should be delivered and procesed.
   *
   * X events are added to the EventQueue object via the Application object.
   *
   * EventQueue also maintains a list of timers that it regularly
   * "services" so they can check whether they need to tell their
   * listeners that a certain amount of time has elapsed. This allows
   * the application to regularly update itself (e.g., for animations)
   * without using threads. (Single-threaded interactive applications
   * are significantly easier to code.)
   *
   * Important note: Normally, the rule of thumb with memory
   * management is that if you allocate something, you are responsible
   * for deallocating it. As an exception to this rule, when adding
   * events to EventQueue, EventQueue will take care of deleting the
   * event after it has been delivered/processed.
   */
  class EventQueue {
  private:
    vector<Event*>    eventQueue;
    vector<Timer*>    timers;

    friend class Timer;

    /**
     * Add a Timer for regular execution in the event loop.
     */
    void AddTimer(Timer* timer);

    /**
     * Remove timer from regular execution in event loop.
     */
    void RemoveTimer(Timer* timer);
  
  public:

    /**
     * When destructor is called, frees up any memory (Events) held in
     * the queue.
     */
    virtual ~EventQueue();

    /**
     * Adds an event to the queue. The EventQueue object will delete
     * the memory pointed to by the event after it has been
     * dispatched (or if the EventQueue object is destroyed).
     */
    void AddEventToQueue(Event* e);

    /**
     * Clears the event queue of all pending events. Its only real
     * purpose is to support unit testing.
     */
    void ClearEventQueue();

    /**
     * This method performs a single run of event processing. It
     * services the timers, then removes the next event from the
     * event queue, calls its DispatchEvent() method, and frees memory
     * for the event.
     */
    void ProcessNextEvent();

    /**
     * Returns the number of events currently queued up.
     *
     * *Important:* Even if there are no events in the EventQueue, you
     * should call ProcessNextEvent() regularly so that timers are
     * properly serviced. However, you *should not* repeatedly call
     * ProcessNextEvent() if there are no events present, without
     * sleeping a little bit. If you don't sleep for a bit, the CPU
     * will be throttled unnecessarily, the TAs will become slightly
     * irritated, and marks will be deducted.
     */
    int GetNumEventsInQueue() const;

    /**
     * Returns the number of timers being managed by EventQueue.
     */
    int GetNumTimers() const;
  };
}

#endif // __EVENT_QUEUE__
