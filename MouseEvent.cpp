// Author: Michael Terry

#include "MouseEvent.h"
#include "XWindow.h"
#include "Logging.h"

namespace cs349
{

  MouseEvent::MouseEvent(XWindow* window, EventType type, const Point & where) : ComponentEvent(window)
  {
    this->type = type;
    this->where = where;
  }

  MouseEvent::~MouseEvent()
  {
    ; // no-op
  }

  void MouseEvent::DispatchEvent()
  {
    this->GetWindow()->HandleMouseEvent(*this);
  }

  MouseEvent::EventType MouseEvent::GetType() const
  {
    return this->type;
  }

  Point MouseEvent::GetWhere() const
  {
    return this->where;
  }

  ostream & operator << (ostream &os, const MouseEvent & e) {
    switch (e.GetType()) {
    case MouseEvent::mouseUp:
      os << "mouseUp ";
      break;
    case MouseEvent::mouseDown:
      os << "mouseDown ";
      break;
    case MouseEvent::mouseMove:
      os << "mouseMove ";
      break;
    case MouseEvent::mouseDrag:
      os << "mouseDrag ";
      break;
    default:
      LOG_ERROR << "Unknown mouse event type in operator <<: " << (int)e.GetType();
    }
    os << " at: " << e.GetWhere();
    return os;
  }
}


