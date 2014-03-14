// STL:
#include <vector>

class Atom 
{
    public:
    
        void init(int x,int y,char t,int s);
        
        bool hasBondWith(const Atom* a) const;
        Atom* bondWith(Atom* a);
        void breakBondWith(Atom* a);
        
    public:
    
        int x,y;
        char type;
        int state;
        
        std::vector<Atom*> bonds;
};

