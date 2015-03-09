// Author: Michael Terry

#include "Timer.h"
#include <algorithm>

#include <time.h>

namespace cs349
{

  Timer::Timer(EventQueue* eventQueue, unsigned long delayMS, bool repeat, TimerListener* l)
  {
    this->eventQueue = eventQueue;
    this->delayMS    = delayMS;
    this->repeat     = repeat;
    this->timerListeners.push_back(l);
    this->running    = false;
  }

  Timer::~Timer()
  {
    if (eventQueue) {
      eventQueue->RemoveTimer(this);
    }
  }

  void Timer::AddTimerListener(TimerListener* l)
  {
    this->timerListeners.push_back(l);
  }

  unsigned long Timer::GetCurrentTime()
  {
    struct timespec tp;
    clock_gettime(CLOCK_REALTIME, &tp);
    return tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
  }

  bool Timer::IsRunning() const
  {
    return this->running;
  }

  void Timer::RemoveTimerListener(TimerListener* l)
  {
    remove(this->timerListeners.begin(), this->timerListeners.end(), l);
  }

  void Timer::ServiceTimer()
  {
    if (!this->running) {
      return;
    }
    unsigned long elapsedTimeMS = GetCurrentTime() - this->lastTime;
    if (elapsedTimeMS > this->delayMS) {
      this->lastTime = GetCurrentTime();
      eventQueue->AddEventToQueue(new TimerEvent(this));
      if (!this->repeat) {
        this->Stop();
      }
    }
  }

  void Timer::Start()
  {
    if (!this->running) {
      eventQueue->AddTimer(this);
      this->lastTime = GetCurrentTime();
      this->running = true;
    }
  }

  void Timer::Stop()
  {
    if (this->running) {
      this->running = false;
      eventQueue->RemoveTimer(this);
    }
  }
}
