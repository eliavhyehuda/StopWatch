// Author: Michael Terry

#ifndef __MOUSEEVENT__
#define __MOUSEEVENT__

namespace cs349
{
  class MouseEvent;
}

#include "ComponentEvent.h"
#include "Point.h"
#include <iostream>

using namespace std;

namespace cs349
{

  /**
   * Encapsulates a mouse event.
   * 
   * Coordinates are assumed to be in the coordinate system of
   * the component to which the event is delivered. This requires
   * creating a new MouseEvent when passing it down the interactor
   * tree.
   * 
   */
  class MouseEvent : public ComponentEvent
  {
  public:
    enum EventType {
      mouseUp,
      mouseDown,
      mouseMove,
      mouseDrag
    };

  private:
    EventType type;
    Point     where;

  public:
    /**
     * Constructs a MouseEvent of the given type, at the given
     * location.
     *
     * @param window The XWindow corresponding to this MouseEvent
     * @param eventType The type of MouseEvent
     * @param where The location of the MouseEvent, in coordinates
     *              local to the Component where this event is delivered.
     */
    MouseEvent(XWindow* window, EventType eventType, const Point & where);
    virtual ~MouseEvent();

    /**
     * Dispatches this event to the XWindow specified when
     * constructed.
     */
    virtual void DispatchEvent();

    /**
     * @return The type of MouseEvent
     */
    EventType    GetType() const;

    /**
     * @return Location of this mouse event, in coordinates local to
     *         the Component to which this event is delivered.
     */
    Point        GetWhere() const;
  };

  ostream & operator << (ostream &os, const MouseEvent & e);
}

#endif /* __MOUSEEVENT__ */
