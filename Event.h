// Author: Michael Terry
#ifndef __EVENT__
#define __EVENT__

namespace cs349
{

  /**
   * The Event class is the base class for any user interface event.
   * The only requirement for subclasses is to implement the
   * DispatchEvent() method.

   * The DispatchEvent() method is called by the EventQueue when the
   * Event is taken off of the queue. Because it is called without any
   * parameters, any Event object must contain all of the
   * data/information necessary to properly handle the event (such as
   * mouse or keyboard data, applicable window, etc.).
   */
  class Event {
  public:
    Event();
    virtual ~Event();

    /**
     * Called by EventQueue when the Event should be delivered and
     * acted upon.
     */
    virtual void DispatchEvent() = 0;
  };
}

#endif // __EVENT__
