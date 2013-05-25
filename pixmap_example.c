// Copyright (c) 2012-2013 Bartosz Szczesny. All rights reserved.
// See LICENSE file or http://bszcz.eu/license/BSD/?year=2012-2013

#define PIXMAP_ENABLE_PPM_GZ
#define PIXMAP_ENABLE_PNG
#include "pixmap.c"


void PixmapPrint( struct pixmap* pixmapPtr );


int main() {
	struct pixmap* pixmapPtr = PixmapAlloc( 1600, 1200 );
	if ( NULL == pixmapPtr ) exit( EXIT_FAILURE ); // error message is printed by PixmapAlloc()

	double x = 0.666;
	for ( long p = 0; p < 3*pixmapPtr->width*pixmapPtr->height; p++ ) {
		x = 4 * x * ( 1.0 - x ); // random numbers from chaotic logistic map
		pixmapPtr->bytes[ p ] = 256 * x;
	}
	//PixmapPrint( pixmapPtr );

	PixmapWritePPM( pixmapPtr, "file.ppm" );
	PixmapWritePPM_GZ( pixmapPtr, "file.ppm.gz" );
	PixmapWritePNG( pixmapPtr, "file.png" );
	PixmapFree( pixmapPtr );
	return 0;
}


void PixmapPrint( struct pixmap* pixmapPtr ) {
	for ( long p = 0; p < 3*pixmapPtr->width*pixmapPtr->height; p++ ) {
		printf( "bytes[ %ld ] = %u\n", p, pixmapPtr->bytes[ p ] );
	}
	for ( long p = 0; p < pixmapPtr->width*pixmapPtr->height; p++ ) {
		for ( long c = 0; c < 3; c++ ) {
			printf( "pixels1[ %ld ][ %ld ] = %u\n", p, c, pixmapPtr->pixels1[ p ][ c ] );
		}
	}
	for ( long h = 0; h < pixmapPtr->height; h++ ) {
		for ( long w = 0; w < pixmapPtr->width; w++ ) {
			for ( long c = 0; c < 3; c++ ) {
				printf( "pixels2[ %ld ][ %ld ][ %ld ] = %u\n", h, w, c, pixmapPtr->pixels2[ h ][ w ][ c ] );
			}
		}
	}
	return;
}
