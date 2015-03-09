// Author: Michael Terry
#ifndef __GRAPHICS__
#define __GRAPHICS__

#include <X11/Xlib.h>
#include <vector>
#include <string>

using namespace std;

namespace cs349
{
  class Graphics; 
  class XWindow;
}

#include "AffineTransform.h"
#include "Point.h"
#include "Rectangle.h"

namespace cs349
{

  /**
   * This class encapsulates X lib's Graphics Context (GC) and provides the
   * means by which components draw themselves on windows.
   * 
   * You can add functionality to this class (for example, there are no
   * methods provided to draw arcs, ellipses, or circles).
   * 
   * You may want to add functionality to change fonts and determine
   * the bounds of a string when drawn to the screen
   */
  class Graphics
  {
  private:
    Display*        display;  ///< X window display associated with
                              ///  this Graphics Context 
    Window          window;   ///< The X window associated with this
                              ///  Graphics Context 
    GC              gc;       ///< X window Graphics Context
    Rectangle       clipRect; /**< *Important*: Always stored in
                                   *window* coordinates. This requires
                                   care in setting/getting */

    AffineTransform transform;       /**< The transform applied to
                                        drawing operations'
                                        coordinates before doing any
                                        actual drawing or painting. */
    unsigned long   backgroundColor; ///< Background color to use
    unsigned long   foregroundColor; ///< Foreground color to use
    int             drawingMode;     ///< Drawing mode

    unsigned long   debugDelayMS; ///< Delay to add to support debugging

    friend class XWindow;

    /**
     * Private constructor. Called by XWindow class when a new window
     * is created.
     */
    Graphics(Display* display, Window window, GC gc);

    /**
     * Sets the graphic context using this object's attribute values.
     */
    void         PrepareGraphicsContext();

    /**
     * Transforms the given points using this object's
     * AffineTransform, returning a set of XPoint objects.
     */
    XPoint*      GetTransformedXPoints(const vector<Point> & points);

  protected:
    virtual void DebugDelay(const string & opName) const;

  public:
    virtual ~Graphics();

    /**
     * Draws a line between the two points given, after transforming
     * them via this object's current AffineTransform. Draws using the
     * current foreground color.
     */
    void DrawLine(const Point & p1, const Point & p2);

    /**
     * Draws a series of line segments given by the vectors of Points,
     * after transforming them via this object's current
     * AffineTransform. Draws using the current foreground color.
     */
    void DrawLineSegments(const vector<vector<Point> > & lines);

    /**
     * Draws a single point after transforming it via this object's
     * current AffineTransform. Draws using the current foreground color.
     */
    void DrawPoint(const Point & p);

    /**
     * Draws a series of points as given by the vector of Points,
     * after transforming them via this object's current
     * AffineTransform. Draws using the current foreground color.
     */
    void DrawPoints(const vector<Point> & points);

    /**
     * Draws the polygon specified by the vector of Points, after
     * transforming them via this object's current AffineTransform.
     * Draws using the current foreground color.
     */
    void DrawPolygon(const vector<Point> & vertices);

    /**
     * Draws the rectangle specified after transforming its points via
     * this object's current AffineTransform. Draws using the current
     * foreground color.
     */
    void DrawRect(const Rectangle & r);

    /**
     * Draws the text given at the point specified, after transforming
     * the point via this object's current AffineTransform. Draws
     * using the current foreground color.
     */
    void DrawText(const Point & p, const string & text);

    void SettingFont ();

    void DrawArc(const Point & p, int width, int height, int angle1, int angle2);


    /**
     * Fills the polygon specified by the vector of Points, after
     * transforming them via this object's current AffineTransform.
     * Fills using the current foreground color.
     */
    void FillPolygon(const vector<Point> & vertices);

    /**
     * Fills the rectangle specified, after transforming its points
     * via this object's current AffineTransform. Fills using the
     * current foreground color.
     */
    void FillRect(const Rectangle & r);

    /**
     * @return X lib's graphics context object.
     */
    GC   GetGC();

    /**
     * The background color that will be used for drawing operations.
     */
    unsigned long GetBackgroundColor() const;

    /**
     * Returns the delay, in milliseconds, being used when rendering
     * graphics operations. Useful for debugging output.
     */
    virtual unsigned long GetDebugDelay() const;

    /**
     * Sets the delay, in milliseconds, to add between performing
     * graphics operations. *Should only be used for debugging
     * purposes.*
     *
     * @param delayMS The delay, in milliseconds
     */
    virtual void SetDebugDelay(unsigned long delayMS);

    /**
     * The foreground color that will be used for drawing operations.
     */
    unsigned long GetForegroundColor() const;

    /**
     * Sets the background color that will be used for drawing
     * operations. Get valid color values from the Application object.
     */
    void SetBackgroundColor(unsigned long color);

    /**
     * Sets the foreground color that will be used for drawing
     * operations. Get valid color values from the Application object.
     */
    void SetForegroundColor(unsigned long color);

 
    /**
     * Gets the drawing mode (e.g., copy, xor, etc.). See X
     * documentation for more info
     * (http://tronche.com/gui/x/xlib/GC/manipulating.html).
     */
    int    GetDrawingMode() const;

    /**
     * Sets the drawing mode (e.g., copy, xor, etc.). See X
     * documentation for more info
     * (http://tronche.com/gui/x/xlib/GC/manipulating.html).
     */
    void   SetDrawingMode(int drawingMode);
 
    /**
     * Gets the Graphics context's clip rect, with coordinates
     * relative to the coordinate system specified by the current
     * AffineTransform.
     */
    Rectangle GetClipRect();

    /**
     * Sets the Graphics context's clip rect, with coordinates
     * relative to the coordinate system specified by the current
     * AffineTransform.
     */
    void      SetClipRect(const Rectangle & r);


    /**
     * AffineTransforms are used to alter how points, lines,
     * rectangles, and polygons are drawn on the screen, by scaling,
     * rotating, and translating them.
     *
     * @return The current AffineTransform
     */ 
    AffineTransform GetTransform() const;

    /**
     * AffineTransforms are used to alter how points, lines,
     * rectangles, and polygons are drawn on the screen, by scaling,
     * rotating, and translating them.
     *
     * Use this to set the AffineTransform to use for any subsequent
     * drawing/painting operations.
     */ 
    void            SetTransform(const AffineTransform & t);
  };
}

#endif /* __GRAPHICS__ */
