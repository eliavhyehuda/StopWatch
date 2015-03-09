#include "SchemeGUI.h"
#include "SchemeAffineTransform.h"
#include "SchemeUtils.h"
#include "XApplication.h"
#include "XWindow.h"
#include "SchemeComponent.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "PaintEvent.h"
#include "Logging.h"
#include <unistd.h>

#ifdef _A2
#include "TurtleGraphics.h"
#endif

namespace cs349 {

  vector<long> component_list_to_id_list(const vector<Component*> & components);
  vector<long> window_list_to_id_list(const vector<XWindow*> & windows);

  XWindow* get_xwindow(scheme* sc, pointer args, int pos);
  Component* get_component(scheme* sc, pointer args, int pos);
  SchemeComponent* get_scheme_component(scheme* sc, pointer args, int pos);

#ifdef _A2
  TurtleGraphics* get_turtle_graphics(scheme* sc, pointer args, int pos);
#endif

  pointer scheme_call_c_function(scheme* sc, pointer args);
  pointer marshall_logging_call(scheme* sc, pointer args, const string & fn_name);
  pointer marshall_xwindow_call(scheme* sc, pointer args, const string & fn_name);
  pointer marshall_graphics_call(scheme* sc, pointer args, const string & fn_name);
  pointer marshall_component_call(scheme* sc, pointer args, const string & fn_name);
  pointer marshall_sc_component_call(scheme* sc, pointer args, const string & fn_name);

#ifdef _A2
  pointer marshall_turtle_call(scheme* sc, pointer args, const string & fn_name);
#endif

  pointer scheme_xwindow_new(scheme* sc, pointer args);
  pointer scheme_get_xwindow_list(scheme* sc, pointer args);

  pointer scheme_graphics_draw_line(scheme* sc, pointer args, Graphics* g);
  pointer scheme_graphics_draw_line_segments(scheme* sc, pointer args, Graphics* g);
  pointer scheme_graphics_draw_point(scheme* sc, pointer args, Graphics* g);
  pointer scheme_graphics_draw_points(scheme* sc, pointer args, Graphics* g);
  pointer scheme_graphics_draw_polygon(scheme* sc, pointer args, Graphics* g);
  pointer scheme_graphics_draw_rect(scheme* sc, pointer args, Graphics* g);
  pointer scheme_graphics_draw_text(scheme* sc, pointer args, Graphics* g);
  pointer scheme_graphics_fill_polygon(scheme* sc, pointer args, Graphics* g);
  pointer scheme_graphics_fill_rect(scheme* sc, pointer args, Graphics* g);
  pointer scheme_graphics_set_background_color(scheme* sc, pointer args, Graphics* g);
  pointer scheme_graphics_set_foreground_color(scheme* sc, pointer args, Graphics* g);
  pointer scheme_graphics_set_clip_rect(scheme* sc, pointer args, Graphics* g);
  pointer scheme_graphics_set_transform(scheme* sc, pointer args, Graphics* g);


  pointer scheme_component_is_point_in_component(scheme* sc, pointer args, Component* component);
  pointer scheme_component_set_background_color(scheme* sc, pointer args, Component* component);
  pointer scheme_component_set_foreground_color(scheme* sc, pointer args, Component* component);
  pointer scheme_component_set_bounds(scheme* sc, pointer args, Component* component);
  pointer scheme_component_set_location(scheme* sc, pointer args, Component* component);
  pointer scheme_component_set_size(scheme* sc, pointer args, Component* component);
  pointer scheme_component_set_visible(scheme* sc, pointer args, Component* component);
  pointer scheme_component_add_component(scheme* sc, pointer args, Component* component);
  pointer scheme_component_remove_component(scheme* sc, pointer args, Component* component);


  pointer scheme_sc_component_is_scheme_component(scheme* sc, pointer args);
  pointer scheme_sc_component_new(scheme* sc, pointer args);
  pointer scheme_sc_component_add_instruction(scheme* sc, pointer args, SchemeComponent * component);
  pointer scheme_sc_component_get_instructions(scheme* sc, pointer args, SchemeComponent * component);
  pointer scheme_sc_component_set_instructions(scheme* sc, pointer args, SchemeComponent * component);


#ifdef _A2
  pointer scheme_is_turtle_graphics(scheme* sc, pointer args);
  pointer scheme_turtle_component_new(scheme* sc, pointer args);
  pointer scheme_turtle_rotate(scheme* sc, pointer args, TurtleGraphics* turtle);
  pointer scheme_turtle_forward(scheme* sc, pointer args, TurtleGraphics* turtle);
  pointer scheme_turtle_draw_text(scheme* sc, pointer args, TurtleGraphics* turtle);
  pointer scheme_turtle_draw_point(scheme* sc, pointer args, TurtleGraphics* turtle);
  pointer scheme_turtle_get_affine_transform(scheme* sc, pointer args, TurtleGraphics* turtle);
  pointer scheme_turtle_get_instructions(scheme* sc, pointer args, TurtleGraphics* turtle);
  pointer scheme_turtle_get_orientation(scheme* sc, pointer args, TurtleGraphics* turtle);
  pointer scheme_turtle_get_position(scheme* sc, pointer args, TurtleGraphics* turtle);
  pointer scheme_turtle_draw_step(scheme* sc, pointer args, TurtleGraphics* turtle);
#endif

  pointer scheme_simulate_key_event(scheme* sc, pointer args);
  pointer scheme_simulate_mouse_event(scheme* sc, pointer args);
  pointer scheme_simulate_paint_event(scheme* sc, pointer args);


  pointer scheme_sleep(scheme* sc, pointer args);
  pointer scheme_usleep(scheme* sc, pointer args);

}

using namespace cs349;

int scheme_install_gui_procedures(scheme* sc) {
  scheme_define(sc, sc->global_env, mk_symbol(sc, "_scheme-call-c-function"),
                mk_foreign_func(sc, scheme_call_c_function));

  scheme_define(sc, sc->global_env, mk_symbol(sc, "get-xwindow-list"),
                mk_foreign_func(sc, scheme_get_xwindow_list));

  scheme_define(sc, sc->global_env, mk_symbol(sc, "simulate-key-event"),
                mk_foreign_func(sc, scheme_simulate_key_event));
  scheme_define(sc, sc->global_env, mk_symbol(sc, "simulate-mouse-event"),
                mk_foreign_func(sc, scheme_simulate_mouse_event));
  scheme_define(sc, sc->global_env, mk_symbol(sc, "simulate-paint-event"),
                mk_foreign_func(sc, scheme_simulate_paint_event));

  scheme_define(sc, sc->global_env, mk_symbol(sc, "sleep"),
                mk_foreign_func(sc, scheme_sleep));
  scheme_define(sc, sc->global_env, mk_symbol(sc, "usleep"),
                mk_foreign_func(sc, scheme_usleep));
  return 0;
}

namespace cs349 {
  vector<long> component_list_to_id_list(const vector<Component*> & components) {
    vector<long> id_list;
    for (vector<Component*>::const_iterator i=components.begin(); i != components.end(); i++) {
      id_list.push_back((*i)->GetComponentNum());
    }
    return id_list;
  }

  vector<long> window_list_to_id_list(const vector<XWindow*> & windows) {
    vector<long> id_list;
    for (vector<XWindow*>::const_iterator i=windows.begin(); i != windows.end(); i++) {
      id_list.push_back((*i)->GetWindowNum());
    }
    return id_list;
  }

  XWindow* get_xwindow(scheme* sc, pointer args, int pos) {
    long windowID;
    if (!scheme_extract_long(sc, args, pos, &windowID)) {
      LOG_ERROR << "Error: Need valid window ID for function";
      return NULL;
    }
    XWindow* window = XWindow::GetXWindow(windowID);
    if (window == NULL) {
      LOG_ERROR << "Error: Invalid window ID: " << windowID;
    }
    return window;
  }

  Component* get_component(scheme* sc, pointer args, int pos) {
    long componentID;
    if (!scheme_extract_long(sc, args, pos, &componentID)) {
      LOG_ERROR << "Error: Need valid component ID for function";
      return NULL;
    }
    Component* component = Component::GetComponent(componentID);
    if (component == NULL) {
      LOG_ERROR << "Error: Invalid component ID: " << componentID;
    }
    return component;
  }

  SchemeComponent* get_scheme_component(scheme* sc, pointer args, int pos) {
    Component* c = get_component(sc, args, pos);
    if (c == NULL) {
      return NULL;
    }
    SchemeComponent* scheme_component = NULL;
    scheme_component = dynamic_cast<SchemeComponent*>(c);
    if (scheme_component == NULL) {
      LOG_ERROR << "Error: Component is not a SchemeComponent";
    }
    return scheme_component;
  }

#ifdef _A2
  TurtleGraphics* get_turtle_graphics(scheme* sc, pointer args, int pos) {
    Component* c = get_component(sc, args, pos);
    if (c == NULL) {
      return NULL;
    }
    TurtleGraphics* turtle = NULL;
    turtle = dynamic_cast<TurtleGraphics*>(c);
    if (turtle == NULL) {
      LOG_ERROR << "Error: Component is not a TurtleGraphics";
    }
    return turtle;
  }
#endif

  pointer scheme_get_xwindow_list(scheme* sc, pointer args) {
    vector<long> windowList = window_list_to_id_list(XWindow::GetWindowList());
    return scheme_long_vector_to_list(sc, windowList);
  }

  pointer scheme_call_c_function(scheme* sc, pointer args) {
    string fn_name;
    if (!scheme_extract_string(sc, args, 0, &fn_name)) {
      return sc->F;
    }

    // Cue past the function name argument
    args = pair_cdr(args);

    /*
     * Functions that do not take any arguments
     */
    if (fn_name == "graphics-white-color") {
      return mk_integer(sc, XApplication::GetInstance()->GetWhiteColor());
    } else if (fn_name == "graphics-black-color") {
      return mk_integer(sc, XApplication::GetInstance()->GetBlackColor());
    } else if (fn_name == "graphics-flush") {
      XFlush(XApplication::GetInstance()->GetXDisplay());
      return sc->T;
    }

    /*
     * Functions for specific classes
     */
    if (fn_name.find("xwindow-") == 0) {
      return marshall_xwindow_call(sc, args, fn_name);
    } else if (fn_name.find("logging-") == 0) {
      return marshall_logging_call(sc, args, fn_name);
    } else if (fn_name.find("graphics-") == 0) {
      return marshall_graphics_call(sc, args, fn_name);
    } else if (fn_name.find("component-") == 0) {
      return marshall_component_call(sc, args, fn_name);
    } else if (fn_name.find("scheme-component-") == 0
               || fn_name.find("is-scheme-component") == 0
               || fn_name.find("_scheme-component-") == 0)
      {
        return marshall_sc_component_call(sc, args, fn_name);
      }
    else if (fn_name.find("turtle-") == 0
             || fn_name.find("is-turtle") == 0
             || fn_name.find("_turtle-") == 0)
      {
#ifdef _A2
        return marshall_turtle_call(sc, args, fn_name);
#endif
      }
    LOG_ERROR << "Error: Unknown function being called from Scheme to C: " << fn_name;
    return sc->F;
  }

  pointer marshall_logging_call(scheme* sc, pointer args, const string & fn_name) {
    if (fn_name == "logging-set-level") {
      long level;
      if (scheme_extract_long(sc, args, 0, &level)) {
        FLAGS_v = level;
      } else {
        LOG_ERROR << "Error: Need long value to set logging level";
        return sc->F;
      }
    }
    return sc->T;
  }
  pointer marshall_xwindow_call(scheme* sc, pointer args, const string & fn_name) {
    if (fn_name == "xwindow-new") {
      return scheme_xwindow_new(sc, args);
    }

    XWindow* window = get_xwindow(sc, args, 0);
    if (window == NULL) {
      LOG_ERROR << "Error: Need window ID for function " << fn_name;
      return sc->NIL;
    }
    if (fn_name == "xwindow-get-depth-first-interactor-tree") {
      return scheme_long_vector_to_list(sc, component_list_to_id_list(window->GetDepthFirstInteractorTree()));
    }
    else if (fn_name == "xwindow-get-breadth-first-interactor-tree") {
      return scheme_long_vector_to_list(sc, component_list_to_id_list(window->GetBreadthFirstInteractorTree()));
    } else if (fn_name == "xwindow-delete") {
      delete window;
      return sc->T;
    }

    LOG_ERROR << "Unknown function: " << fn_name;
    return sc->F;
  }

  pointer scheme_xwindow_new(scheme* sc, pointer args) {
    string title;
    Rectangle bounds;
    if (scheme_extract_string(sc, args, 0, &title)
        && (scheme_extract_rect(sc, args, 1, &bounds)))
      {
        XWindow *win = new XWindow(title, bounds);
        return mk_integer(sc, win->GetWindowNum());
      }
    return mk_integer(sc, -1);
  }

  pointer marshall_graphics_call(scheme* sc, pointer args, const string & fn_name) {
    XWindow* window = get_xwindow(sc, args, 0);
    if (window == NULL) {
      LOG_ERROR << "Error: Need window ID for function " << fn_name;
      return sc->F;
    }
    Graphics* g = window->GetGraphics();
    if (fn_name == "graphics-draw-line") {
      return scheme_graphics_draw_line(sc, args, g);
    } else if (fn_name == "graphics-draw-line-segments") {
      return scheme_graphics_draw_line_segments(sc, args, g);
    } else if (fn_name == "graphics-draw-point") {
      return scheme_graphics_draw_point(sc, args, g);
    } else if (fn_name == "graphics-draw-points") {
      return scheme_graphics_draw_points(sc, args, g);
    } else if (fn_name == "graphics-draw-polygon") {
      return scheme_graphics_draw_polygon(sc, args, g);
    } else if (fn_name == "graphics-draw-rect") {
      return scheme_graphics_draw_rect(sc, args, g);
    } else if (fn_name == "graphics-draw-text") {
      return scheme_graphics_draw_text(sc, args, g);
    } else if (fn_name == "graphics-fill-polygon") {
      return scheme_graphics_fill_polygon(sc, args, g);
    } else if (fn_name == "graphics-fill-rect") {
      return scheme_graphics_fill_rect(sc, args, g);
    } else if (fn_name == "graphics-set-background-color") {
      return scheme_graphics_set_background_color(sc, args, g);
    } else if (fn_name == "graphics-set-foreground-color") {
      return scheme_graphics_set_foreground_color(sc, args, g);
    } else if (fn_name == "graphics-get-background-color") {
      return mk_integer(sc, window->GetGraphics()->GetBackgroundColor());
    } else if (fn_name == "graphics-get-foreground-color") {
      return mk_integer(sc, window->GetGraphics()->GetForegroundColor());
    } else if (fn_name == "graphics-get-clip-rect") {
      Rectangle clipRect = window->GetGraphics()->GetClipRect();
      return scheme_rectangle_to_list(sc, clipRect);
    } else if (fn_name == "graphics-set-clip-rect") {
      return scheme_graphics_set_clip_rect(sc, args, g);
    } else if (fn_name == "graphics-get-transform") {
      AffineTransform t = window->GetGraphics()->GetTransform();
      return scheme_matrix_to_lists(sc, t.GetMatrix());
    } else if (fn_name == "graphics-set-transform") {
      return scheme_graphics_set_transform(sc, args, g);
    } else if (fn_name == "graphics-get-debug-delay") {
      return mk_integer(sc, window->GetGraphics()->GetDebugDelay());
    } else if (fn_name == "graphics-set-debug-delay") {
      long debugDelay;
      if (scheme_extract_long(sc, args, 1, &debugDelay)) {
        window->GetGraphics()->SetDebugDelay(debugDelay);
        return sc->T;
      }
      return sc->F;
    }

    LOG_ERROR << "Unknown function: " << fn_name;
    return sc->F;
  }


  pointer scheme_graphics_draw_line(scheme* sc, pointer args, Graphics *g) {
    Point start, end;
    if (scheme_extract_point(sc, args, 1, &start)
        && scheme_extract_point(sc, args, 2, &end))
      {
        g->DrawLine(start, end);
        return sc->T;
      }
    return sc->F;
  }

  pointer scheme_graphics_draw_line_segments(scheme* sc, pointer args, Graphics* g) {
    if (args == sc->NIL || !is_pair(args)) {
      LOG_ERROR << "Error: Need list of point lists for graphics-draw-line-segments";
      return sc->F;
    }

    vector<vector<Point> > lines;
    if (scheme_get_list_length(sc, args) < 2) {
      LOG_ERROR << "Error: Need list of point lists for graphics-draw-line-segments";
      return sc->F;
    }
    pointer cur_pointer = pair_car(pair_cdr(args));
    while (1) {
      if (cur_pointer == sc->NIL) {
        break;
      }
      if (!is_pair(cur_pointer)) {
        LOG_ERROR << "Error: Unexpected type in graphics-draw-line-segments";
        break;
      }
      vector<Point> points;
      if (!scheme_extract_point_list(sc, cur_pointer, 0, &points)) {
        LOG_ERROR << "Error: Need point lists for graphics-draw-line-segments";
        return sc->F;
      }
      lines.push_back(points);
      cur_pointer = pair_cdr(cur_pointer);
    }
    g->DrawLineSegments(lines);
    return sc->T;
  }

  pointer scheme_graphics_draw_point(scheme* sc, pointer args, Graphics* g) {
    Point point;
    if (scheme_extract_point(sc, args, 1, &point)) {
      g->DrawPoint(point);
      return sc->T;
    }
    return sc->F;
  }

  pointer scheme_graphics_draw_points(scheme* sc, pointer args, Graphics* g) {
    vector<Point> points;
    if (scheme_extract_point_list(sc, args, 1, &points)) {
      g->DrawPoints(points);
      return sc->T;
    }
    return sc->F;
  }

  pointer scheme_graphics_draw_polygon(scheme* sc, pointer args, Graphics* g) {
    vector<Point> points;
    if (scheme_extract_point_list(sc, args, 1, &points)) {
      g->DrawPolygon(points);
      return sc->T;
    }
    return sc->F;
  }

  pointer scheme_graphics_draw_rect(scheme* sc, pointer args, Graphics* g) {
    Rectangle rect;
    if (scheme_extract_rect(sc, args, 1, &rect)) {
      g->DrawRect(rect);
      return sc->T;
    }
    return sc->F;
  }

  pointer scheme_graphics_draw_text(scheme* sc, pointer args, Graphics* g) {
    Point point;
    string text;
    if (scheme_extract_point(sc, args, 1, &point)
        && scheme_extract_string(sc, args, 2, &text))
      {
        g->DrawText(point, text.c_str());
        return sc->T;
      }
    return sc->F;
  }

  pointer scheme_graphics_fill_polygon(scheme* sc, pointer args, Graphics* g) {
    vector<Point> points;
    if (scheme_extract_point_list(sc, args, 1, &points)) {
      g->FillPolygon(points);
      return sc->T;
    }
    return sc->F;
  }

  pointer scheme_graphics_fill_rect(scheme* sc, pointer args, Graphics* g) {
    Rectangle rect;
    if (scheme_extract_rect(sc, args, 1, &rect)) {
      g->FillRect(rect);
      return sc->T;
    }
    return sc->F;
  }

  pointer scheme_graphics_set_background_color(scheme* sc, pointer args, Graphics* g) {
    long color;
    if (scheme_extract_long(sc, args, 1, &color)) {
      g->SetBackgroundColor((unsigned long)color);
      return sc->T;
    }
    return sc->F;
  }

  pointer scheme_graphics_set_foreground_color(scheme* sc, pointer args, Graphics* g) {
    long color;
    if (scheme_extract_long(sc, args, 1, &color)) {
      g->SetForegroundColor((unsigned long)color);
      return sc->T;
    }
    return sc->F;
  }

  pointer scheme_graphics_set_clip_rect(scheme* sc, pointer args, Graphics* g) {
    Rectangle rect;
    if (scheme_extract_rect(sc, args, 1, &rect)) {
      g->SetClipRect(rect);
      return sc->T;
    }
    return sc->F;
  }

  pointer scheme_graphics_set_transform(scheme* sc, pointer args, Graphics* g) {
    SquareMatrix m;
    if (scheme_extract_matrix(sc, args, 1, &m)) {
      AffineTransform t(m);
      g->SetTransform(t);
      return sc->T;
    }
    return sc->F;
  }

  pointer marshall_component_call(scheme* sc, pointer args, const string & fn_name) {
    Component* component = get_component(sc, args, 0);
    if (component == NULL) {
      LOG_ERROR << "Error: Need component ID for " << fn_name;
      return sc->F;
    }

    if (fn_name == "component-get-name") {
      return mk_string(sc, component->GetName().c_str());
    } else if (fn_name == "component-get-background-color") {
      return mk_integer(sc, component->GetBackgroundColor());
    } else if (fn_name == "component-get-bounds") {
      return scheme_rectangle_to_list(sc, component->GetBounds());
    } else if (fn_name == "component-get-foreground-color") {
      return mk_integer(sc, component->GetForegroundColor());
    } else if (fn_name == "component-get-parent") {
      Component* parent = component->GetParent();
      if (parent == NULL) return mk_integer(sc, 0);
      return mk_integer(sc, parent->GetComponentNum());
    } else if (fn_name == "component-get-parent-window") {
      XWindow* window = component->GetParentWindow();
      if (window == NULL) return mk_integer(sc, 0);
      return mk_integer(sc, window->GetWindowNum());
    } else if (fn_name == "component-is-point-in-component?") {
      return scheme_component_is_point_in_component(sc, args, component);
    } else if (fn_name == "component-is-visible?") {
      return (component->IsVisible()) ? sc->T : sc->F;
    } else if (fn_name == "component-repaint") {
      component->Repaint();
      return sc->T;
    } else if (fn_name == "component-set-background-color") {
      return scheme_component_set_background_color(sc, args, component);
    } else if (fn_name == "component-set-foreground-color") {
      return scheme_component_set_foreground_color(sc, args, component);
    } else if (fn_name == "component-set-bounds") {
      return scheme_component_set_bounds(sc, args, component);
    } else if (fn_name == "component-set-location") {
      return scheme_component_set_location(sc, args, component);
    } else if (fn_name == "component-set-size") {
      return scheme_component_set_size(sc, args, component);
    } else if (fn_name == "component-set-visible") {
      return scheme_component_set_visible(sc, args, component);
    } else if (fn_name == "component-add-component") {
      return scheme_component_add_component(sc, args, component);
    } else if (fn_name == "component-remove-component") {
      return scheme_component_remove_component(sc, args, component);
    } else if (fn_name == "component-set-debug-delay") {
      long debugDelay;
      if (scheme_extract_long(sc, args, 1, &debugDelay)) {
        component->SetDebugDelay(debugDelay);
        return sc->T;
      }
      return sc->F;
    }

    LOG_ERROR << "Error: Unknown function: " << fn_name;
    return sc->F;
  }

  pointer scheme_component_is_point_in_component(scheme* sc, pointer args, Component * component) {
    Point point;
    if (scheme_extract_point(sc, args, 1, &point)) {
      return (component->IsPointInComponent(point)) ? sc->T : sc->F;
    }
    return sc->NIL;
  }

  pointer scheme_component_set_background_color(scheme* sc, pointer args, Component* component) {
    long color;
    if (scheme_extract_long(sc, args, 1, &color)) {
      component->SetBackgroundColor((unsigned long)color);
      return sc->T;
    }
    return sc->F;
  }

  pointer scheme_component_set_foreground_color(scheme* sc, pointer args, Component* component) {
    long color;
    if (scheme_extract_long(sc, args, 1, &color)) {
      component->SetForegroundColor((unsigned long)color);
      return sc->T;
    }
    return sc->F;
  }

  pointer scheme_component_set_bounds(scheme* sc, pointer args, Component * component) {
    Rectangle rect;
    if (scheme_extract_rect(sc, args, 1, &rect)) {
      component->SetBounds(rect);
      return sc->T;
    }
    return sc->F;
  }

  pointer scheme_component_set_location(scheme* sc, pointer args, Component* component) {
    Point point;
    if (scheme_extract_point(sc, args, 1, &point)) {
      component->SetLocation(point);
      return sc->T;
    }
    return sc->F;
  }

  pointer scheme_component_set_size(scheme* sc, pointer args, Component* component) {
    long width, height;
    if (scheme_extract_long(sc, args, 1, &width)
        && scheme_extract_long(sc, args, 2, &height))
      {
        component->SetSize(width, height);
        return sc->T;
      }
    return sc->F;
  }

  pointer scheme_component_set_visible(scheme* sc, pointer args, Component* component) {
    bool val;
    if (scheme_extract_bool(sc, args, 1, &val)) {
      component->SetVisible(val);
      return sc->T;
    }
    return sc->F;
  }

  pointer scheme_component_add_component(scheme* sc, pointer args, Component* component) {
    Component* component_to_add = get_component(sc, args, 1);
    if (component_to_add == NULL) {
      LOG_ERROR << "Error: Need component ID for component-add-component";
      return sc->F;
    }
    component->AddComponent(component_to_add);
    return sc->T;
  }

  pointer scheme_component_remove_component(scheme* sc, pointer args, Component* component) {
    Component* component_to_remove = get_component(sc, args, 1);
    if (component_to_remove == NULL) {
      LOG_ERROR << "Error: Need component ID for component-remove-component";
      return sc->F;
    }
    component->RemoveComponent(component_to_remove);
    return sc->T;
  }

  pointer marshall_sc_component_call(scheme* sc, pointer args, const string & fn_name) {
    if (fn_name == "scheme-component-new") {
      return scheme_sc_component_new(sc, args);
    } else if (fn_name == "is-scheme-component?") {
      return scheme_sc_component_is_scheme_component(sc, args);
    }

    SchemeComponent* component = get_scheme_component(sc, args, 0);
    if (component == NULL) {
      LOG_ERROR << "Error: Need component ID for " << fn_name;
      return sc->F;
    }

    if (fn_name == "_scheme-component-add-instruction") {
      return scheme_sc_component_add_instruction(sc, args, component);
    } else if (fn_name == "scheme-component-clear-instructions") {
      component->ClearInstructions();
      return sc->T;
    } else if (fn_name == "_scheme-component-get-instructions") {
      return scheme_string_vector_to_list(sc, component->GetInstructions());
    } else if (fn_name == "_scheme-component-set-instructions") {
      return scheme_sc_component_set_instructions(sc, args, component);
    }

    LOG_ERROR << "Unknown function: " << fn_name;
    return sc->F;
  }

  pointer scheme_sc_component_new(scheme* sc, pointer args) {
    string component_name;
    if (scheme_extract_string(sc, args, 0, &component_name)) {
      SchemeComponent* component = new SchemeComponent(component_name);
      return mk_integer(sc, component->GetComponentNum());
    }
    return mk_integer(sc, -1);
  }

  pointer scheme_sc_component_add_instruction(scheme* sc, pointer args, SchemeComponent* component) {
    string op_args;
    if (scheme_extract_string(sc, args, 1, &op_args)) {
      component->AddInstruction(op_args);
      return sc->T;
    }
    return sc->F;
  }

  pointer scheme_sc_component_set_instructions(scheme* sc, pointer args, SchemeComponent* component) {
    vector<string> slist;
    if (scheme_extract_string_list(sc, args, 1, &slist)) {
      component->SetInstructions(slist);
      return sc->T;
    }
    return sc->F;
  }

  pointer scheme_sc_component_is_scheme_component(scheme* sc, pointer args) {
    Component* c = get_component(sc, args, 0);
    if (c == NULL) {
      return sc->F;
    }
    SchemeComponent* scheme_component = NULL;
    scheme_component = dynamic_cast<SchemeComponent*>(c);
    return (scheme_component == NULL) ? sc->F : sc->T;
  }

#ifdef _A2
  pointer marshall_turtle_call(scheme* sc, pointer args, const string & fn_name) {
    if (fn_name == "turtle-component-new") {
      return scheme_turtle_component_new(sc, args);
    } else if (fn_name == "is-turtle-graphics?") {
      return scheme_is_turtle_graphics(sc, args);
    }

    TurtleGraphics* turtle = get_turtle_graphics(sc, args, 0);
    if (turtle == NULL) {
      LOG_ERROR << "Error: Need component ID for " << fn_name;
      return sc->F;
    }

    if (fn_name == "turtle-rotate") {
      return scheme_turtle_rotate(sc, args, turtle);
    } else if (fn_name == "turtle-forward") {
      return scheme_turtle_forward(sc, args, turtle);
    } else if (fn_name == "turtle-draw-text") {
      return scheme_turtle_draw_text(sc, args, turtle);
    } else if (fn_name == "turtle-draw-point") {
      return scheme_turtle_draw_point(sc, args, turtle);
    } else if (fn_name == "turtle-clear-instructions") {
      turtle->ClearInstructions();
      return sc->T;
    } else if (fn_name == "_turtle-get-instructions") {
      return scheme_turtle_get_instructions(sc, args, turtle);
    } else if (fn_name == "turtle-get-num-steps") {
      return mk_integer(sc, turtle->GetNumSteps());
    } else if (fn_name == "turtle-get-orientation") {
      return scheme_turtle_get_orientation(sc, args, turtle);
    } else if (fn_name == "turtle-get-position") {
      return scheme_turtle_get_position(sc, args, turtle);
    } else if (fn_name == "turtle-get-affine-transform") {
      return scheme_turtle_get_affine_transform(sc, args, turtle);
    } else if (fn_name == "turtle-draw-step") {
      return scheme_turtle_draw_step(sc, args, turtle);
    }

    LOG_ERROR << "Unknown function: " << fn_name;
    return sc->F;
  }

  pointer scheme_is_turtle_graphics(scheme* sc, pointer args) {
    Component* c = get_component(sc, args, 0);
    if (c == NULL) {
      return sc->F;
    }
    TurtleGraphics* turtle = NULL;
    turtle = dynamic_cast<TurtleGraphics*>(c);
    return (turtle == NULL) ? sc->F : sc->T;
  }

  pointer scheme_turtle_component_new(scheme* sc, pointer args) {
    string component_name;
    if (scheme_extract_string(sc, args, 0, &component_name)) {
      TurtleGraphics* component = new TurtleGraphics(component_name);
      return mk_integer(sc, component->GetComponentNum());
    }
    return mk_integer(sc, -1);
  }

  pointer scheme_turtle_rotate(scheme* sc, pointer args, TurtleGraphics* turtle) {
    double theta;
    bool redrawNow;
    if (scheme_extract_double(sc, args, 1, &theta)
        && scheme_extract_bool(sc, args, 2, &redrawNow)) {
      turtle->Rotate(theta, redrawNow);
      return sc->T;
    }
    return sc->F;
  }

  pointer scheme_turtle_forward(scheme* sc, pointer args, TurtleGraphics* turtle) {
    double amount;
    bool penDown;
    bool redrawNow;
    if (scheme_extract_double(sc, args, 1, &amount)
        && scheme_extract_bool(sc, args, 2, &penDown)
        && scheme_extract_bool(sc, args, 3, &redrawNow))
      {
        turtle->Forward(amount, penDown, redrawNow);
        return sc->T;
      }
    return sc->F;
  }

  pointer scheme_turtle_draw_text(scheme* sc, pointer args, TurtleGraphics* turtle) {
    string text;
    bool redrawNow;
    if (scheme_extract_string(sc, args, 1, &text)
        && scheme_extract_bool(sc, args, 2, &redrawNow))
      {
        turtle->DrawText(text, redrawNow);
        return sc->T;
      }
    return sc->F;
  }

  pointer scheme_turtle_draw_point(scheme* sc, pointer args, TurtleGraphics* turtle) {
    bool redrawNow;
    if (scheme_extract_bool(sc, args, 1, &redrawNow)) {
      turtle->DrawPoint(redrawNow);
      return sc->T;
    }
    return sc->F;
  }

  pointer scheme_turtle_get_instructions(scheme* sc, pointer args, TurtleGraphics* turtle) {
    vector<const TurtleGraphicsOp*> ops = turtle->GetInstructions();
    vector<string> sexps;
    for (vector<const TurtleGraphicsOp*>::iterator i = ops.begin(); i != ops.end(); i++) {
      sexps.push_back((*i)->AsSexp());
    }
    return scheme_string_vector_to_list(sc, sexps);
  }

  pointer scheme_turtle_get_orientation(scheme* sc, pointer args, TurtleGraphics* turtle) {
    long stepNum;
    if (scheme_extract_long(sc, args, 1, &stepNum)) {
      return mk_real(sc, turtle->GetOrientation(stepNum));
    }
    return sc->F;
  }

  pointer scheme_turtle_get_position(scheme* sc, pointer args, TurtleGraphics* turtle) {
    long stepNum;
    if (scheme_extract_long(sc, args, 1, &stepNum)) {
      return scheme_point_to_list(sc, turtle->GetPosition(stepNum));
    }
    return sc->NIL;
  }

  pointer scheme_turtle_get_affine_transform(scheme* sc, pointer args, TurtleGraphics* turtle) {
    long stepNum;
    if (scheme_extract_long(sc, args, 1, &stepNum)) {
      return scheme_matrix_to_lists(sc, turtle->GetAffineTransform(stepNum).GetMatrix());
    }
    return sc->NIL;
  }

  pointer scheme_turtle_draw_step(scheme* sc, pointer args, TurtleGraphics* turtle) {
    long stepNum;
    if (scheme_extract_long(sc, args, 1, &stepNum)) {
      turtle->DrawStep(stepNum);
      return sc->T;
    }
    return sc->F;
  }
#endif


  /**
   * (simulate-key-event window-id press? key-character
   */
  pointer scheme_simulate_key_event(scheme* sc, pointer args) {
    XWindow* window = get_xwindow(sc, args, 0);
    if (window == NULL) {
      LOG_ERROR << "Error: Need window ID for simulate-key-event";
      return sc->F;
    }
    bool keyPress;
    char keyChar;
    if (!scheme_extract_bool(sc, args, 1, &keyPress)
        || !scheme_extract_char(sc, args, 2, &keyChar))
      {
        LOG_ERROR << "Error: Need boolean (for press/release) and character for simulate-key-event";
        return sc->F;
      }
    XApplication::GetInstance()->GetEventQueue()->AddEventToQueue(new KeyEvent(window, (keyPress) ? KeyEvent::keyPress : KeyEvent::keyRelease, keyChar));
    return sc->T;
  }

  /**
   * (simulate-mouse-event window-id event-type where)
   */
  pointer scheme_simulate_mouse_event(scheme* sc, pointer args) {
    XWindow* window = get_xwindow(sc, args, 0);
    if (window == NULL) {
      LOG_ERROR << "Error: Need window ID for simulate-mouse-event";
      return sc->F;
    }
    long eventType;
    Point where;
    if (!scheme_extract_long(sc, args, 1, &eventType)
        || !scheme_extract_point(sc, args, 2, &where))
      {
        LOG_ERROR << "Error: Need integer (for mouse event type) and point for simulate-mouse-event";
        return sc->F;
      }
    XApplication::GetInstance()->GetEventQueue()->AddEventToQueue(new MouseEvent(window, (MouseEvent::EventType)eventType, where));
    return sc->T;
  }

  pointer scheme_simulate_paint_event(scheme* sc, pointer args) {
    XWindow* window = get_xwindow(sc, args, 0);
    if (window == NULL) {
      LOG_ERROR << "Error: Need window ID for simulate-paint-event";
      return sc->F;
    }
    Rectangle damagedArea;
    if (!scheme_extract_rect(sc, args, 1, &damagedArea)) {
      LOG_ERROR << "Error: Need rectangle for simulate-paint-event";
      return sc->F;
    }
    XApplication::GetInstance()->GetEventQueue()->AddEventToQueue(new PaintEvent(window, damagedArea));
    return sc->T;
  }


  pointer scheme_sleep(scheme* sc, pointer args) {
    long amount;
    if (!scheme_extract_long(sc, args, 0, &amount)) {
      LOG_ERROR << "Error: Need integer argument to sleep";
      return sc->F;
    }
    sleep(amount);
    return sc->T;
  }

  pointer scheme_usleep(scheme* sc, pointer args) {
    long amount;
    if (!scheme_extract_long(sc, args, 0, &amount)) {
      LOG_ERROR << "Error: Need integer argument to usleep";
      return sc->F;
    }
    usleep(amount);
    return sc->T;
  }
}
