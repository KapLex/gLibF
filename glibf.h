//	flib.c - A TrueType font library for the PSP
//
//	Based on the freetype2 tutorials and example code
//
//	Copyright 2006 samedi1971@yahoo.com
//
//	Requirements:	PSP graphics library by Psilocybeing
//				libpng for PSP
//				libfreetype2 for PSP


#ifndef GLIBF_H_
#define GLIBF_H_



// error codes
#define GLIBF_ERR_CANTINIT	1	// problem initializing freetype 2
#define GLIBF_ERR_CANTOPEN	2	// problem finding or loading .ttf file
#define GLIBF_ERR_MEMORY	3	// ran out of memory
#define GLIBF_ERR_INVALIDFONT	4	// attempt to use bad/empty font
#define GLIBF_ERR_BADSIZE	5	// requested size out of range


// definitions
typedef struct {
  FT_LIBRARY library;
  FT_FACE face;
} gfFont;

// functions
void gfSetFont(gfFont *font);
void gfSetFontColor(g2dColor color);
void gfSetFontAngle(double angle);
int gfSetFontSize(int size);
gfFont* gfLoadFont(char *filename);
void gfUnloadFont();
void gfTextToScreen(char *text, int x, int y);
void gfTextToImage(char *text, g2dTexture *image, int x, int y);



#endif  // GLIBF_H_
