// Author: Michael Terry

#ifndef __TIMEREVENT__
#define __TIMEREVENT__

#include <vector>

using namespace std;

namespace cs349
{
  class TimerEvent;
}

#include "Event.h"
#include "Timer.h"

namespace cs349
{

  /**
   * An event that is created whenever a Timer triggers.
   */
  class TimerEvent : public Event
  {
  private:
    Timer* timer;
    TimerEvent(Timer* timer);
	
    friend class Timer;
	
  public:
    virtual ~TimerEvent();
    virtual void DispatchEvent();

    /**
     * Return a pointer to the Timer that triggered.
     */
    Timer* GetTimer() const;
  };

}

#endif /* __TIMEREVENT__ */
