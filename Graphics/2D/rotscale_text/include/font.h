#ifndef _font_h_
#define _font_h_

//byte order: big endian
//tiling grid: 8x8

//dimension data
#define font_WIDTH	8
#define font_HEIGHT	768

//byte array representing the picture
extern const u16 fontData[];
extern const u16 fontPalette[];
extern const u32 fontDataSize;
extern const u32 fontPaletteSize;

#endif // _font_h_
