// local:
#include "World.hpp"

// wxWidgets:
#include <wx/wx.h>

class World_wx : public World
{
    public:
        void Draw(wxGraphicsContext *pGC,float scale);
};
