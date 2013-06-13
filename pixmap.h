// Copyright (c) 2012-2013 Bartosz Szczesny. All rights reserved.
// See LICENSE file or http://bszcz.eu/license/BSD/?year=2012-2013

#ifndef __PIXMAP_H__
#define __PIXMAP_H__

#include <stdio.h>
#include <stdlib.h>

// chroma sampling
enum {
	 PIXMAP_CHROMA_411 = 1,		// 4:1:1
	 PIXMAP_CHROMA_422 = 2,		// 4:2:2
	 PIXMAP_CHROMA_444 = 4		// 4:4:4
};

// colour channels
enum {
	 PIXMAP_RED = 0,	// red
	 PIXMAP_GREEN = 1,	// green
	 PIXMAP_BLUE = 2,	// blue
	 PIXMAP_COLORS = 3
};

// good values for jpg compression
enum {
	PIXMAP_GOOD_CHROMA = PIXMAP_CHROMA_444,
	PIXMAP_GOOD_QUALITY = 80
};

struct pixmap {
	long width;
	long height;
	int chroma;	// jpeg only
	int quality;	// jpeg only
	unsigned char*		bytes;		// access by: bytes[0..PIXMAP_COLORS*width*height] ( RGBRGB... in row major order )
	unsigned char**		pixels;		// access by: pixels[0..width*height][PIXMAP_RED..PIXMAP_BLUE]
	unsigned char*** 	pixels2;	// access by: pixels2[0..width][0..height][PIXMAP_RED..PIXMAP_BLUE]
};

void* PixmapFree( struct pixmap* img );

void* PixmapAllocError( struct pixmap* img );

struct pixmap* PixmapAlloc( long width, long height );

#endif
