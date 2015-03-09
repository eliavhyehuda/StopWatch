// Author: Michael Terry
#ifndef __COMPONENT__
#define __COMPONENT__

#include <vector>
#include <map>
#include <string>

using namespace std;

// Forward declarations
namespace cs349
{
  class XWindow;
}

// Project includes
#include "Graphics.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "PaintEvent.h"
#include "Point.h"
#include "Rectangle.h"

namespace cs349
{

  /**
   * Component is one of the biggest classes in the framework as it is
   * the "thing" that users interact with: It represents anything that
   * is visible, and anything that can respond to mouse and key
   * events. It is also the base class for the XWindow class, which
   * represents the actual window.
   * 
   * Components can contain other components (AddComponent). This
   * creates your interactor tree, a hierarchical ordering of
   * interactive components. The root of this interactor tree is an
   * XWindow object.
   * 
   * Components have a location and bounds. This location is in the
   * coordinate system of its parent Component. For example, if
   * component A is a child of the window, and has a location of (5,
   * 5), then that location is relative to the top-left corner of the
   * window's display area. If component A has a child, component B,
   * with a location of (10, 5), then its location in the window is
   * (15, 10), since (10, 5) is relative to the top-left corner of its
   * parent, component A, whose location is (5, 5).
   * 
   * While the location of the component is relative to its parent's
   * coordinate system (i.e., its location), any painting and
   * interaction in the component is assumed to be in the coordinate
   * system of the component itself. For example, say the user clicks
   * at point (17, 20) on the window. In our example above, that point
   * corresponds to point (12, 15) in component A's coordinate system.
   * Thus, when component A receives that mouse event, the coordinates
   * will be relative to the top-left corner of it, rather than in
   * window coordinates.
   * 
   * Because coordinates in events are assumed to be in the coordinate
   * system of the component itself, *you* are responsible for
   * properly translating coordinates to children coordinates if you
   * pass events on to them. For example, in our example above, (17,
   * 20) in the window coordinate system translates to (12, 15) in
   * component A's corrdinate system, and (2, 10) in component B's
   * coordinate system.
   * 
   * Components respond to events via the HandleKeyEvent and
   * HandleMouseEvent methods.
   *
   * When the component needs to be painted, the Paint method is
   * called with a Graphics object. All drawing operations occur
   * through the Graphics object. Typically, what you do is write a
   * single implementation of Paint in Component, setting up all
   * relevant variables in the Graphics context object (such as
   * foreground/background color), then call the internal (protected)
   * method, PaintComponent. Subclasses then override PaintComponent
   * to implement custom painting behavior, with the Graphics object
   * already assumed to be configured for drawing for that Component.
   *
   * Once the Component has painted itself, it should paint its
   * children. Prior to doing this, it needs to set up the Graphic
   * context's AffineTransform to shift the coordinate system to that
   * of each child. This allows each Component to assume that its
   * top-left corner is the origin, and paint accordingly.
   * 
   * Important: Components should only paint themselves (and their
   * children) when their visible attribute is set to true. If a
   * component is not visible, then its children should *not* be
   * painted, regardless of the state of the visible attribute for
   * them.
   * 
   * Whenever the component changes its visual presentation, it should
   * call one of the "Repaint" methods. The Repaint methods should add
   * a PaintEvent event to the Application event queue to be repainted
   * in the future.
   */
  class Component
  {	
  private:
    string        name;            ///< Name is used for interactive debugging, logging
    Rectangle     bounds;          /**< The bounds of the component, where the x,y
                                        position is relative to the parent coordinate system */
    Component*    parent;          ///< The parent Component. An XWindow's parent is NULL
    bool          visible;         ///< Whether the Component should itself and its children
    unsigned long backgroundColor; ///< The color to use for the background when painting
    unsigned long foregroundColor; ///< The color to use for the background when painting
    unsigned long debugDelayMS;    ///< Used to debug
                                   ///graphics/painting

    long          componentNum;    ///< Used for scripting to uniquely identify the Component

    static long componentCounter;  /**< Used for scripting. Automatically increments each
                                        time a new Component is created */
    static map<long, Component*> intComponentMap; ///< Used for scripting

  protected:
    vector<Component*> children;   ///< All the children of this Component

    /**
     * The method that does the actual painting of the component once
     * the Graphics context object has been set up by the Paint
     * method.
     *
     * The method is pure virtual because it should be overridden by
     * subclasses to do the actual painting.
     */
    virtual void PaintComponent(Graphics* g) = 0;
	
  public:
    /**
     * @param name An arbitrary name to assign this Component to help
     * with debugging and scripting. It is not used anywhere else.
     */
    Component(const string & name);

    /**
     * Free up any memory (such as child Components) held by this
     * Component.
     */
    virtual ~Component();

    /**
     * @return The name passed in when constructed. Used for scripting
     * and debugging
     */
    virtual string         GetName() const;

    /**
     * Adds a child Component to this Component and sets its parent to
     * this Component. Once added, this Component will take over
     * memory management of the pointer, deleting it should this
     * object be destroyed. However, if the child Component is
     * manually removed via RemoveComponent(), then it no longer is
     * responsible for managing the child's memory.
     * @param child The Component to add as a child.
     */
    virtual void           AddComponent(Component* child);

    /**
     * Removes the specified Component from the list of children and
     * sets its parent to NULL, if found it found. Does nothing if the
     * child is not in the list of child Components for this Component.
     * @param child The Component to remove as a child.
     */
    virtual void           RemoveComponent(Component* child);
    
    /**
     * @return The background color used when painting this Component.
     */
    virtual unsigned long  GetBackgroundColor() const;

    /**
     * @return The bounds of this Component: Its x,y position (in the
     *         coordinate system of the parent), and its width/height.
     */
    virtual Rectangle      GetBounds() const;

    /**
     * @return The foreground color used when painting this Component.
     */
    virtual unsigned long  GetForegroundColor() const;

    /**
     * @return A pointer to this Component's parent, NULL if it has no
     *         parent (which is the case if it is an XWindow).
     */
    virtual Component*     GetParent();

    /**
     * @return A pointer to the XWindow this Component is within, or
     *         NULL if it is not currently in a window.
     */
    virtual XWindow*       GetParentWindow();

    /**
     * Called when a KeyEvent has been generated.
     *
     * @return true if the event has been handled by this Component or one of
     * its children. 
     */
    virtual bool           HandleKeyEvent(const KeyEvent & e);

    /**
     * Called when a MouseEvent has been generated within the space of
     * this Component. *The coordinates are assumed to be in the
     * Component's coordinate system.*
     *
     * @return true if the event has been handled by this Component or one of
     * its children. 
     */
    virtual bool           HandleMouseEvent(const MouseEvent & e);

    /**
     * Tests whether a point, in the *parent Component's coordinate
     * system*, is within this Component's bounds. Note that this
     * requires converting the Point into the local coordinate system.
     * This method is used for determining where a MouseEvent should
     * be delivered.
     */
    virtual bool IsPointInComponent(const Point & p) const;
    
    /**
     * @return Whether this Component is visible or not (and hence
     * whether it and its children should be painted or not).
     */
    virtual bool IsVisible() const;

    /**
     * Called when the Component should paint itself. In this method,
     * this class should configure the Graphics context with all of
     * this Component's settings (such as foreground/background
     * color), then call the protected PaintComponent method.
     *
     * The Component should assume the Graphics object's
     * AffineTransform has already been transformed to produce a local
     * coordinate system (so the top-left corner of the Component can
     * be assumed to be the origin).
     */
    virtual void Paint(Graphics* g);
    
    /**
     * Forces a repaint of this object (its entire area).
     */
    virtual void Repaint();

    /**
     * Forces a repaint of only the specified area of this Component
     * (where the location is assumed to be in the local coordinate
     * system).
     */
    virtual void Repaint(const Rectangle & r);

    /**
     * Sets the background color of this Component. Triggers a call to Repaint().
     * @param c The color to use (should be either the black or white
     *          color obtained from the Application object.)
     */
    virtual void SetBackgroundColor(unsigned long c);

    /**
     * Sets the foreground color of this Component. Triggers a call to Repaint().
     * @param c The color to use (should be either the black or white
     *          color obtained from the Application object.)
     */
    virtual void SetForegroundColor(unsigned long c);

    /**
     * Sets the bounds of this Component, where location is relative
     * to the parent Component. Triggers a call to Repaint().
     * @param r The bounds, with location relative to the parent.
     */
    virtual void SetBounds(const Rectangle & r);

    /**
     * Sets the location of this Component, relative to the parent
     * Component. Triggers a call to Repaint().
     * @param p The new location, relative to the parent.
     */
    virtual void SetLocation(const Point & p);

    /**
     * Sets a debug delay for painting.
     */
    virtual void SetDebugDelay(unsigned long delayMS);

    /**
     * Sets the parent of this Component. Triggers a call to
     * Repaint().
     */
    virtual void SetParent(Component* parent);

    /**
     * Sets the size of this Component. Triggers a call to Repaint().
     */
    virtual void SetSize(int width, int height);

    /**
     * Sets whether this Component is visible (painted) or not.
     * Triggers a call to Repaint().
     */
    virtual void SetVisible(bool visible);

    /**
     * Returns the unique number of this Component. Used for scripting.
     * @return The unique number of this Component.
     */
    long GetComponentNum() const;

    /**
     * Gets the Component with the associated number, or NULL if no
     * such Component exists.
     * @return A pointer to the Component with the given component
     * number, or NULL if no such Component exists.
     */
    static Component* GetComponent(long componentNum);

    /**
     * Walks the interactor tree for this Component, returning all
     * descendants found via a depth-first walk of the interactor
     * tree.
     */
    vector<Component*> GetDepthFirstInteractorTree() const;

    /**
     * Walks the interactor tree for this Component, returning all
     * descendants found via a breadth-first walk of the interactor
     * tree.
     */
    vector<Component*> GetBreadthFirstInteractorTree() const;
  };

}
#endif /* __COMPONENT__ */
