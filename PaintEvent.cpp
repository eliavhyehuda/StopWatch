#include "PaintEvent.h"
#include "XWindow.h"
#include "Logging.h"

namespace cs349
{

  PaintEvent::PaintEvent(XWindow* window, const Rectangle & damagedArea) : ComponentEvent(window)
  {
    this->damagedArea = damagedArea;
  }

  PaintEvent::~PaintEvent()
  {
    ; // no-op
  }

  void PaintEvent::DispatchEvent()
  {
    LOG_TODO << "TODO CS349: Implement PaintEvent::DispatchEvent()";
// TODO CS349
    this->GetWindow()->HandlePaintEvent(*this);
  }

  Rectangle PaintEvent::GetDamagedArea() const
  {
    return this->damagedArea;
  }
}
