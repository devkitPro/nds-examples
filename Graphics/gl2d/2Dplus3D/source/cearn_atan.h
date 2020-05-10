/*
coranac.com's awesome atan2 implementation
very fast and very small.
Kudos!

*/

#include <nds.h>


#ifndef cearn_atan__H
#define cearn_atan__H


static int __qran_seed= 42;


u32 atan2Lookup(int x, int y);
u32 atan2Lerp(int x, int y);

int sqran(int seed);

//! Quick (and very dirty) pseudo-random number generator 
/*! \return random in range [0,8000h>
*/
inline int qran()
{	
    __qran_seed= 1664525*__qran_seed+1013904223;
    return (__qran_seed>>16) & 0x7FFF;
}

//! Ranged random number
/*! \return random in range [\a min, \a max>
*   \note (max-min) must be lower than 8000h
*/
inline int qran_range(int min, int max)	
{    
	return (qran()*(max-min)>>15)+min;
}

#endif

