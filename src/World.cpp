// local:
#include "World.hpp"
#include "fatal_if.hpp"

// STL:
#include <string>
#include <algorithm>
#include <iostream> // DEBUG
using namespace std;

// stdlib:
#include <stdlib.h>
#include <time.h>

void World::initAtom(int iAtom,int x,int y,char t,int s)
{
    this->atoms[iAtom].init(x,y,t,s);
    this->setAt(x,y,&this->atoms[iAtom]);
}

void World::init(int x,int y)
{
    srand((unsigned int)time(NULL));
    
    this->X = x;
    this->Y = y;
    this->grid = vector<vector<Atom*> >(X,vector<Atom*>(Y,(Atom*)NULL));
    this->iterations = 0;
    
    // add some atoms
    const int num_copies = 1;
    const int n=19 * num_copies + 300;
    this->atoms = vector<Atom>(n);
    int i=0;
    string s = "eabbcdaf";
    for(int iCopy=0;iCopy<num_copies;iCopy++)
    {
        for(int j=0;j<s.length();j++)
        {
            initAtom(i,1+j,10+iCopy*10,s[j],(j==0)?8:1);
            if(j>0)
                this->atoms[i].bondWith(&this->atoms[i-1]);
            if(false && iCopy>0)
                this->atoms[i].bondWith(&this->atoms[i-s.length()]);
            i++;
        }
    }
    int ax,ay;
    for(;i<n;i++)
    {
        do {
            ax = rand()%this->X;
            ay = rand()%this->Y;
        } while(this->getAt(ax,ay)!=NULL);
        initAtom(i,ax,ay,'a'+rand()%6,0);
    }
    
    cout << canMoveBlock(0,11,20,20,0,1) << endl;
    
    // add some reactions:
    
    // classic 2002 set (requires newly bonded atoms to jump over) (causes tangling)
    this->reactions.push_back(Reaction('e',8,false,'e',0,4,true,3));
    this->reactions.push_back(Reaction('x',4,true,'y',1,2,true,5));
    this->reactions.push_back(Reaction('x',5,false,'x',0,7,true,6));
    this->reactions.push_back(Reaction('x',3,false,'y',6,2,true,3));
    this->reactions.push_back(Reaction('x',7,true,'y',3,4,true,3));
    this->reactions.push_back(Reaction('f',4,true,'f',3,8,false,8));
    this->reactions.push_back(Reaction('x',2,true,'y',8,9,true,1));
    this->reactions.push_back(Reaction('x',9,true,'y',9,8,false,8));
    
    // adapted from 2007 set (allows bonding either side) (causes tangling)
    /*this->reactions.push_back(Reaction('e',8,false,'e',0,2,true,3));
    this->reactions.push_back(Reaction('x',2,true,'y',1,7,true,4));
    this->reactions.push_back(Reaction('x',4,false,'y',3,5,true,7));
    this->reactions.push_back(Reaction('x',5,false,'x',0,6,true,6));
    this->reactions.push_back(Reaction('x',6,false,'y',7,3,true,4));
    this->reactions.push_back(Reaction('x',6,true,'y',4,1,false,2));
    this->reactions.push_back(Reaction('x',7,true,'y',1,2,true,2));
    this->reactions.push_back(Reaction('f',2,true,'f',3,8,false,8));
    this->reactions.push_back(Reaction('x',2,true,'y',8,9,true,1));
    this->reactions.push_back(Reaction('x',9,true,'y',9,8,false,8));*/
    
    /* // adapted from 2003 set (allows bonding either side) (no tangling but needs 3-way reactions)
    this->reactions.push_back(Reaction('e',6,false,'e',0,4,true,3));
    this->reactions.push_back(Reaction('x',3,true,'x',4,true,'y',1,false,4,true,4,true,2,true));
    this->reactions.push_back(Reaction('x',2,false,'x',0,3,true,5));
    this->reactions.push_back(Reaction('x',3,true,'x',5,false,'y',4,true,4,true,3,true,4,false));
    this->reactions.push_back(Reaction('f',3,true,'f',4,6,false,6));
    this->reactions.push_back(Reaction('x',6,true,'y',4,1,true,7));
    this->reactions.push_back(Reaction('x',7,true,'y',7,6,false,6));*/
    
    /* // Dave Mann's set (improved a little) (no tangling)
    // grab atom of correct type
    this->reactions.push_back(Reaction('x',10,false,'a',0,20,true,21));
    this->reactions.push_back(Reaction('x',11,false,'b',0,20,true,21));
    this->reactions.push_back(Reaction('x',12,false,'c',0,20,true,21));
    this->reactions.push_back(Reaction('x',13,false,'d',0,20,true,21));
    this->reactions.push_back(Reaction('x',14,false,'e',0,20,true,21));
    this->reactions.push_back(Reaction('f',1,false,'f',0,20,true,21));
    // send up message: (20 = "atom has been grabbed")
    this->reactions.push_back(Reaction('x',20,true,'y',30,60,true,20));
    this->reactions.push_back(Reaction('x',20,true,'y',31,60,true,20));
    this->reactions.push_back(Reaction('x',20,true,'y',32,60,true,20));
    this->reactions.push_back(Reaction('x',20,true,'y',33,60,true,20));
    this->reactions.push_back(Reaction('x',20,true,'y',34,60,true,20));
    this->reactions.push_back(Reaction('x',20,true,'y',35,60,true,20));
    // found next uncopied atom, so send down its type (30='a', 32='b', ...)
    this->reactions.push_back(Reaction('x',20,true,'a',1,30,true,30));
    this->reactions.push_back(Reaction('x',20,true,'b',1,31,true,31));
    this->reactions.push_back(Reaction('x',20,true,'c',1,32,true,32));
    this->reactions.push_back(Reaction('x',20,true,'d',1,33,true,33));
    this->reactions.push_back(Reaction('x',20,true,'e',1,34,true,34));
    this->reactions.push_back(Reaction('x',20,true,'f',1,35,true,35));
    // copy ‘type’ message down the molecule
    this->reactions.push_back(Reaction('x',60,true,'y',30,30,true,30));
    this->reactions.push_back(Reaction('x',60,true,'y',31,31,true,31));
    this->reactions.push_back(Reaction('x',60,true,'y',32,32,true,32));
    this->reactions.push_back(Reaction('x',60,true,'y',33,33,true,33));
    this->reactions.push_back(Reaction('x',60,true,'y',34,34,true,34));
    this->reactions.push_back(Reaction('x',60,true,'y',35,35,true,35));
    // message has reached grabbing end, set required atom type (10='a', 11='b', ...)
    this->reactions.push_back(Reaction('x',21,true,'y',30,10,true,30));
    this->reactions.push_back(Reaction('x',21,true,'y',31,11,true,31));
    this->reactions.push_back(Reaction('x',21,true,'y',32,12,true,32));
    this->reactions.push_back(Reaction('x',21,true,'y',33,13,true,33));
    this->reactions.push_back(Reaction('x',21,true,'y',34,14,true,34));
    // detect that copy is complete (e21-60-60-...-f60-f60-60-...-60-e20)
    this->reactions.push_back(Reaction('e',21,true,'x',60,2,true,23));
    // send ‘copy complete’ message (state 23)
    this->reactions.push_back(Reaction('x',23,true,'y',60,2,true,23));
    // copy complete message reaches other end of molecule
    this->reactions.push_back(Reaction('e',20,true,'x',23,2,true,2));
    // separate copy from original
    this->reactions.push_back(Reaction('f',2,true,'f',2,3,false,3));
    // tell everything to reset to state 1
    this->reactions.push_back(Reaction('x',3,true,'y',2,1,true,3));
    this->reactions.push_back(Reaction('e',3,true,'x',1,1,true,1));*/
}

Atom* World::getAt(int x,int y) const
{
    fatal_if(this->isOffGrid(x,y),"World::getAt: coordinates out of range");
    return this->grid[x][y];
}

void World::setAt(int x,int y,Atom* a)
{
    fatal_if(this->isOffGrid(x,y),"World::setAt: coordinates out of range");
    fatal_if(a==NULL && this->getAt(x,y)==NULL,"World::setAt: grid location already empty");
    fatal_if(a!=NULL && this->getAt(x,y)!=NULL,"World::setAt: grid location not empty");
    this->grid[x][y] = a;
}

void World::doTimeStep()
{
    // do the physics (movement, no reactions)

    const bool do_individual_movement   = true;
    const bool do_bonded_atoms_movement = false;
    const bool do_mpeg_block_movement   = false;
    
    if( do_individual_movement )
    {
        this->moveSomeIndividualAtoms();
    }
    
    if( do_bonded_atoms_movement )
    {
        this->moveSomeBondedAtoms();
    }
    
    if( do_mpeg_block_movement )
    {
        const int BLOCK_MOVES_PER_REACTION_CHECK = 10; // (should be proportional to size somehow?)
        // (no point re-checking for reactions if many atoms haven't moved)
        for(int i=0;i<BLOCK_MOVES_PER_REACTION_CHECK;i++)
            this->moveABlock();
    }
        
    // do the chemistry (reactions, no movement)
    this->doReactions();
    
    this->iterations++;
}

void World::doReactions()
{
    for(vector<Atom>::iterator it = this->atoms.begin();it!=this->atoms.end();it++)
    {
        Atom *a = &*it;
        for(int iNeighbor=0;iNeighbor<4;iNeighbor++)
        {
            int tx = a->x+vonNeumann[iNeighbor][0];
            int ty = a->y+vonNeumann[iNeighbor][1];
            if(this->isOffGrid(tx,ty)) continue;
            Atom *b = this->getAt(tx,ty);
            if(b==NULL) continue;
            // (only do 2-way reactions for now)
            if(this->reactIfCan(a,b)) break;
            if(this->reactIfCan(b,a)) break;
        }
    }
}

bool World::reactIfCan(Atom *a,Atom *b)
{
    for(vector<Reaction>::const_iterator it=this->reactions.begin();it!=reactions.end();it++)
    {
        const Reaction &r = *it;
        if(r.n_inputs!=2) continue;
        if( r.a_state==a->state && r.b_state==b->state)
        {
            if( r.a_type==a->type || Reaction::isVariable(r.a_type) ) 
            {
                if( r.b_type==b->type || 
                    ( Reaction::isVariable(r.b_type) && r.a_type!=r.b_type ) || 
                    ( Reaction::isVariable(r.b_type) && r.a_type==r.b_type && a->type==b->type) ) 
                {
                    bool currently_bonded = a->hasBondWith(b);
                    if( ( r.current_ab_bond && currently_bonded ) || ( !r.current_ab_bond && !currently_bonded ) )
                    {
                        a->state = r.future_a_state;
                        b->state = r.future_b_state;
                        if(r.current_ab_bond && !r.future_ab_bond)
                            a->breakBondWith(b);
                        else if(!r.current_ab_bond && r.future_ab_bond)
                            a->bondWith(b);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void World::moveABlock()
{
    // move a rectangular part of the world by one square horizontally or vertically
    int sx = rand()%this->X;
    int sy = rand()%this->Y;
    int ex = sx + rand()%(this->X-sx);
    int ey = sy + rand()%(this->Y-sy);
    int iMove = rand()%4;
    int dx = vonNeumann[iMove][0];
    int dy = vonNeumann[iMove][1];
    if(canMoveBlock(sx,sy,ex,ey,dx,dy))
        moveBlock(sx,sy,ex,ey,dx,dy);
}

bool bondTooLong(int x1,int y1,int x2,int y2)
{
    enum BondLength { Moore, MooreR2, vonNeumann, vonNeumannR2 };
    
    const BondLength chosenBondLength = vonNeumannR2;
    
    switch( chosenBondLength )
    {
        case Moore:           return abs(x2-x1)>1 || abs(y2-y1)>1 ;
        case MooreR2:         return abs(x2-x1)>2 || abs(y2-y1)>2 ;
        case vonNeumann:      return abs(x2-x1) + abs(y2-y1) > 1 ;
        case vonNeumannR2:    return abs(x2-x1) + abs(y2-y1) > 2 ;
    }
}

bool inRect(int x,int y,int start_x,int start_y,int end_x,int end_y)
{
    return x>=start_x && x<=end_x && y>=start_y && y<=end_y;
}

double electrostaticPotential(Atom *a,Atom *b)
{
    if(a->bonds.empty() || b->bonds.empty()) return 0.0;
    if( (a->type=='a' && b->type=='a'))// || (b->type=='c' && a->type=='c') )
        return -1.0; // 'a1s' and 'c' attract (when both have bonds)
    else
        return 0.0;
}

bool World::canMoveBlock(int sx,int sy,int ex,int ey,int dx,int dy) const
{
    if( this->isOffGrid(sx+dx,sy+dy) || this->isOffGrid(ex+dx,ey+dy) ) return false;
    
    double energy_before=0,energy_after=0;
    
    // can we shift this part of the world without collisions or stretching any bonds too far?
    for(int x=sx;x<=ex;x++)
    {
        for(int y=sy;y<=ey;y++)
        {
            // only consider squares on the edge
            if(x>sx && x<ex && y>sy && y<ey) continue;
            // retrieve the atom (if any)
            Atom *a = this->getAt(x,y);
            if(a==NULL) continue;
            // would this atom fall on an existing atom outside the block?
            int tx = a->x+dx;
            int ty = a->y+dy;
            if( !inRect(tx,ty,sx,sy,ex,ey) && this->getAt(tx,ty)!=NULL ) return false;
            // would this move over-stretch any of this atom's bonds with others outside the block?
            for(vector<Atom*>::const_iterator it=a->bonds.begin();it!=a->bonds.end();it++)
            {
                Atom *b = *it;
                if( inRect(b->x,b->y,sx,sy,ex,ey) ) continue; // b is in the block too
                if( bondTooLong(tx,ty,b->x,b->y) ) return false;
            }
            // now consider the electrostatic potential energy:
            // before the move:
            for(int iNeighbor=0;iNeighbor<4;iNeighbor++)
            {
                // for each out-of-block neighbor in our current position, compute some energy term
                int nx = a->x + vonNeumann[iNeighbor][0];
                int ny = a->y + vonNeumann[iNeighbor][1];
                if( inRect(nx,ny,sx,sy,ex,ey) ) continue; // ignore in-block neighbors
                if( this->isOffGrid(nx,ny) ) continue; // ignore off-grid positions
                Atom *b = this->getAt(nx,ny);
                if(b!=NULL)
                    energy_before += electrostaticPotential(a,b);
            }
            // after the move:
            for(int iNeighbor=0;iNeighbor<4;iNeighbor++)
            {
                // for each out-of-block neighbor in our new position, compute some energy term
                int nx = a->x + dx + vonNeumann[iNeighbor][0];
                int ny = a->y + dy + vonNeumann[iNeighbor][1];
                if( inRect(nx,ny,sx,sy,ex,ey) ) continue; // ignore in-block neighbors
                if( this->isOffGrid(nx,ny) ) continue; // ignore off-grid positions
                Atom *b = this->getAt(nx,ny);
                if(b!=NULL)
                    energy_after += electrostaticPotential(a,b);
            }
        }
    }
    
    const bool consider_electrostatic_energy = false;
    
    if(consider_electrostatic_energy && energy_after > energy_before && rand()%100) // energy has increased: reject this move (most of the time)
        return false;
        
    return true;
}

void World::moveBlock(int sx,int sy,int ex,int ey,int dx,int dy)
{
    // collect the atoms, move them
    vector<Atom*> movers;
    for(int x=sx;x<=ex;x++)
    {
        for(int y=sy;y<=ey;y++)
        {
            Atom *a = this->getAt(x,y);
            if(a!=NULL)
                movers.push_back(a);
        }
    }
    this->moveTheseAtoms(movers,dx,dy);
}

void World::moveTheseAtoms(const vector<Atom*>& movers,int dx,int dy)
{
    // pull them from the grid, move them, put them back
    for(vector<Atom*>::const_iterator it=movers.begin();it!=movers.end();it++)
    {
        Atom *a = *it;
        this->setAt(a->x,a->y,NULL);
        a->x += dx;
        a->y += dy;
    }
    for(vector<Atom*>::const_iterator it=movers.begin();it!=movers.end();it++)
    {
        Atom *a = *it;
        this->setAt(a->x,a->y,a);
    }
}

void World::moveSomeIndividualAtoms()
{
    // allow every atom one chance to move
    for( vector<Atom>::iterator a = this->atoms.begin(); a != this->atoms.end(); ++a )
    {
        int iMove = rand()%4;
        int dx = vonNeumann[iMove][0];
        int dy = vonNeumann[iMove][1];
        // can we make this move?
        if( isOffGrid( a->x + dx, a->y + dy ) ) 
        {
            continue;
        }
        // would this atom land on another?
        if( this->getAt( a->x + dx, a->y + dy ) )
        {
            continue; 
        }
        // would this move over-stretch any of this atom's bonds with non-movers?
        bool stillOK = true;
        for( vector<Atom*>::const_iterator it = a->bonds.begin(); it != a->bonds.end(); ++it )
        {
            Atom *b = *it;
            if( bondTooLong( a->x + dx, a->y + dy, b->x, b->y ) ) 
            {
                stillOK = false;
                break; 
            }
        }
        if( !stillOK)
        {
            continue; 
        }
        
        this->setAt( a->x, a->y, NULL );
        a->x += dx;
        a->y += dy;
        this->setAt( a->x, a->y, &(*a) );
    }
}

void World::moveSomeBondedAtoms()
{
    vector<Atom*> movers;
    movers.push_back(&this->atoms[rand()%this->atoms.size()]);
    Atom *atom_to_add;
    do {
        // find an atom to add (if any)
        atom_to_add = NULL;
        for(vector<Atom*>::const_iterator it=movers.begin();it!=movers.end() && atom_to_add==NULL;it++)
        {
            Atom *a = *it;
            for(vector<Atom*>::const_iterator it2=a->bonds.begin();it2!=a->bonds.end();it2++)
            {
                Atom *b = *it2;
                if( find(movers.begin(),movers.end(),b) == movers.end() )
                {
                    atom_to_add = b;
                    break;
                }
            }
        }
        if(atom_to_add)
            movers.push_back(atom_to_add);
    } while(atom_to_add);// && rand()%2); // prefer moving fewer atoms
    
    int iMove = rand()%4;
    int dx = vonNeumann[iMove][0];
    int dy = vonNeumann[iMove][1];
    // can we make this move?
    bool can_move = true;
    for(vector<Atom*>::const_iterator it=movers.begin();it!=movers.end() && can_move;it++)
    {
        Atom *a = *it;
        if(isOffGrid(a->x+dx,a->y+dy)) { can_move = false; break; }
        // would this atom fall on an existing non-mover atom?
        {
            Atom *b = this->getAt(a->x+dx,a->y+dy);
            if(b && find(movers.begin(),movers.end(),b)==movers.end()) 
            { 
                can_move=false; 
                break; 
            }
        }
        // would this move over-stretch any of this atom's bonds with non-movers?
        for(vector<Atom*>::const_iterator it=a->bonds.begin();it!=a->bonds.end();it++)
        {
            Atom *b = *it;
            if(find(movers.begin(),movers.end(),b)!=movers.end()) continue;
            if(bondTooLong(a->x+dx,a->y+dy,b->x,b->y)) { can_move = false; break; }
        }
    }
    if(can_move)
        moveTheseAtoms(movers,dx,dy);
}

