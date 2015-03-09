// Author: Michael Terry

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "KeyEvent.h"
#include "XWindow.h"
#include "Logging.h"

namespace cs349
{

  KeyEvent::KeyEvent(XWindow* window, KeyEvent::EventType type, XKeyEvent e) : ComponentEvent(window)
  {
    this->type = type;
    XLookupString(&e, &(this->key), 1, NULL, NULL);
  }

  KeyEvent::KeyEvent(XWindow* window, KeyEvent::EventType type, char key) : ComponentEvent(window)
  {
    this->type = type;
    this->key  = key;
  }

  KeyEvent::~KeyEvent()
  {
    ; // no-op
  }

  void KeyEvent::DispatchEvent()
  {
    this->GetWindow()->HandleKeyEvent(*this);
  }

  char KeyEvent::GetChar() const
  {
    return this->key;
  }

  KeyEvent::EventType KeyEvent::GetType() const
  {
    return this->type;
  }

  ostream & operator << (ostream &os, const KeyEvent & e) {
    os << "KeyEvent: ";
    
    switch (e.GetType()) {
    case KeyEvent::keyPress:
      os << "keyPress ";
      break;
    case KeyEvent::keyRelease:
      os << "keyRelease ";
      break;
    default:
      LOG_ERROR << "Unknown KeyEvent type: " << (int)e.GetType();
    }
    os << e.GetChar();
    return os;
  }

}
