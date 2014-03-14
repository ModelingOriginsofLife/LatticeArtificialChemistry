// local:
#include "Atom.hpp"
#include "fatal_if.hpp"

// STL:
#include <algorithm>
using namespace std;

void Atom::init(int x,int y,char t,int s)
{
    this->x = x;
    this->y = y;
    this->type = t;
    this->state = s;
}

bool Atom::hasBondWith(const Atom* a) const
{
    return find(this->bonds.begin(),this->bonds.end(),a) != this->bonds.end() ;
}

Atom* Atom::bondWith(Atom* a)
{
    fatal_if(this->hasBondWith(a),"Atom::bondWith: already bonded to them");
    fatal_if(a->hasBondWith(this),"Atom::bondWith: already bonded to us");
    this->bonds.push_back(a);
    a->bonds.push_back(this);
    return this; // allow chaining
}

void Atom::breakBondWith(Atom* a)
{
    fatal_if(!this->hasBondWith(a),"Atom::bondWith: not bonded to them");
    fatal_if(!a->hasBondWith(this),"Atom::bondWith: not bonded to us");
    this->bonds.erase(find(this->bonds.begin(),this->bonds.end(),a));
    a->bonds.erase(find(a->bonds.begin(),a->bonds.end(),this));
}
