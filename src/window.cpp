// wxWidgets:
#include <wx/wx.h>
#include <wx/dcbuffer.h>

// local:
#include "World_wx.hpp"
#include "fatal_if.hpp"

// STL:
#include <iostream>
#include <sstream>
using namespace std;

// workaround for cross-platform timing differences
#ifdef _WIN32
    #include <sys/timeb.h>
    #include <sys/types.h>
    #include <winsock.h>
    // http://www.linuxjournal.com/article/5574
    void gettimeofday(struct timeval* t,void* timezone)
    {       struct _timeb timebuffer;
          _ftime( &timebuffer );
          t->tv_sec=timebuffer.time;
          t->tv_usec=1000*timebuffer.millitm;
    }
#else
    #include <sys/time.h>
#endif

class MyApp: public wxApp
{
    virtual bool OnInit();
};

class MyFrame: public wxFrame
{
public:

    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

    void OnPaint(wxPaintEvent& event);
    void OnIdle(wxIdleEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnSize(wxSizeEvent& event);

    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    
    void OnRun(wxCommandEvent& event);
    void OnUpdateRun(wxUpdateUIEvent& event);

    void OnStop(wxCommandEvent& event);
    void OnUpdateStop(wxUpdateUIEvent& event);

    void OnReallyFast(wxCommandEvent& event);
    void OnUpdateReallyFast(wxUpdateUIEvent& event);

    DECLARE_EVENT_TABLE()

protected:
    
    World_wx world;
    
    bool running;
    bool really_fast;
    float its_per_second; // how fast are we running?

    struct timeval tod_record;
    double tod_before,tod_after;
    int its_before;

    float scale;

    bool save_video;
};

namespace ID {
    enum
    {
        Quit = 1,
        About,
        Run,
        Stop,
        ReallyFast,
    };
};

BEGIN_EVENT_TABLE(MyFrame, wxFrame)

    EVT_PAINT(MyFrame::OnPaint)
    EVT_IDLE(MyFrame::OnIdle)
    EVT_SIZE(MyFrame::OnSize)
    EVT_MOTION(MyFrame::OnMouseMove)

    EVT_MENU(ID::Quit, MyFrame::OnQuit)
    EVT_MENU(ID::About, MyFrame::OnAbout)

    EVT_MENU(ID::Run, MyFrame::OnRun)
    EVT_UPDATE_UI(ID::Run, MyFrame::OnUpdateRun)
    EVT_MENU(ID::Stop, MyFrame::OnStop)
    EVT_UPDATE_UI(ID::Stop, MyFrame::OnUpdateStop)
    EVT_MENU(ID::ReallyFast, MyFrame::OnReallyFast)
    EVT_UPDATE_UI(ID::ReallyFast, MyFrame::OnUpdateReallyFast)

END_EVENT_TABLE()

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame( _("lattice_shape"), wxDefaultPosition,
                                  wxSize(300,200) );
    frame->Show(true);
    SetTopWindow(frame);
    return true;
} 

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame( NULL, -1, title, pos, size )
{
    wxMenuBar *menuBar = new wxMenuBar;
    {
        wxMenu *file_menu = new wxMenu;
        file_menu->Append( ID::Quit, _("E&xit") );
        menuBar->Append( file_menu, _("&File") );
    }
    {
        wxMenu *actions_menu = new wxMenu;
        actions_menu->Append( ID::Run, _("&Run\tF5") );
        actions_menu->AppendCheckItem( ID::ReallyFast, _("Really &fast\tF7") );
        actions_menu->Append( ID::Stop, _("&Stop\tF6") );
        menuBar->Append( actions_menu, _("&Actions") );
    }
    {
        wxMenu *help_menu = new wxMenu;
        help_menu->Append( ID::About, _("&About...") );
        menuBar->Append( help_menu, _("&Help") );
    }
    SetMenuBar( menuBar );

    CreateStatusBar();
    SetStatusText( _("Ready") );
    
    // initialise our world
    running = false;
    really_fast = false;
    its_per_second = 0.0F;
    save_video = false;

    world.init(30,30);
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(TRUE);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox( _("Some crazy lattice chemistry."),
                  _("About lattice_shape"),
                  wxOK | wxICON_INFORMATION, this);
}

void MyFrame::OnIdle(wxIdleEvent& event)
{
    // we drive our game loop by onIdle events
    if(!this->running) return;

    try 
    {
        this->world.doTimeStep();
    }
    catch(const char* s)
    {
        //wxMessageBox(wxString(s,wxConvUTF8));
        cout << s << endl;
        throw;
    }

    // report the number of iterations so far
    if(!this->really_fast || this->world.getIterations()%10000==0)
    {
        gettimeofday(&tod_record, 0);
        tod_before = tod_after;
        tod_after = ((double) (tod_record.tv_sec)) + ((double) (tod_record.tv_usec)) / 1.0e6;
        this->its_per_second = (this->world.getIterations()-this->its_before) / (tod_after - tod_before);
        this->its_before = this->world.getIterations();
        ostringstream oss;
        oss << "Iterations: " << this->world.getIterations() << " (" << int(its_per_second) << " its/s)";
        this->SetStatusText(wxString(oss.str().c_str(),wxConvUTF8));
        this->Refresh(false);
    }

    if(!this->really_fast)
        wxMilliSleep(5);

    event.RequestMore(); // trigger another onIdle event
}

void MyFrame::OnPaint(wxPaintEvent& event)
{
    wxBufferedPaintDC dc( this );        
    wxGraphicsContext* pGC = wxGraphicsContext::Create( dc );
    fatal_if(!pGC,"Failed to create wxGraphicsContext");
    
    // blank the area
    pGC->SetPen(*wxWHITE_PEN);
    pGC->SetBrush(*wxWHITE_BRUSH);
    pGC->DrawRectangle(0,0,this->GetClientSize().x,this->GetClientSize().y);

    bool save_this_frame = this->save_video;

    wxBitmap bmp;
    if(save_this_frame)
    {
        int sx = this->world.getSizeX()*scale;
        int sy = this->world.getSizeY()*scale;
        // round up to multiple of 4 to comply with video encoding
        while(sx%4) sx++;
        while(sy%4) sy++;
        bmp.Create(sx,sy);
        dc.SelectObject(bmp);
    }

    this->world.Draw(pGC,this->scale);

    if(save_this_frame)
    {
        ostringstream oss;
        oss << "frame_" << this->world.getIterations()/2 << ".bmp";
        bmp.SaveFile(wxString(oss.str().c_str(),wxConvUTF8),wxBITMAP_TYPE_BMP);
    }

    delete pGC;
}

void MyFrame::OnRun(wxCommandEvent& event) 
{
    this->running = true;
    this->Refresh();
}

void MyFrame::OnUpdateRun(wxUpdateUIEvent &event) 
{
    event.Enable(!this->running);
}

void MyFrame::OnStop(wxCommandEvent& event) 
{
    this->running = false;
}

void MyFrame::OnUpdateStop(wxUpdateUIEvent &event) 
{
    event.Enable(this->running);
}

void MyFrame::OnReallyFast(wxCommandEvent& event) 
{
    this->really_fast = !this->really_fast;
}

void MyFrame::OnUpdateReallyFast(wxUpdateUIEvent &event) 
{
    event.Enable(this->running);
    event.Check(this->really_fast);
}

void MyFrame::OnMouseMove(wxMouseEvent& event) 
{
    // if stopped and mouse-moving over the window then report the status of the cell being pointed at
    if(!running)
    {
        int sx = event.GetX()/scale;
        int sy = event.GetY()/scale;
        ostringstream oss;
        if(!this->world.isOffGrid(sx,sy))
        {
            Atom* a = this->world.getAt(sx,sy);
            if(a!=NULL)
                oss << a->type << a->state;
        }
        this->SetStatusText(wxString(oss.str().c_str(),wxConvUTF8));
    }
    
    event.Skip();
}

void MyFrame::OnSize(wxSizeEvent& event)
{
    float scale_x = this->GetClientSize().GetWidth()/this->world.getSizeX();
    float scale_y = this->GetClientSize().GetHeight()/this->world.getSizeY();
    this->scale = min(scale_x,scale_y);

    this->Refresh(true);
}
