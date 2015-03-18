// local
#include "Atom.hpp"
#include "Reaction.hpp"

// STL:
#include <vector>

const int vonNeumann[4][2] = {{1,0},{0,1},{-1,0},{0,-1}};

class World
{
    public:

		World() : X(0), Y(0) {}
    
        void init(int x,int y);
        
        int getSizeX() const { return this->X; }
        int getSizeY() const { return this->Y; }
        
        bool isOffGrid(int x,int y) const { return x<0 || x>=this->X || y<0 || y>=this->Y; }
        Atom* getAt(int x,int y) const;
        
        void doTimeStep();
        int getIterations() const { return this->iterations; }
        
    private:
    
        void initAtom(int iAtom,int x,int y,char t,int s);
        
        void setAt(int x,int y,Atom* a);
        
        void doReactions();
        bool reactIfCan(Atom *a,Atom *b);
        
        void moveABlock();
        bool canMoveBlock(int sx,int sy,int ex,int ey,int dx,int dy) const;
        void moveBlock(int sx,int sy,int ex,int ey,int dx,int dy);
        
        void moveSomeIndividualAtoms();
        void moveSomeBondedAtoms();
        
        void moveTheseAtoms(const std::vector<Atom*>& movers,int dx,int dy);
        
    protected:
    
        int X,Y;
        int iterations;
        
        std::vector<Atom> atoms;
        std::vector<Reaction> reactions;
        
        std::vector<std::vector<Atom*> > grid; // grid[X][Y]
};
