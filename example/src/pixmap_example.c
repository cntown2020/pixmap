// Copyright (c) 2012-2013 Bartosz Szczesny
// LICENSE: The MIT License (MIT)

#include <github.com/bszcz/pixmap/pixmap_ppm.h>
#include <github.com/bszcz/pixmap/pixmap_ppm_gz.h>
#include <github.com/bszcz/pixmap/pixmap_png.h>
#include <github.com/bszcz/pixmap/pixmap_jpg.h>

void pixmap_print( struct pixmap* img );

int main() {
	struct pixmap* img = pixmap_alloc( 1600, 1200 );
	if ( NULL == img ) exit( EXIT_FAILURE ); // error message is printed by pixmap_alloc()

	double x = 0.666;
	for ( long p = 0; p < PIXMAP_COLORS*img->width*img->height; p++ ) {
		x = 4 * x * ( 1.0 - x ); // random numbers from chaotic logistic map
		img->bytes[ p ] = 256 * x;
	}
	//pixmap_print( img );

	pixmap_write_ppm( img, "pixmap.ppm" );
	pixmap_write_ppm_gz( img, "pixmap.ppm.gz" );
	pixmap_write_png( img, "pixmap.png" );
	pixmap_write_jpg( img, "pixmap.jpg" );

	img->chroma = PIXMAP_CHROMA_422;
	img->quality = 40;
	pixmap_write_jpg( img, "pixmap_c422_q40.jpg" );
	img->chroma = PIXMAP_CHROMA_411;
	img->quality = 20;
	pixmap_write_jpg( img, "pixmap_c411_q20.jpg" );

	pixmap_free( img );
	return 0;
}

void pixmap_print( struct pixmap* img ) {
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
