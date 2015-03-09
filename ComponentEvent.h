// Author: Michael Terry

#ifndef __COMPONENTEVENT__
#define __COMPONENTEVENT__

#include "Event.h"

namespace cs349
{

  class XWindow;

  /**
   * The base class for any event involving a component on the window.
   * This class merely stores the XWindow object for subclasses.
   * 
   */
  class ComponentEvent : public Event
  {
  private:
    XWindow* window;
	
  public:
    ComponentEvent(XWindow* window);
    virtual ~ComponentEvent();
	
    virtual XWindow* GetWindow() const;
  };

}

#endif /* __COMPONENTEVENT__ */
