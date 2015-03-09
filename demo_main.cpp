#include <string.h>

#include "XApplication.h"
#include "XWindow.h"
#include "SchemeInterpreter.h"
#include "SchemeComponent.h"
#include "Logging.h"

using namespace std;
using namespace cs349;

const char* scheme_init_fname = "init.scm";
const char* cs349_init_fname  = "cs349_init.scm";

class Clock : public Component, public TimerListener
{
private:
  Timer* t;
  string timeStr;
	
public:
  Clock(EventQueue* eventQueue) : Component("clock") {
    t = new Timer(eventQueue, 500, true, this);
    t->Start();
  }
  virtual ~Clock() {
    t->Stop();
    delete t;
  }
	
  virtual void PaintComponent(Graphics* g) {
    g->DrawText(Point(0,10), this->timeStr.c_str());
  }
	
  virtual bool HandleMouseEvent(const MouseEvent & e) {
    if (!Component::HandleMouseEvent(e)) {
      LOG_DEBUG << "Timer got mouse event number: " << e.GetType();
    }
    return true;
  }

  virtual void HandleTimerEvent(const cs349::TimerEvent & e) {
    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
		
    timeStr = asctime(timeinfo);
    this->Repaint();
  }
};

int main (int argc, char *argv[])
{
  FLAGS_logtostderr = 1;
  //  FLAGS_log_dir = "./log";
  google::InitGoogleLogging(argv[0]);

  LOG_DEBUG << "Starting up main.cpp";

  XApplication* app = XApplication::GetInstance();
  scheme_interpreter_init(scheme_init_fname, cs349_init_fname, argc, argv);

  XWindow* window = new XWindow(string("My Test Window"), Rectangle(50, 50, 450, 450));
  Clock* clock = new Clock(app->GetEventQueue());
  SchemeComponent* schemeComponent = new SchemeComponent("Scheme Component");
	
  clock->SetBounds(Rectangle(10, 10, 200, 30));
  schemeComponent->SetBounds(Rectangle(0, 40, 450, 400));
	
  window->AddComponent(clock);
  window->AddComponent(schemeComponent);
	
  clock->SetVisible(true);
  schemeComponent->SetVisible(true);
  window->SetVisible(true);
  
  app->Run();
	
  vector<XWindow*> windows = XWindow::GetWindowList();
  for (vector<XWindow*>::iterator win = windows.begin(); win != windows.end(); win++) {
    delete (*win);
  }

  delete app;

  return scheme_interpreter_shutdown();
}
