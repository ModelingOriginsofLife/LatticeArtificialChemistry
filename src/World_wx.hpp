// local:
#include "World.hpp"

// wxWidgets:
#include <wx/wxprec.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/graphics.h>

class World_wx : public World
{
    public:
        void Draw(wxGraphicsContext *pGC,float scale);
};
