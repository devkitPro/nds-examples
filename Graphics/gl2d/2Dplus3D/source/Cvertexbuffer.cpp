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

#include "Cvertexbuffer.h"

/******************************************************************************


******************************************************************************/
Cvertexbuffer::Cvertexbuffer()
{
    ips_vertex = NULL;
    ips_texture = NULL;
	ips_color = NULL;
    ps_poly = NULL;

    i_max_poly = 0;
    i_primitive_type = 0;
	i_texture_ID = 0;
}

/******************************************************************************


******************************************************************************/
Cvertexbuffer::~Cvertexbuffer()
{
	
	if (ips_vertex) delete[] ips_vertex;
    ips_vertex = NULL;
    
    if (ps_poly) delete[] ps_poly;
    ps_poly = NULL;

    i_max_poly = 0;
    i_primitive_type = 0;
	i_texture_ID = 0;
	
	if (ips_texture) delete[] ips_texture;
    ips_texture = NULL;
	
	if (ips_color) delete[] ips_color;
    ips_color = NULL;

}

/******************************************************************************


******************************************************************************/
void Cvertexbuffer::render(s32 text_off_u,s32 text_off_v, bool colorize)
{
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(0, i_texture_ID);
	if (colorize)
	{
		glBegin (GL_TRIANGLES);
			//glNormal(NORMAL_PACK(0,inttov10(-1),0));
			for (int i=0; i < (i_max_poly); i++)
			{
				

				int i1 = ps_poly[i].v1;
				int i2 = ps_poly[i].v2;
				int i3 = ps_poly[i].v3;
				
				glColor3b(ips_color[i1].r,ips_color[i1].g,ips_color[i1].b);
				glTexCoord2f32 (ips_texture[i1].u +text_off_u,ips_texture[i1].v +text_off_v);
				glVertex3v16 (ips_vertex[i1].x,ips_vertex[i1].y,ips_vertex[i1].z);
				
				glColor3b(ips_color[i2].r,ips_color[i2].g,ips_color[i2].b);
				glTexCoord2f32 (ips_texture[i2].u + text_off_u,ips_texture[i2].v +text_off_v);
				glVertex3v16 (ips_vertex[i2].x,ips_vertex[i2].y,ips_vertex[i2].z);

				glColor3b(ips_color[i3].r,ips_color[i3].g,ips_color[i3].b);
				glTexCoord2f32 (ips_texture[i3].u + text_off_u,ips_texture[i3].v +text_off_v);
				glVertex3v16 (ips_vertex[i3].x,ips_vertex[i3].y,ips_vertex[i3].z);
				
			}
		glEnd(); 
	}
	else
	{
		glBegin (GL_TRIANGLES);
			//glNormal(NORMAL_PACK(0,inttov10(-1),0));
			for (int i=0; i < (i_max_poly); i++)
			{
				

				int i1 = ps_poly[i].v1;
				int i2 = ps_poly[i].v2;
				int i3 = ps_poly[i].v3;
				
				glTexCoord2f32 (ips_texture[i1].u +text_off_u,ips_texture[i1].v +text_off_v);
				glVertex3v16 (ips_vertex[i1].x,ips_vertex[i1].y,ips_vertex[i1].z);
				
				glTexCoord2f32 (ips_texture[i2].u + text_off_u,ips_texture[i2].v +text_off_v);
				glVertex3v16 (ips_vertex[i2].x,ips_vertex[i2].y,ips_vertex[i2].z);

				glTexCoord2f32 (ips_texture[i3].u + text_off_u,ips_texture[i3].v +text_off_v);
				glVertex3v16 (ips_vertex[i3].x,ips_vertex[i3].y,ips_vertex[i3].z);
				
			}
		glEnd(); 
	}
	
}



/******************************************************************************


******************************************************************************/
int Cvertexbuffer::load_texture(u8 *texture_gfx)
{

	if (!texture_gfx) return 1;
	glGenTextures(1, &i_texture_ID);
	glBindTexture(0, i_texture_ID);
	glTexImage2D( 0, 0, GL_RGB, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_TEXCOORD, texture_gfx );
	return 0;
}


/******************************************************************************


******************************************************************************/
// special functions
Cvertexbuffer *init_grid  ( int rings,    
							int bands,              
			  				float width,  
			  				float height,
							int uscale,
							int vscale)
{

   Cvertexbuffer *vb = new Cvertexbuffer;

    int max_point = (rings) * (bands);      //+1 for last band duplicate
   	Cvector3f32 *ivertex = new Cvector3f32[max_point]; 
    Ctexcoordf32 *itexture = new Ctexcoordf32[max_point];  //
	Crgbf32 *icolor = new Crgbf32[max_point]; 
    Cpolygon  *poly = new Cpolygon[max_point * 2];  //
    
    vb->ips_vertex = ivertex;
	vb->ips_texture =  itexture;
	vb->ips_color =  icolor;
    vb->ps_poly = poly;
    
    vb->i_max_vertex = max_point;
    vb->i_max_poly = max_point * 2;
    vb->i_primitive_type = GL_TRIANGLES;
    
	
	//lathing
	int s, u, slice, maxvert;
	maxvert = max_point;
    int i = 0;
	for (s = 0; s < rings; s++)
	{
 		 slice = s * bands;
		 for (u = 0; u < bands; u++)	  //duplicate texture ( not bands - 1)
		 {
			 poly[i].v1=(u+bands+1+(slice)) % maxvert;
			 poly[i].v2=(u+bands+(slice)) % maxvert;
			 poly[i].v3=(u+(slice)) % maxvert;
			 poly[i+1].v1=(u+(slice)) % maxvert;
			 poly[i+1].v2=(u+1+(slice)) % maxvert;
			 poly[i+1].v3=(u+bands+1+(slice)) % maxvert;
			 i += 2;
		 }
	}
				

	float half_width = width/2;
	float half_height = height/2;
	
	
	float a1 = 2*width/rings;
	float a2 = 2*height/bands;
	int k = 0;
	
			
	for (int i = 0; i < rings; i++)
	{
		for (int j = 0; j < bands; j++)
		{

			
			
			float x = -half_width + (i * a1);
			float y = 0;
			float z = -half_height + (j * a2);;
						
			ivertex[k].x = floattov16 (x);
			ivertex[k].y = floattov16 (y);
			ivertex[k].z = floattov16 (z);
			
			
			           
            icolor[k].r = (1.0f - (j*4 / (float)bands)) * 255;
            icolor[k].g = (1.0f - (j*4 / (float)bands)) * 255;
            icolor[k].b = (1.0f - (j*4 / (float)bands)) * 255;

			float u = ((float)i / rings ) * uscale;
			float v = ((float)j / bands ) * vscale;
			
			
			itexture[k].u = floattof32(u);
			itexture[k].v = floattof32(v);
			
			k++;
			
		}
	}
	
	return vb;

}

/******************************************************************************


******************************************************************************/
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
									float n3)
{


	
	/*
	'	a1 = abs(cos(m*theta/4)/a)^n2
    '	a2 = abs(sin(m*theta/4)/a)^n3
    '	r(theta) = (a1+a2)^(-1/n1)
    '
    '	x = r*(theta)  * cos(theta) * r*(phi)* cos(phi)
    '	y = r*(theta)  * sin(theta) * r*(phi)* cos(phi)
    '	z = r*(phi)  * sin(phi)
    '
    '	-pi/2 <= phi <= pi/2
    '	-pi <= theta <= pi
	*/
	
	
    Cvertexbuffer *vb = new Cvertexbuffer;

    int max_point = (rings) * (bands+1);      //+1 for last band duplicate
   	Cvector3f32 *ivertex = new Cvector3f32[max_point]; 
    Ctexcoordf32 *itexture = new Ctexcoordf32[max_point];  //
	Crgbf32 *icolor = new Crgbf32[max_point]; 
    Cpolygon  *poly = new Cpolygon[max_point * 2];  //
    
    vb->ips_vertex = ivertex;
	vb->ips_texture =  itexture;
	vb->ips_color =  icolor;
    vb->ps_poly = poly;
    
    vb->i_max_vertex = max_point;
    vb->i_max_poly = max_point * 2;
    vb->i_primitive_type = GL_TRIANGLES;
    
	
	//lathing
	int s, u, slice, maxvert;
	maxvert = max_point;
    int i = 0;
	for (s = 0; s < rings; s++)
	{
 		 slice = s * bands;
		 for (u = 0; u <= bands; u++)	  //duplicate texture ( not bands - 1)
		 {
			 poly[i].v1=(u+bands+1+(slice)) % maxvert;
			 poly[i].v2=(u+bands+(slice)) % maxvert;
			 poly[i].v3=(u+(slice)) % maxvert;
			 poly[i+1].v1=(u+(slice)) % maxvert;
			 poly[i+1].v2=(u+1+(slice)) % maxvert;
			 poly[i+1].v3=(u+bands+1+(slice)) % maxvert;
			 i += 2;
		 }
	}
				
	
	
	float phi, theta;
	float r1, r2, a1, a2;
	
	float Tpi_d = TWOPI/bands;
	float Ppi_d = PI/rings;
	
    phi = -PI/2;
	
	int k = 0;
	
	for (int i = 0; i < rings; i++)
	{
		
		a1 = pow(fabs(cos(m*phi/4)/a),n2);
        a2 = pow(fabs(sin(m*phi/4)/b),n3);
        r2 = pow(a1+a2,-1/n1) ;
        r2 *= radius;
        
        phi += Ppi_d;
        
        theta = -PI;
		
		for (int j = 0; j <= bands; j++)
		{

			a1 = pow(fabs(cos(m*theta/4)/a),n2);
			a2 = pow(fabs(sin(m*theta/4)/b),n3);
			r1 = pow(a1+a2,-1/n1) ;
			r1 *= radius;
			
			
			float x = r1  * cos(theta) * r2* cos(phi);
			float y = r1  * sin(theta) * r2* cos(phi);
			float z = r2  * sin(phi);
			
			theta += Tpi_d; 
			
			
						
			ivertex[k].x = floattov16 (x);
			ivertex[k].y = floattov16 (y);
			ivertex[k].z = floattov16 (z);
			
			
			           
            icolor[k].r = (1.0f - (i*4 / (float)rings)) * 255;
            icolor[k].g = (1.0f - (i*4 / (float)rings)) * 255;
            icolor[k].b = (1.0f - (i*4 / (float)rings)) * 255;

			float u = ((float)i / rings ) * uscale;
			float v = ((float)j / bands ) * vscale;
			
			
			itexture[k].u = floattof32(u);
			itexture[k].v = floattof32(v);
			
			k++;
			
		}
	}
	
	return vb;
}

/******************************************************************************


******************************************************************************/
// initialize out tunnel and
// store float values to f32 classes
Cvertexbuffer *init_ascaris ( int rings,    	  	  // rings
			  				  int bands,              // bands
			  				  float radius,			  // radius of each ring from center
			  				  float center_offset,	  // swirl offset
							  int uscale,
							  int vscale)
{

    Cvertexbuffer *vb = new Cvertexbuffer;

    int max_point = (rings) * (bands + 1);      //+1 for last band duplicate
   	Cvector3f32 *ivertex = new Cvector3f32[max_point]; 
    Ctexcoordf32 *itexture = new Ctexcoordf32[max_point];  //
	Crgbf32 *icolor = new Crgbf32[max_point]; 
    Cpolygon  *poly = new Cpolygon[max_point * 2];  //
    
    vb->ips_vertex = ivertex;
	vb->ips_texture =  itexture;
	vb->ips_color =  icolor;
    vb->ps_poly = poly;
    
    vb->i_max_vertex = max_point;
    vb->i_max_poly = max_point * 2;
    vb->i_primitive_type = GL_TRIANGLES;
    
	
	//lathing
	int s, u, slice, maxvert;
	maxvert = max_point;
    int i = 0;
	for (s = 0; s < (rings); s++)
	{
		 slice = s * bands;
		 for (u = 0; u < bands; u++)	  //duplicate texture ( not bands - 1)
		 {
			 poly[i].v1=(u+bands+1+(slice)) % maxvert;
			 poly[i].v2=(u+bands+(slice)) % maxvert;
			 poly[i].v3=(u+(slice)) % maxvert;
			 poly[i+1].v1=(u+(slice)) % maxvert;
			 poly[i+1].v2=(u+1+(slice)) % maxvert;
			 poly[i+1].v3=(u+bands+1+(slice)) % maxvert;
			 i += 2;
		 }
	}


	int k = 0;
	float xc, yc, zc;
	for (int i = 0 ; i < rings; i++)
	{
		zc = i - (i/20);
	    xc = cos(TWOPI * zc / rings)* center_offset;
      	yc = sin(TWOPI * zc / rings)* center_offset;
		for (int j = 0; j <= bands; j++)
		{

			float x = xc + cos(j * TWOPI / bands ) * radius;
			float y = yc + sin(j * TWOPI / bands ) * radius;
			float z = 0-(i*2);
						
			ivertex[k].x = floattov16 (x);
			ivertex[k].y = floattov16 (y);
			ivertex[k].z = floattov16 (z);
			
			
			           
            icolor[k].r = (1.0f - (i*4 / (float)rings)) * 255;
            icolor[k].g = (1.0f - (i*4 / (float)rings)) * 255;
            icolor[k].b = (1.0f - (i*4 / (float)rings)) * 255;

			float u = ((float)i / rings ) * uscale;
			float v = ((float)j / bands ) * vscale;
			
			
			itexture[k].u = floattof32(u);
			itexture[k].v = floattof32(v);
			
			k++;
		}
	}
	
	return vb;
}

