// local
#include "World_wx.hpp"

// STL:
#include <sstream>
using namespace std;

wxColor GetColor(char type)
{
    switch(type)
    {
        default:
        case 'e': return wxColor(255,0,0); // red
        case 'f': return wxColor(0,255,0); // green
        case 'a': return wxColor(220,220,0); // yellow
        case 'b': return wxColor(128,128,128); // grey
        case 'c': return wxColor(0,255,255); // cyan
        case 'd': return wxColor(0,0,255); // blue
    }
}

void World_wx::Draw(wxGraphicsContext *pGC,float scale)
{
    // draw the background rectangle
    pGC->SetPen(*wxBLACK_PEN);
    pGC->SetBrush(*wxWHITE_BRUSH);
    pGC->DrawRectangle(0,0,int(this->X*scale),int(this->Y*scale));

    // draw the atoms
    for(vector<Atom>::const_iterator it = this->atoms.begin();it!=this->atoms.end();it++)
    {
        const Atom *a = &*it;
        // draw the cell
        wxColor color = GetColor(a->type);
        if(a->state==0)
        {
            // make the color somewhat transparent
            color.Set(color.Red(),color.Green(),color.Blue(),20);
        }
        pGC->SetBrush(wxBrush(color));
        pGC->SetPen(wxPen(color));
        pGC->DrawRectangle(a->x*scale+1,a->y*scale+1,scale-2,scale-2);
        if(false && a->state>0)
        {
            // draw the cell's state
            ostringstream oss;
            oss << a->state;
            wxString t(oss.str().c_str(),wxConvUTF8);
            double w,h,descent,externalLeading;
            pGC->SetFont(*wxNORMAL_FONT,*wxBLACK);
            pGC->GetTextExtent(t,&w,&h,&descent,&externalLeading); // will use rect size to center
            pGC->DrawText(t,(int)(a->x*scale+scale/2)-w/2,(int)(a->y*scale+scale/2)-h/2);
        }
    }

    // draw the bonds
    pGC->SetPen(wxPen(wxColor(0,0,0,50),3));
    for(vector<Atom>::const_iterator it = this->atoms.begin();it!=this->atoms.end();it++)
    {
        const Atom *a = &*it;
        // draw the cell's bonds as lines
        for(vector<Atom*>::const_iterator it2=a->bonds.begin();it2!=a->bonds.end();it2++)
        {
            Atom *b = *it2;
            if(b<a) continue; // (avoid drawing the same line more than once)

            pGC->StrokeLine(int(a->x*scale+scale/2),int(a->y*scale+scale/2),
                int(b->x*scale+scale/2),int(b->y*scale+scale/2));
        }
    }
}
