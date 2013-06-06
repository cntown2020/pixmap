// Copyright (c) 2012-2013 Bartosz Szczesny. All rights reserved.
// See LICENSE file or http://bszcz.eu/license/BSD/?year=2012-2013

#define PIXMAP_ENABLE_PPM_GZ
#define PIXMAP_ENABLE_PNG
#define PIXMAP_ENABLE_JPG
#include "pixmap.c"


void PixmapPrint( struct pixmap* img );


int main() {
	struct pixmap* img = PixmapAlloc( 1600, 1200 );
	if ( NULL == img ) exit( EXIT_FAILURE ); // error message is printed by PixmapAlloc()

	double x = 0.666;
	for ( long p = 0; p < 3*img->width*img->height; p++ ) {
		x = 4 * x * ( 1.0 - x ); // random numbers from chaotic logistic map
		img->bytes[ p ] = 256 * x;
	}
	//PixmapPrint( img );

	PixmapWritePPM( img, "file.ppm" );
	PixmapWritePPM_GZ( img, "file.ppm.gz" );
	PixmapWritePNG( img, "file.png" );
	PixmapWriteJPG( img, "file.jpg" );
	img->chroma = C_422;
	img->quality = 40;
	PixmapWriteJPG( img, "file_c422_q40.jpg" );
	img->chroma = C_411;
	img->quality = 20;
	PixmapWriteJPG( img, "file_c411_q20.jpg" );
	PixmapFree( img );
	return 0;
}


void PixmapPrint( struct pixmap* img ) {
	for ( long p = 0; p < 3*img->width*img->height; p++ ) {
		printf( "bytes[ %ld ] = %u\n", p, img->bytes[ p ] );
	}
	for ( long p = 0; p < img->width*img->height; p++ ) {
		for ( long c = 0; c < 3; c++ ) {
			printf( "pixels[ %ld ][ %ld ] = %u\n", p, c, img->pixels[ p ][ c ] );
		}
	}
	for ( long h = 0; h < img->height; h++ ) {
		for ( long w = 0; w < img->width; w++ ) {
			for ( long c = 0; c < 3; c++ ) {
				printf( "pixels2[ %ld ][ %ld ][ %ld ] = %u\n", h, w, c, img->pixels2[ h ][ w ][ c ] );
			}
		}
	}
	return;
}
