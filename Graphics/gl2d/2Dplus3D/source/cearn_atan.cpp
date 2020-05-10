/******************************************************************************

	Coranac.com's awesome atan2 implementation
	Very fast and very small.
	Kudos!

******************************************************************************/


#include <nds.h>
#include <math.h>

#include "cearn_atan.h"



// Get the octant a coordinate pair is in.
#define OCTANTIFY(_x, _y, _o)   do {                            \
    int _t; _o= 0;                                              \
    if(_y<  0)  {            _x= -_x;   _y= -_y; _o += 4; }     \
    if(_x<= 0)  { _t= _x;    _x=  _y;   _y= -_t; _o += 2; }     \
    if(_x<=_y)  { _t= _y-_x; _x= _x+_y; _y=  _t; _o += 1; }     \
} while(0);


static const u32 BRAD_PI_SHIFT=14,   BRAD_PI = 1<<BRAD_PI_SHIFT;
static const u32 BRAD_HPI= BRAD_PI/2, BRAD_2PI= BRAD_PI*2;

static const u32 ATAN_ONE = 0x1000, ATAN_FP= 12;


// Some constants for dealing with atanLUT.
static const u32 ATANLUT_STRIDE = ATAN_ONE / 0x80, ATANLUT_STRIDE_SHIFT= 5;


// Arctangents LUT. Interval: [0, 1] (one=128); PI=0x20000
const unsigned short atanLUT[130] ALIGN(4)=
{
    0x0000,0x0146,0x028C,0x03D2,0x0517,0x065D,0x07A2,0x08E7,
    0x0A2C,0x0B71,0x0CB5,0x0DF9,0x0F3C,0x107F,0x11C1,0x1303,
    0x1444,0x1585,0x16C5,0x1804,0x1943,0x1A80,0x1BBD,0x1CFA,
    0x1E35,0x1F6F,0x20A9,0x21E1,0x2319,0x2450,0x2585,0x26BA,
    0x27ED,0x291F,0x2A50,0x2B80,0x2CAF,0x2DDC,0x2F08,0x3033,
    0x315D,0x3285,0x33AC,0x34D2,0x35F6,0x3719,0x383A,0x395A,
    0x3A78,0x3B95,0x3CB1,0x3DCB,0x3EE4,0x3FFB,0x4110,0x4224,
    0x4336,0x4447,0x4556,0x4664,0x4770,0x487A,0x4983,0x4A8B,
// 64
    0x4B90,0x4C94,0x4D96,0x4E97,0x4F96,0x5093,0x518F,0x5289,
    0x5382,0x5478,0x556E,0x5661,0x5753,0x5843,0x5932,0x5A1E,
    0x5B0A,0x5BF3,0x5CDB,0x5DC1,0x5EA6,0x5F89,0x606A,0x614A,
    0x6228,0x6305,0x63E0,0x64B9,0x6591,0x6667,0x673B,0x680E,
    0x68E0,0x69B0,0x6A7E,0x6B4B,0x6C16,0x6CDF,0x6DA8,0x6E6E,
    0x6F33,0x6FF7,0x70B9,0x717A,0x7239,0x72F6,0x73B3,0x746D,
    0x7527,0x75DF,0x7695,0x774A,0x77FE,0x78B0,0x7961,0x7A10,
    0x7ABF,0x7B6B,0x7C17,0x7CC1,0x7D6A,0x7E11,0x7EB7,0x7F5C,
// 128
    0x8000,0x80A2
};





// QDIV stands for the fixed-point division method most appropriate for
// your system. Modify where appropriate.
// This would be for NDS.
static inline int QDIV(int num, int den, const int bits)
{
    while(REG_DIVCNT & DIV_BUSY);
    REG_DIVCNT = DIV_64_32;

    REG_DIV_NUMER = ((int64)num)<<bits;
    REG_DIV_DENOM_L = den;

    while(REG_DIVCNT & DIV_BUSY);

    return (REG_DIV_RESULT_L);
}



// Basic lookup+linear interpolation for atan2.
// Returns [0,2pi), where pi ~ 0x4000.
u32 atan2Lerp(int x, int y)
{
    if(y==0)    return (x>=0 ? 0 : BRAD_PI);

    int phi;
    u32 t, fa, fb, h;

    OCTANTIFY(x, y, phi);
    phi *= BRAD_PI/4;

    t= QDIV(y, x, ATAN_FP);
    h= t % ATANLUT_STRIDE;
    //h = mod32(t,ATANLUT_STRIDE);
	fa= atanLUT[t/ATANLUT_STRIDE  ];
    fb= atanLUT[t/ATANLUT_STRIDE+1];

    return phi + ( fa + ((fb-fa)*h >> ATANLUT_STRIDE_SHIFT) )/8;
}


// Basic lookup for atan2. Returns [0,2pi), where pi ~ 0x4000.
u32 atan2Lookup(int x, int y)
{
    if(y==0)    return (x>=0 ? 0 : BRAD_PI);

    int phi;
    u32 t;

    OCTANTIFY(x, y, phi);
    phi *= BRAD_PI/4;
    t = QDIV(y, x, ATAN_FP);

    return phi + atanLUT[t/ATANLUT_STRIDE]/8;
}


// Seed routine
int sqran(int seed)
{	
    int old= __qran_seed;
    __qran_seed= seed; 
    return old;	
}




