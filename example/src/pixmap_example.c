// Copyright (c) 2012-2013 Bartosz Szczesny. All rights reserved.
// See LICENSE file or http://bszcz.eu/license/BSD/?year=2012-2013

#include <github.com/bszcz/pixmap/pixmap.h>
#include <github.com/bszcz/pixmap/pixmap_ppm.h>
#include <github.com/bszcz/pixmap/pixmap_ppm_gz.h>
#include <github.com/bszcz/pixmap/pixmap_png.h>
#include <github.com/bszcz/pixmap/pixmap_jpg.h>

void PixmapPrint( struct pixmap* img );

int main() {
	struct pixmap* img = PixmapAlloc( 1600, 1200 );
	if ( NULL == img ) exit( EXIT_FAILURE ); // error message is printed by PixmapAlloc()

	double x = 0.666;
	for ( long p = 0; p < PIXMAP_COLORS*img->width*img->height; p++ ) {
		x = 4 * x * ( 1.0 - x ); // random numbers from chaotic logistic map
		img->bytes[ p ] = 256 * x;
	}
	//PixmapPrint( img );

	PixmapWritePPM( img, "pixmap.ppm" );
	PixmapWritePPM_GZ( img, "pixmap.ppm.gz" );
	PixmapWritePNG( img, "pixmap.png" );
	PixmapWriteJPG( img, "pixmap.jpg" );

	img->chroma = PIXMAP_CHROMA_422;
	img->quality = 40;
	PixmapWriteJPG( img, "pixmap_c422_q40.jpg" );
	img->chroma = PIXMAP_CHROMA_411;
	img->quality = 20;
	PixmapWriteJPG( img, "pixmap_c411_q20.jpg" );

	PixmapFree( img );
	return 0;
}

void PixmapPrint( struct pixmap* img ) {
	for ( long p = 0; p < PIXMAP_COLORS*img->width*img->height; p++ ) {
		printf( "bytes[ %ld ] = %u\n", p, img->bytes[ p ] );
	}
	for ( long p = 0; p < img->width*img->height; p++ ) {
		for ( long c = 0; c < PIXMAP_COLORS; c++ ) {
			printf( "pixels[ %ld ][ %ld ] = %u\n", p, c, img->pixels[ p ][ c ] );
		}
	}
	for ( long h = 0; h < img->height; h++ ) {
		for ( long w = 0; w < img->width; w++ ) {
			for ( long c = 0; c < PIXMAP_COLORS; c++ ) {
				printf( "pixels2[ %ld ][ %ld ][ %ld ] = %u\n", h, w, c, img->pixels2[ h ][ w ][ c ] );
			}
		}
	}
	return;
}
