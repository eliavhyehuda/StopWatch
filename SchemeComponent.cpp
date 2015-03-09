#include "SchemeComponent.h"
#include "SchemeInterpreter.h"
#include "SchemeUtils.h"
#include "XWindow.h"
#include "Logging.h"
#include <sstream>

using namespace std;

namespace cs349 {

  SchemeComponent::SchemeComponent(const string & name)
    : Component(name)
  {
    ; // no-op
  }

  void SchemeComponent::PaintComponent(Graphics* g) {
    XWindow* win = this->GetParentWindow();
    if (win == NULL) {
      return;
    }
    for (vector<string>::iterator i = instructions.begin(); i != instructions.end(); i++) {
      scheme_interpreter_execute_graphics_instruction(win->GetWindowNum(), i->c_str());
    }
  }

  void SchemeComponent::AddInstruction(const string & op_args) {
    instructions.push_back(op_args);
    this->Repaint();
  }

  void SchemeComponent::ClearInstructions() {
    instructions.erase(instructions.begin(), instructions.end());
    this->Repaint();
  }

  vector<string> SchemeComponent::GetInstructions() const {
    return instructions;
  }

  void SchemeComponent::SetInstructions(const vector<string> & instructions) {
    this->instructions = instructions;
    this->Repaint();
  }


  bool SchemeComponent::HandleKeyEvent(const KeyEvent & e)
  {
    bool componentResult = Component::HandleKeyEvent(e);
    stringstream ss;

    ss << "(scheme-component-handle-key-event " << this->GetComponentNum() << " ";
    switch (e.GetType()) {
    case KeyEvent::keyPress:
      ss << "'key-press";
      break;

    case KeyEvent::keyRelease:
      ss << "'key-release";
      break;

    default:
      LOG_ERROR << "Error: Unknown KeyEvent type: " << e.GetType();
      return componentResult;
    }
    ss << " #\\" << e.GetChar();
    ss << ")";

    scheme* sc = NULL;
    pointer result;
    int error;
    bool schemeResult = false;
    error = scheme_interpreter_evaluate_string(ss.str().c_str(), &sc, &result);

    if (error) {
      LOG_ERROR << "Error evaluating string: " << ss.str();
    } else {
      if (sc != NULL) {
        schemeResult = result == sc->T;
      } else {
        LOG_ERROR << "Error: No pointer Scheme interpreter returned;";
      }
    }
    return componentResult || schemeResult;
  }

  bool SchemeComponent::HandleMouseEvent(const MouseEvent & e)
  {
    bool componentResult = Component::HandleMouseEvent(e);
    stringstream ss;

    ss << "(scheme-component-handle-mouse-event " << this->GetComponentNum() << " ";
    switch (e.GetType()) {
    case MouseEvent::mouseDown:
      ss << "'mouse-down";
      break;

    case MouseEvent::mouseUp:
      ss << "'mouse-up";
      break;

    case MouseEvent::mouseMove:
      ss << "'mouse-move";
      break;

    case MouseEvent::mouseDrag:
      ss << "'mouse-drag";
      break;

    default:
      LOG_ERROR << "Error: Unknown MouseEvent type: " << e.GetType();
      return componentResult;
    }
    ss << " (list " << e.GetWhere().x << " " << e.GetWhere().y;
    ss << "))";

    scheme* sc = NULL;
    pointer result;
    int error;
    bool schemeResult = false;
    error = scheme_interpreter_evaluate_string(ss.str().c_str(), &sc, &result);

    if (error) {
      LOG_ERROR << "Error evaluating string: " << ss.str();
    } else {
      if (sc != NULL) {
        schemeResult = result == sc->T;
      } else {
        LOG_ERROR << "Error: No pointer Scheme interpreter returned;";
      }
    }
    return componentResult || schemeResult;
  }
}
