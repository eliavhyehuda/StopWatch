// Author: Michael Terry

#include "TimerEvent.h"
#include "TimerListener.h"

namespace cs349
{

  TimerEvent::TimerEvent(Timer* timer) : Event()
  {
    this->timer = timer;
  }

  TimerEvent::~TimerEvent()
  {
  }

  void TimerEvent::DispatchEvent()
  {
    for (vector<TimerListener*>::iterator iter = this->timer->timerListeners.begin(); iter != this->timer->timerListeners.end(); iter++) {
      TimerListener* l = (*iter);
      l->HandleTimerEvent(*this);
    }
  }

  Timer* TimerEvent::GetTimer() const
  {
    return this->timer;
  }
}
