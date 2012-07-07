//	flib - A TrueType font library for the PSP
//
//	Based on the freetype2 tutorials and example code
//
//	Copyright 2006 samedi1971@yahoo.com
//
//	Requirements:	PSP graphics library by Psilocybeing
//				libpng for PSP
//				libfreetype2 for PSP


#include <pspkernel.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "glib2d.h"
#include "glibf.h"


// private
g2dColor font_color = 0xffffffff;
double font_angle = 0.0;
FT_Library library = 0;
FT_Face face = 0;
gfFont *current_font = NULL;

//private
void gfRenderFont(char *text, g2dTexture *image, int x, int y);
void gfDrawBitmap(g2dTexture *image, FT_Bitmap *bitmap, FT_Int x, FT_Int y, int width, int height);


void gfSetFont(gfFont *font) {
	current_font = font;	
}

void gfSetFontColor(g2dColor color) {
	font_color = color;
}


void gfSetFontAngle(double angle) {
	if(angle >= 360.0 || angle <= -360.0)
		font_angle = 0.0;
	else if(angle < 0.0)
		font_angle = 360.0 - angle;
	else
		font_angle = angle;
}


int gfSetFontSize(int size) {
	FT_Error error;
	if(!face)
		return GLIBF_ERR_INVALIDFONT;
	error = FT_Set_Char_Size( face, size * 64, 0, 100, 0 );
	if(error)
		return GLIBF_ERR_BADSIZE;
	return 0;
}


// Note:	gfLoadFont() returns a void pointer which will be defined in
//		a future flib version when support for multiple fonts is added.
//		At that time a set_font() function will be added to switch the
//		current font face used in all other functions.  For now just
//		treat the return value as NULL on failure and non-NULL on
//		success.
gfFont gfLoadFont(char *filename) {
	FT_Error error;
	error = FT_Init_FreeType( &library );              /* initialize library */
  	if(error)
		return 0;
  	error = FT_New_Face( library, filename, 0, &face );
	if(error)
		return 0;
	FT_Set_Char_Size( face, 14 * 64, 0, 100, 0 );	// set default 14 point size
	return (gfFont) 1;
}


void gfUnloadFont() {
	FT_Done_Face(face);
	FT_Done_FreeType(library);
	face = 0;
	library = 0;
}


void gfTextToScreen(char *text, int x, int y) {
	g2dBeginPoints();
	gfRenderFont(text, 0, x, y);
	g2dEnd();
}


void gfTextToImage(char *text, g2dTexture *image, int x, int y) {
	return; // Not implemented
	gfRenderFont(text, image, x, y);
}


void gfRenderFont(char *text, g2dTexture *image, int x, int y) {
	FT_GlyphSlot slot;
	FT_Matrix matrix;
	FT_Vector pen;
	FT_Error error;
	double angle;
	int target_height = 272;
	int n, num_chars;

	if(!text || !*text)
		return;
	if(image)
		target_height = image->th;

	slot = face->glyph;
	num_chars = strlen(text);
	angle = ( font_angle / 360 ) * 3.14159 * 2;
	matrix.xx = (FT_Fixed)(cos(angle) * 0x10000L);
	matrix.xy = (FT_Fixed)(-sin(angle) * 0x10000L);
	matrix.yx = (FT_Fixed)(sin(angle) * 0x10000L);
	matrix.yy = (FT_Fixed)(cos(angle) * 0x10000L);

	pen.x = x * 64;
	pen.y = (target_height - y) * 64;
	for(n = 0; n < num_chars; n++) {
		FT_Set_Transform(face, &matrix, &pen);
		error = FT_Load_Char( face, text[n], FT_LOAD_RENDER );
		if(error)
			continue;                 /* ignore errors */
		if(image)
			gfDrawBitmap(image, &slot->bitmap, slot->bitmap_left,
				target_height - slot->bitmap_top, image->w,
				image->h);
		else // to screen
			gfDrawBitmap(0, &slot->bitmap, slot->bitmap_left,
				target_height - slot->bitmap_top, 480,
				272);
		pen.x += slot->advance.x;
		pen.y += slot->advance.y;
	}
}


void gfDrawBitmap(g2dTexture *image, FT_Bitmap *bitmap, FT_Int x, FT_Int y, int width, int height) {
	FT_Int i, j, p, q;
	FT_Int x_max = x + bitmap->width;
	FT_Int y_max = y + bitmap->rows;
	g2dColor pixel, grey;
	int r, g, b;

	for(i = x, p = 0; i < x_max; i++, p++) {
		for(j = y, q = 0; j < y_max; j++, q++) {
			if(i >= width || i < 0 || j >= height || j < 0)
				continue;
			grey = bitmap->buffer[q * bitmap->width + p];
			if (grey > 0) {
				r = (grey * G2D_GET_R(font_color)) / 255;
				g = (grey * G2D_GET_G(font_color)) / 255;
				b = (grey * G2D_GET_B(font_color)) / 255;
	  			pixel = 0xff000000 | (b << 16) | (g << 8) | r;
			} else {
	  			pixel = 0;
			}
			
			if (pixel) {
				if (image) {
					// putPixelg2dTexture(pixel, i, j, image);
				} else {
					g2dSetCoordXY(i, j);
					g2dSetColor(pixel);
					g2dAdd();
				}
                      
			} 
		}
	}

}

