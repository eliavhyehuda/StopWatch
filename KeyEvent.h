// Author: Michael Terry

#ifndef __KEYEVENT__
#define __KEYEVENT__

#include <X11/Xlib.h>
#include <iostream>

using namespace std;

namespace cs349
{
  class KeyEvent;
}

#include "ComponentEvent.h"

namespace cs349
{

  /**
   * Encapsulates a key event.
   *
   * Note: You may need to add functionality that will allow you to
   *       get more information than the key's character. For example,
   *       the "Escape" key doesn't represent a character.
   */
  class KeyEvent : public ComponentEvent
  {
  public:
    enum EventType {
      keyPress,
      keyRelease
    };
  private:
    EventType type;
    char      key;
	
  public:
    /**
     * Creates a new KeyEvent from an XKeyEvent struct.
     */
    KeyEvent(XWindow* window, EventType type, XKeyEvent e);

    /**
     * Creates a new synthetic KeyEvent for the key represented by the
     * given character.
     */
    KeyEvent(XWindow* window, EventType type, char key);
    virtual ~KeyEvent();

    /**
     * Dispatches this event to the XWindow specified when
     * constructed.
     */
    virtual void DispatchEvent();

    /**
     * Returns whether it was a keyPress or keyRelease event.
     */
    EventType    GetType() const;

    /**
     * Returns the character representing the key pressed.
     */
    char         GetChar() const;
  };

  ostream & operator << (ostream &os, const KeyEvent & e);

}

#endif /* __KEYEVENT__*/
