// Author: Michael Terry

#include "XWindow.h"

#include <algorithm>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include "XApplication.h"
#include "Logging.h"

namespace cs349
{

  map<long, XWindow*> XWindow::intWindowMap;
  map<Window, XWindow*> XWindow::windowMappings;
  
  XWindow* XWindow::GetXWindowForWindow(Window window)
  {
    return windowMappings[window];
  }

  XWindow::XWindow(const string & title, const Rectangle & bounds) : Component(title)
  {
    this->display = XApplication::GetInstance()->GetXDisplay();

    // Create the window
    this->window = XCreateSimpleWindow(	this->display,
                                        DefaultRootWindow(this->display),
                                        bounds.x,
                                        bounds.y,
                                        bounds.width,
                                        bounds.height,
                                        0, // Window border width. Doesn't include width by window manager
                                        XApplication::GetInstance()->GetWhiteColor(),
                                        XApplication::GetInstance()->GetBlackColor());

    // Add ourselves to the mappings
    windowMappings[this->window] = this;
    windowNum = this->GetComponentNum();
    intWindowMap[windowNum] = this;

    XTextProperty xtext;
    xtext.value = (unsigned char*)title.c_str();
    xtext.encoding = XA_STRING;
    xtext.format = 8; // bits per character
    xtext.nitems = strlen(title.c_str()); // string length
    XSetWMName(this->display, this->window, &xtext);
							
    // Select input events
    XSelectInput(	this->display,
                        this->window,
                        ButtonPressMask
                        | ButtonReleaseMask
                        | KeyPressMask
                        | KeyReleaseMask
                        | ExposureMask
                        | PointerMotionMask
                        | ButtonMotionMask
                        | StructureNotifyMask);
					
    GC gc = XCreateGC(	display,
                        window,
                        0,
                        0);
						
    this->graphics = new Graphics(this->display, this->window, gc);
	
    // Now set our bounds to that which we were set
    XWindowAttributes attr;
    XGetWindowAttributes(this->display, this->window, &attr);
    Component::SetBounds(Rectangle(attr.x, attr.y, attr.width, attr.height));
  }

  XWindow::~XWindow()
  {
    // Remove ourself from the mappings
    windowMappings.erase(windowMappings.find(this->window));
    intWindowMap.erase(intWindowMap.find(windowNum));
    XDestroyWindow(this->display, this->window);
  }

  Graphics* XWindow::GetGraphics()
  {
    return this->graphics;
  }

  XWindow* XWindow::GetParentWindow()
  {
    return this;
  }

  void XWindow::HandlePaintEvent(const PaintEvent & e)
  {
    // Set up Graphics
    this->graphics->SetTransform(AffineTransform::MakeIdentity());
    this->graphics->SetClipRect(e.GetDamagedArea());
    this->Paint(this->graphics); // Calls Component's implementation
  }

  void XWindow::PaintComponent(Graphics * g) {
    ; // no-op -- all painting handled by children
  }

  void XWindow::SetTitle(const char* title)
  {
    XSizeHints hints;
	
    // Set its basic properties
    XSetStandardProperties(	display,
                                this->window,
                                title,
                                title,
                                None, // Pixmap for icon
                                NULL,
                                0,
                                &hints);
  }

  // Overrides
  Rectangle XWindow::GetBounds() const {
    Window root;
    int x, y;
    unsigned int width, height, border_width, depth;
    XGetGeometry(display, window, &root, &x, &y, &width, &height, &border_width, &depth);
// TODO (Mike): This sometimes crashes nondeterministically, so
// commented out for now
    // Window parent, *children;
    // unsigned int dummy_w, dummy_h, n_children;
    // children = NULL;
    // if (XQueryTree(display, window, &root, &parent, &children, &n_children) == 0) {
    //   LOG_ERROR << "Error: XQueryTree failed";
    // } else {
    //   XGetGeometry(display, parent, &root, &x, &y, &dummy_w, &dummy_h, &border_width, &depth);
    // }
    // if (children) {
    //   XFree(children);
    // }
    return Rectangle(x, y, width, height);
  }

  void XWindow::Repaint(const Rectangle & r) {
    XApplication::GetInstance()->GetEventQueue()->AddEventToQueue(new PaintEvent(this, r));
  }
  void XWindow::SetBounds(const Rectangle & r)
  {
    int x = (int)r.x;
    int y = (int)r.y;
    unsigned int width = (unsigned int)max(1.0, r.width);
    unsigned int height = (unsigned int)max(1.0, r.height);
    XMoveResizeWindow(display, window, x, y, width, height);
    Component::SetBounds(r);
  }
  void XWindow::SetLocation(const Point & p)
  {
    this->SetBounds(Rectangle(p.x, p.y, this->GetBounds().width, this->GetBounds().height));
  }
  void XWindow::SetParent(Component* parent)
  {
    LOG_ERROR << "Error: Cannot set parent of an XWindow";
  }
  void XWindow::SetSize(int width, int height)
  {
    this->SetBounds(Rectangle(this->GetBounds().x, this->GetBounds().y, width, height));
  }
  void XWindow::SetVisible(bool visible)
  {
    Component::SetVisible(visible);
    if (visible) {
      XMapRaised(display, window);
    } else {
      XUnmapWindow(display, window);
    }
  }
  
  long XWindow::GetWindowNum() const {
    return this->windowNum;
  }

  vector<XWindow*> XWindow::GetWindowList() {
    vector<XWindow*> windowList;
    for (map<long, XWindow*>::const_iterator i=intWindowMap.begin(); i != intWindowMap.end(); i++) {
      windowList.push_back(i->second);
    }
    return windowList;
  }

  XWindow* XWindow::GetXWindow(long windowNum) {
    if (intWindowMap.find(windowNum) == intWindowMap.end()) {
      return NULL;
    }
    return intWindowMap[windowNum];
  }
}
