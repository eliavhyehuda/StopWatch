// Author: Michael Terry
#ifndef __SCHEME_COMPONENT__
#define __SCHEME_COMPONENT__

#include "Component.h"
#include "SchemeGUI.h"
#include <vector>
#include <string>

using namespace std;

namespace cs349 {

  /**
   * SchemeComponent is a Component that can execute drawing
   * operations via Scheme code. Most methods defined in Graphics can
   * be added as instructions that are executed when PaintComponent()
   * is called. Data marshalling is handled in SchemeGUI.cpp
   */
  class SchemeComponent : public Component {
  private:
    vector<string> instructions;

    void AddInstruction(const string & op_args);
    void SetInstructions(const vector<string> & instructions);

    friend pointer scheme_sc_component_add_instruction(scheme* sc, pointer args, SchemeComponent* component);
    friend pointer scheme_sc_component_set_instructions(scheme* sc, pointer args, SchemeComponent* component);

  protected:
    virtual void PaintComponent(Graphics* g);

  public:
    SchemeComponent(const string & name);

    /**
     * Clears any Scheme instructions stored by this component.
     */
    void ClearInstructions();

    /**
     * Gets all of the Scheme instructions stored by this component.
     */
    vector<string> GetInstructions() const;

    /**
     * Overrides Component::HandleKeyEvent to notify Scheme
     * interpreter that a Scheme component has received a KeyEvent.
     */
    virtual bool HandleKeyEvent(const KeyEvent & e);

    /**
     * Overrides Component::HandleMouseEvent to notify Scheme
     * interpreter that a Scheme component has received a MouseEvent.
     */
    virtual bool HandleMouseEvent(const MouseEvent & e);
  };
}

#endif // __SCHEME_COMPONENT__
