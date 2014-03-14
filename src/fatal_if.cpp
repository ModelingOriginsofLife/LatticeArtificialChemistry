// local:
#include "fatal_if.hpp"

void fatal_if(bool test,const char* s)
{
    // useful check while debugging but turn off for maximum speed
#if 1
    if(test) 
        throw s;
#endif

}
