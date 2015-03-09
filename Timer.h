// Author: Michael Terry

#ifndef __TIMER__
#define __TIMER__

#include <vector>
#include <iostream>

using namespace std;

namespace cs349
{
  class Timer;
}

#include "TimerListener.h"
#include "Event.h"
#include "EventQueue.h"

namespace cs349
{

  /**
   * A basic timer class that integrates with an EventQueue object. To
   * use, instantiate a Timer with the delay specified in
   * milliseconds, indicate whether the timer repeats, and specify the
   * object that will be called whenever the timer triggers.
   * 
   * You should not need to modify this class to make it work, though
   * you must ensure that the EventQueue object regularly calls the
   * "ServiceTimer" method in its ProcessNextEvent() method to ensure
   * the Timer can check whether enough time has elapsed to notify its
   * listeners.
   * 
   * You must explicitly call "Start" to start the timer.
   * 
   * Caveats: Precision of the timer will be limited to how often ServiceTimer()
   *          is called by EventQueue via ProcessNextEvent. The timer is not
   *          guaranteed to notify its listeners at the elapsed time
   *          specified.
   *
   * As with other objects, you are responsible for deallocating any memory for this Timer * when you are finished with it.
   */
  class Timer
  {
  private:
    EventQueue*            eventQueue; /**< The event queue that this
                                          is a part of */
    unsigned long          delayMS;    ///< The delay in milliseconds
    unsigned long          lastTime;
    bool                   repeat;     ///< Whether it's a repeating timer
    bool                   running;    ///< Whether it's currently running
    vector<TimerListener*> timerListeners; /**< TimerListeners that are notified
                                              when the timer triggers */
	
    /**
     * Called by the Application from its event loop to enable the
     * Timer to check whether it needs to create a TimerEvent
     */
    void ServiceTimer();
	
    static unsigned long GetCurrentTime();
	
    friend class EventQueue;
    friend class TimerEvent;

  public:
    /**
     * Construct a new Timer object.
     *
     * @param eventQueue The EventQueue this Timer will be associated
     * with
     * @param delayMS The delay, in milliseconds, before this Timer
     * triggers once started
     * @param repeat Whether the Timer should reset itself and start
     * over once the initial time has elapsed
     * @param l The TimerListener object that will be notified when
     * this Timer triggers
     */
    Timer(EventQueue* eventQueue, unsigned long delayMS, bool repeat, TimerListener* l);

    /**
     * On destruction, removes itself from the EventQueue's list of
     * timers
     */
    virtual ~Timer();

    /**
     * Add a TimerListener object to be notified when the Timer
     * triggers.
     */
    void AddTimerListener(TimerListener* l);

    /**
     * Remove a TimerListener object
     */
    void RemoveTimerListener(TimerListener* l);
	
    /**
     * Starts the Timer and adds it to the EventQueue's list of Timers.
     */
    void Start();

    /**
     * Stops the Timer and removes in from the EventQueue's list of
     * Timers.
     */
    void Stop();

    /**
     * Returns whether the Timer is currently running.
     */	
    bool IsRunning() const;
  };

}

#endif /* __TIMER__ */
