#include "XApplication.h"
#include "StopWatch.h"
#include <sstream>
#include <time.h>

using namespace std;

static int xclock;
//static int pause;
//static int pause2;
static int stopPosition = 0;
static bool IsReset = true;
//static string StartBText = "Start";
static bool IsPause = false;

namespace cs349 {

  StopWatch::StopWatch(EventQueue* eventQueue, const Rectangle & bounds) : XWindow(string("Stop Watch!"), bounds) {
    SetSize(700,400);
    this->running = false;
    this->timer = new Timer(eventQueue, 100, true, this);
    timer->Start();
    startTimeMS = 0;
  }

  StopWatch::~StopWatch() {
    timer->Stop();
    delete timer;
    timer = NULL;
  }

  unsigned long StopWatch::GetCurTime() {
    struct timespec tp;
    clock_gettime(CLOCK_REALTIME, &tp);
    return tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
  }
  
  void StopWatch::Paint(Graphics* g) {
// TODO CS349

    
    unsigned long black = XApplication::GetInstance()->GetBlackColor();
    unsigned long white = XApplication::GetInstance()->GetWhiteColor();
    g->SetForegroundColor (black);
    g->FillRect(Rectangle(0,0,700,400));
    g->SetForegroundColor (white);

    g->SettingFont();

    string StartBText;

    Point arcMin = Point (125,125);

    Point arcSec = Point (350,125);

    Point arcMil = Point (575,125);


    int xseconds = 0;
    int xminutes = 0;
    int xmilliseconds = 0;

    if (!IsReset){
      xclock = GetCurTime()-startTimeMS;

      xseconds = xclock / 1000;
      xminutes = xseconds / 60;
      xclock = xclock - xminutes * 60 * 1000;

      xseconds = xclock / 1000;
      xmilliseconds = xclock - xseconds * 1000;



      g->DrawArc(arcMin, 190, 190, 90*64, -6*64*xminutes);
      g->DrawArc(arcMin, 191, 191, 90*64, -6*64*xminutes);
      g->DrawArc(arcMin, 192, 192, 90*64, -6*64*xminutes);
      g->DrawArc(arcMin, 193, 193, 90*64, -6*64*xminutes);
      g->DrawArc(arcMin, 194, 194, 90*64, -6*64*xminutes);


      g->DrawArc(arcSec, 190, 190, 90*64, -6*64*xseconds);
      g->DrawArc(arcSec, 191, 191, 90*64, -6*64*xseconds);
      g->DrawArc(arcSec, 192, 192, 90*64, -6*64*xseconds);
      g->DrawArc(arcSec, 193, 193, 90*64, -6*64*xseconds);
      g->DrawArc(arcSec, 194, 194, 90*64, -6*64*xseconds);


      g->DrawArc(arcMil, 190, 190, 90*64, -0.36*64*xmilliseconds);
      g->DrawArc(arcMil, 191, 191, 90*64, -0.36*64*xmilliseconds);
      g->DrawArc(arcMil, 192, 192, 90*64, -0.36*64*xmilliseconds);
      g->DrawArc(arcMil, 193, 193, 90*64, -0.36*64*xmilliseconds);
      g->DrawArc(arcMil, 194, 194, 90*64, -0.36*64*xmilliseconds);

      StartBText = "Pause";
    }
    else{
      g->SetForegroundColor (black);
      g->FillRect(Rectangle(0,0,675,400));
      g->SetForegroundColor (white);
      StartBText = "Start";
      Stop();
    }




    // Start,Pause and Reset Buttons

    Rectangle StartB = Rectangle(185,275,100,75);

    Rectangle ResetB = Rectangle(415,275,100,75);

  
    g->DrawRect(StartB);

    g->DrawRect(ResetB);

    Point StartBTextP = Point (205,325);

    Point ResetBTextP = Point (435,325);

    

    if (IsPause){
      StartBText = "Cont..";
      this->running = false;
    }

    string ResetBText = "Reset";

    g->DrawText(StartBTextP, StartBText);

    g->DrawText(ResetBTextP, ResetBText);


    // Minutes, Seconds and Milliseconds Bars

    g->DrawArc(arcMin, 184, 184, 90*64, -360*64);
    g->DrawArc(arcMin, 185, 185, 90*64, -360*64);

    g->DrawArc(arcSec, 184, 184, 90*64, -360*64);
    g->DrawArc(arcSec, 185, 185, 90*64, -360*64);

    g->DrawArc(arcMil, 184, 184, 90*64, -360*64);
    g->DrawArc(arcMil, 185, 185, 90*64, -360*64);



    stringstream a;
    a<<xminutes;
    string min;
    a>>min;

    stringstream a1;
    a1<<xseconds;
    string sec;
    a1>>sec;

    stringstream a2;
    a2<<xmilliseconds;
    string mil;
    a2>>mil;

 
    g->DrawText(Point(115,125),min);
    g->DrawText(Point(340,125),sec);
    g->DrawText(Point(560,125),mil);

    g->DrawText(Point(85,160),"Minutes");
    g->DrawText(Point(310,160),"Seconds");
    g->DrawText(Point(505,160),"Milliseconds");


    XFlush(XApplication::GetInstance()->GetXDisplay());
  }
  
  bool StopWatch::HandleMouseEvent(const MouseEvent & e) {
    // TODO: Remove the following code and add your own event handling
    // code to handle mouse events
    LOG_DEBUG << "Received HandleMouseEvent in StopWatch" << e;

// TODO CS349


    if (e.GetWhere().x >= 185 && e.GetWhere().x <= 285 && e.GetWhere().y >= 275 && e.GetWhere().y <= 350) {
      if (!IsRunning()){
        IsPause = false;
        IsReset = false;
        this->running = true;
        startTimeMS = GetCurTime() - stopPosition;
      }
      else{
        IsReset = false;
        stopPosition = xclock;
        IsPause = true;
      }
    }
    else if (e.GetWhere().x >= 415 && e.GetWhere().x <= 515 && e.GetWhere().y >= 275 && e.GetWhere().y <= 350) {
      stopPosition = 0;
      IsReset = true;
      this->running = true;
      IsPause = false;
    }
    







  
/*
    switch (e.gettype) {
      case MouseEvent::MouseUp:
        if(StartAndStopButton->IsPointInRectangle(e.GetWhere())){
          if (running)
            this->stop()
          else
            this->start()
        else if (reset)
        reset
      break;

      case MouseEvent::MouseDown:
      break;

      case mouseEvent::mousemove
      break;

      case MouseEvent::mouseDrag
      break;      
*/
    return true;
  }

  void StopWatch::HandleTimerEvent(const cs349::TimerEvent & e) {
    if (running) {
      Component::Repaint();
    }
  }

  void StopWatch::Start() {
    this->running = true;
    startTimeMS = GetCurTime();
  }

  void StopWatch::Stop() {
    this->running = false;
  }

  bool StopWatch::IsRunning() const {
    return running;
  }
}
