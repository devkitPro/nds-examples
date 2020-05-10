/******************************************************************************
*******************************************************************************

	Space Impakto DS
	relminator
	http://rel.betterwebber.com


	Cvertexbuffer class


*******************************************************************************
******************************************************************************/ 
#include <nds.h>
#include <math.h>


#ifndef Cvertexbuffer__H
#define Cvertexbuffer__H

static const float PI = 3.141593f;
static const float TWOPI = PI * 2;


// .12 FP vector
class Cvector3f32
{
	public:
	s32 x;
	s32 y;
	s32 z;
};

class Crgbf32
{
	public:
	s32 r;
	s32 g;
	s32 b;
};



class Ctexcoordf32
{
    public:
    	s32 u;
		s32 v;
};

class Cpolygon
{
    public:
    	u32 v1;
		u32 v2;
		u32 v3;

};

// vertex buffer object. LOL
class Cvertexbuffer
{
    public:
        Cvertexbuffer();
	    ~Cvertexbuffer();
		void render(s32 text_off_u,s32 text_off_v, bool colorize);
		void render_lines(u8 r, u8 g, u8 b);
	    int load_texture(u8 *texture_gfx);
		int i_max_poly;
	    int i_max_vertex;
	    int i_primitive_type;
		int i_texture_ID;
		Cvector3f32 *ips_vertex;
		Ctexcoordf32 *ips_texture;
		Crgbf32 *ips_color;
	    Cpolygon *ps_poly;	    	    
};

Cvertexbuffer *init_grid  ( int rings,    
							int bands,              
			  				float width,  
			  				float height,
							int uscale,
							int vscale);
							


Cvertexbuffer *init_super_shape (	int rings,    	  	  // rings
									int bands,            // bands
									float radius,		  // radius 
									int uscale,
									int vscale,
									float a,
									float b,
									float m,
									float n1,
									float n2,
									float n3);



Cvertexbuffer *init_ascaris ( int rings,    	  	  // rings
			  				  int bands,              // bands
			  				  float radius,			  // radius of each ring from center
			  				  float center_offset,	  // swirl offset
							  int uscale,
							  int vscale);
							  

#endif

