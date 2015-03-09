// Author: Michael Terry

#ifndef __XWINDOW__
#define __XWINDOW__

#include <X11/Xlib.h>
#include <map>

using namespace std;

// Forward declarations
namespace cs349
{
  class XWindow;
}

// Project includes
#include "Component.h"
#include "Graphics.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "PaintEvent.h"
#include "Rectangle.h"

void xwindow_load_first();

namespace cs349
{

  /**
   * The XWindow class encapsulates an X lib Window object, which
   * represents the actual window users see and interact with.
   * 
   * It is derived from Component, and thus shares much of its
   * functionality, though it must override some of the functionality
   * to make it appropriate to a Window (such as setting the location
   * and bounds).
   * 
   * This class also maintains a mapping of X lib window pointers to
   * our XWindow class pointers. Thus, when X lib creates a new event
   * and specifies a window, you can retrieve the XWindow object
   * corresponding to that window.
   * 
   * Each XWindow object maintains its own Graphics context, which it
   * passes on to its child components to enable drawing into the
   * window.
   * 
   * PaintEvents are passed to the XWindow. Paint events should be
   * analyzed and passed on to children to have them paint themselves.
   * However, only children intersecting the damaged area specified by
   * the PaintEvent should be drawn.
   */
  class XWindow : public Component
  {
  private:
    Display*  display;
    Window    window;
    Graphics* graphics;
    long      windowNum; ///< Used for scripting
 
    static map<long, XWindow*> intWindowMap; ///< Used for scripting interface
    static map<Window, XWindow*> windowMappings;
 
  public:
    /**
     * Creates a new window with the given title and bounds.
     */
    XWindow(const string & title, const Rectangle & bounds);
    virtual ~XWindow();
 
    /**
     * Returns a pointer to the XWindow object corresponding to the X
     * lib window pointer
     */
    static XWindow* GetXWindowForWindow(Window window);

    /**
     * Returns the graphics context associated with this window
     */
    virtual Graphics* GetGraphics();
 

    /**
     * Called when there are damaged areas that need to be repainted
     * in the window.
     */
    virtual void HandlePaintEvent(const PaintEvent & e);


    /**
     * Set title of the window
     */
    virtual void SetTitle(const char* title);

    // Overrides from Component
    virtual Rectangle GetBounds() const;
    virtual void PaintComponent(Graphics* g);
    virtual XWindow* GetParentWindow();
    virtual void Repaint(const Rectangle & r);
    virtual void SetBounds(const Rectangle & r);
    virtual void SetLocation(const Point & p);
    virtual void SetParent(Component* parent);
    virtual void SetSize(int width, int height);
    virtual void SetVisible(bool visible);

    /**
     * This mapping is used for scripting interfaces. It allows
     * scripts to refer to windows via integers, rather than pointers.
     */
    long GetWindowNum() const;

    /**
     * Gets a list of all XWindow objects that currently exist. Used
     * for scripting interface.
     */
    static vector<XWindow*> GetWindowList();

    /**
     * Retrieves a pointer to the XWindow corresponding to the given
     * windowNum, or NULL if no such window exists.
     */
    static XWindow* GetXWindow(long windowNum);
  };

}

#endif /* __XWINDOW__ */
