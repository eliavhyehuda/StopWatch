// Author: Michael Terry

#include "ComponentEvent.h"
#include "XWindow.h"

namespace cs349
{

  ComponentEvent::ComponentEvent(XWindow* window)
  {
    this->window = window;
  }

  ComponentEvent::~ComponentEvent()
  {
  }

  XWindow* ComponentEvent::GetWindow() const
  {
    return this->window;
  }

}
