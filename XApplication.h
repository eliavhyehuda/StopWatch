// Author: Michael Terry

#ifndef __X_APPLICATION__
#define __X_APPLICATION__

#include <X11/Xlib.h>
#include <vector>
#include "EventQueue.h"

using namespace std;

namespace cs349
{

  /**
   * XApplication is the back-bone of your application. XApplication
   * does the following:
   * - Initializes and maintains the connection with X, and provides
   *   access to important X variables (such as the values for black
   *   and white colors).
   * - Implements the event loop via its Run() method. In the event
   *   loop, new events are received from X, packaged up as our own
   *   custom Event objects, and added to the EventQueue object.
   *   Events are then peeled off the event queue in FIFO order, and
   *   asked to deliver themselves.
   * 
   * XApplication is a *singleton*: There is only one instance of it in
   * existence throughout the lifetime of the application. As such,
   * you don't directly instantiate the XApplication object. Instead,
   * you call XApplication::GetInstance().
   * 
   */
  class XApplication
  {
  private:
    static XApplication* s_Instance;

    Display*   display; ///< A pointer to the display
    int        screen;  ///< Represents the default screen to display on
    bool       quit;    ///< Whether the app should continue to run, or quit
    EventQueue eventQueue; ///< The event queue used to manage events
  
    /**
     * Private constructor so only one instance can be created (it's a
     * singleton). Use GetInstance() to construct/get a pointer to the
     * actual object.
     */
    XApplication();
    friend int ErrorHandler(Display* display);
    int        HandleXError(Display* display);

    /**
     * This method checks whether any X events are pending. If so, it
     * packages them up as our own events and adds them to the eventQueue.
     */
    void       CheckForXEvents();
  
  public:
    /**
     * Gets a pointer to the XApplication object (constructing it, if
     * necessary).
     */
    static XApplication* GetInstance();
    ~XApplication();
    
    /**
     * Retrieves the X Display for this X server.
     */
    Display* GetXDisplay() const;

    /**
     * Retrieves the default screen for this X server.
     */
    int GetDefaultScreen() const;

    /**
     * Gets the value representing black from this X server.
     */
    unsigned long GetBlackColor() const;

    /**
     * Gets the value representing white from this X server.
     */
    unsigned long GetWhiteColor() const;
  
    /**
     * Returns a pointer to this app's EventQueue object, to be able
     * to add Events to it.
     */
    EventQueue* GetEventQueue();

    /**
     * Starts the application event loop. This will run until "Quit"
     * is called or the user closes the window.
     */
    void Run();
  
    /**
     * Stops the event loop.
     */
    void Quit();

  };

}

#endif /* __APPLICATION__ */
