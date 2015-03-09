// Author: Michael Terry

#include "Graphics.h"
#include "XApplication.h"
#include "XWindow.h"
#include "Logging.h"

#include <unistd.h>
#include <string.h>

namespace cs349
{
  Graphics::Graphics(Display* display, Window window, GC gc)
  {
    this->debugDelayMS = 0;
    this->display      = display;
    this->window       = window;
    this->gc           = gc;
    this->drawingMode  = GXcopy;
	
    // Set clip to current size of window
    XWindowAttributes attr;
    XGetWindowAttributes(this->display, this->window, &attr);
    this->clipRect.x = 0;
    this->clipRect.y = 0;
    this->clipRect.width = attr.width;
    this->clipRect.height = attr.height;
  }

  Graphics::~Graphics()
  {
    ; // no-op
  }

  void Graphics::DebugDelay(const string & opName) const {
    if (debugDelayMS > 0) {
      Point origin = Point(0, 0);
      Point x_axis = Point(1, 0);
      Point y_axis = Point(0, 1);
      LOG_PAINT << "Graphics op: " << opName;
      LOG_PAINT << "(0,0) through current transform: " << (this->transform * origin);
      LOG_PAINT << "(1,0) through current transform: " << (this->transform * x_axis);
      LOG_PAINT << "(0,1) through current transform: " << (this->transform * y_axis);

      XFlush(XApplication::GetInstance()->GetXDisplay());
      usleep(debugDelayMS * 1000);
    }
  }

  unsigned long Graphics::GetDebugDelay() const {
    return debugDelayMS;
  }

  void Graphics::SetDebugDelay(unsigned long delayMS) {
    debugDelayMS = delayMS;
  }

  void Graphics::DrawLine(const Point & p1, const Point & p2)
  {
    this->PrepareGraphicsContext();
    Point t1 = this->transform * p1;
    Point t2 = this->transform * p2;
    XDrawLine(this->display, this->window, this->gc, (int)t1.x, (int)t1.y, (int)t2.x, (int)t2.y);
    DebugDelay(string("DrawLine"));
  }

  void Graphics::DrawLineSegments(const vector<vector<Point> > & lines)
  {
    DebugDelay(string("DrawLineSegments (via call to DrawPolygon)"));
    for (vector<vector<Point> >::const_iterator iter=lines.begin(); iter != lines.end(); iter++) {
      this->DrawPolygon(*iter);
    }
  }

  void Graphics::DrawPoint(const Point & p)
  {
    this->PrepareGraphicsContext();
    Point newPoint = this->transform * p;
    XDrawPoint(this->display, this->window, this->gc, (int)newPoint.x, (int)newPoint.y);
    DebugDelay(string("DrawPoint"));
  }

  void Graphics::DrawPoints(const vector<Point> & points)
  {
    this->PrepareGraphicsContext();
    XPoint* xPoints = this->GetTransformedXPoints(points);
    XDrawPoints(this->display, this->window, this->gc, xPoints, points.size(), CoordModeOrigin);
    delete[] xPoints;
    DebugDelay(string("DrawPoints"));
  }

  void Graphics::DrawPolygon(const vector<Point> & vertices)
  {
    this->PrepareGraphicsContext();
    XPoint* xPoints = this->GetTransformedXPoints(vertices);
    XDrawLines(this->display, this->window, this->gc, xPoints, vertices.size(), CoordModeOrigin);
    delete[] xPoints;
    DebugDelay(string("DrawPolygon"));
  }

  void Graphics::DrawRect(const Rectangle & r)
  {
    Point p1 = Point(r.x, r.y);
    Point p2 = Point(r.x+r.width, r.y);
    Point p3 = Point(r.x+r.width, r.y+r.height);
    Point p4 = Point(r.x, r.y+r.height);
    vector<Point> points;
    points.push_back(p1);
    points.push_back(p2);
    points.push_back(p3);
    points.push_back(p4);
    points.push_back(p1);
    DebugDelay("DrawRect (via call to DrawPolygon)");
    this->DrawPolygon(points);
  }

  void Graphics::DrawText(const Point & p, const string & text)
  {
    this->PrepareGraphicsContext();
    XTextItem xTextItem;
    xTextItem.chars  = const_cast<char*>(text.c_str());
    xTextItem.nchars = text.size();
    xTextItem.font   = None;
    xTextItem.delta  = 0;
    Point newPoint = this->transform * p;
    XDrawText(this->display, this->window, this->gc, (int)newPoint.x, (int)newPoint.y, &xTextItem, 1);
    DebugDelay(string("DrawText"));
  }

  void Graphics::SettingFont ()
  {
    this->PrepareGraphicsContext();
    char* font_name = "12x24romankana";
    Font font_info;
    font_info = XLoadFont(this->display, font_name);
    XSetFont(this->display, this->gc, font_info);
  }

  void Graphics::DrawArc(const Point & p, int w, int h, int angle1, int angle2)
  {
    this->PrepareGraphicsContext();
    Point t = this->transform * p;
    XDrawArc(this->display, this->window, this->gc, (int)t.x-(w/2), (int)t.y-(h/2), w, h, angle1, angle2);
  }


  void Graphics::FillPolygon(const vector<Point> & vertices)
  {
    this->PrepareGraphicsContext();
    XPoint* xPoints = this->GetTransformedXPoints(vertices);
    XFillPolygon(this->display, this->window, this->gc, xPoints, vertices.size(), Complex, CoordModeOrigin);
    delete[] xPoints;
    DebugDelay(string("FillPolygon"));
  }

  void Graphics::FillRect(const Rectangle & r)
  {
    Point p1 = Point(r.x, r.y);
    Point p2 = Point(r.x+r.width, r.y);
    Point p3 = Point(r.x+r.width, r.y+r.height);
    Point p4 = Point(r.x, r.y+r.height);
    vector<Point> points;
    points.push_back(p1);
    points.push_back(p2);
    points.push_back(p3);
    points.push_back(p4);
    points.push_back(p1);
    DebugDelay("FillRect (via call to FillPolygon)");
    this->FillPolygon(points);
  }

  unsigned long Graphics::GetBackgroundColor() const
  {
    return this->backgroundColor;
  }

  Rectangle Graphics::GetClipRect()
  {
    // Transform from window coordinates to current coordinates
    AffineTransform t = this->transform.GetInverse();
    Point p = t * this->clipRect.GetTopLeft();
    return Rectangle(p.x, p.y, this->clipRect.width, this->clipRect.height);
  }

  unsigned long Graphics::GetForegroundColor() const
  {
    return this->foregroundColor;
  }

  int Graphics::GetDrawingMode() const
  {
    return this->drawingMode;
  }

  GC Graphics::GetGC()
  {
    return this->gc;
  }

  AffineTransform Graphics::GetTransform() const
  {
    return this->transform;
  }

  XPoint* Graphics::GetTransformedXPoints(const vector<Point> & points)
  {
    XPoint* newPoints = new XPoint[points.size()];
    for (unsigned int i = 0; i < points.size(); i++) {
      Point p = this->transform * points[i];
      newPoints[i].x = (int)p.x;
      newPoints[i].y = (int)p.y;
    }
    return newPoints;
  }

  void Graphics::PrepareGraphicsContext()
  {
    XRectangle clip;
    XSetBackground(this->display, this->gc, this->backgroundColor);
    XSetForeground(this->display, this->gc, this->foregroundColor);
    XSetFunction(this->display, this->gc, this->drawingMode);
    clip.x = (int)this->clipRect.x;
    clip.y = (int)this->clipRect.y;
    clip.width = (int)this->clipRect.width;
    clip.height = (int)this->clipRect.height;
    XSetClipRectangles(this->display, this->gc, 0, 0, &clip, 1, Unsorted);
  }

  void Graphics::SetBackgroundColor(unsigned long color)
  {
    this->backgroundColor = color;
  }

  void Graphics::SetClipRect(const Rectangle & r)
  {
    // Transform into window coordinates
    Point p = this->transform * r.GetTopLeft();
    this->clipRect = Rectangle(p.x, p.y, r.width, r.height);
  }

  void Graphics::SetForegroundColor(unsigned long color)
  {
    this->foregroundColor = color;
  }

  void Graphics::SetDrawingMode(int drawingMode)
  {
    this->drawingMode = drawingMode;
  }

  void Graphics::SetTransform(const AffineTransform & t)
  {
    this->transform = t;
  }

}
