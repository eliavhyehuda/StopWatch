// Author: Michael Terry

#ifndef __PAINTEVENT__
#define __PAINTEVENT__

namespace cs349
{
  class PaintEvent;
}

#include "ComponentEvent.h"
#include "Rectangle.h"

namespace cs349
{

  /**
   * Encapsulates a paint event. "Damaged area" refers to
   * what area of the window needs to be repainted.
   * 
   * Coordinates are assumed to be in the coordinate system of
   * the Component to which the event is delivered. Thus, they must be
   * updated when this event is passed down the interactor tree (where
   * they are updated by creating a new PaintEvent passing in the new
   * coordinates at construction time).
   * 
   * TODO CS349: You will need to implement this class's methods. Look
   * at MouseEvent and KeyEvent for examples on how to implement these
   * methods (they are rather straightforward).
   */
  class PaintEvent : public ComponentEvent
  {
  private:
    /**
     * The damaged region, in coordinates local to the Component to
     * which this event is delivered.
     */
    Rectangle damagedArea;
	
  public:
    /**
     * Creates a new PaintEvent object for the specified window,
     * corresponding to the area specified.
     *
     * @param window The XWindow to which this event corresponds
     * @param damagedArea The area that needs to be repainted, in
     * coordinates local to the Component to which this is delivered.
     */
    PaintEvent(XWindow* window, const Rectangle & damagedArea);
    virtual ~PaintEvent();

    /**
     * Dispatch this event to the XWindow specified when the object
     * was constructed.
     */
    virtual void DispatchEvent();
    
    /**
     * Returns the area that needs to be repainted, in coordinates
     * local to the Component to which this event is delivered.
     */
    Rectangle GetDamagedArea() const;
  };

}

#endif /* __PAINTEVENT__ */
