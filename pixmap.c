// Copyright (c) 2012-2013 Bartosz Szczesny
// LICENSE: The MIT License (MIT)

#include <github.com/bszcz/pixmap/pixmap.h>

void* PixmapFree( struct pixmap* img ) {
	if ( NULL == img ) {
		fprintf( stderr, "PixmapWarning: pixmap pointer == NULL\n" );
	} else {
		if ( NULL != img->pixels2 ) {
			for ( long h = 0; h < img->height; h++ ) {
				free( img->pixels2[ h ] );
			}
		}
		free( img->pixels2 );
		free( img->pixels );
		free( img->bytes );
		free( img );
	}
	return NULL; // img = PixmapFree( img );
}

void* PixmapAllocError( struct pixmap* img ) {
	fprintf( stderr, "PixmapError: cannot allocate memory\n" );
	PixmapFree( img );
	return NULL;
}

struct pixmap* PixmapAlloc( long width, long height ) {
	if ( width <= 0 || height <= 0 ) {
		fprintf( stderr, "PixmapError: width = %ld, height = %ld\n", width, height );
		return NULL;
	}

	// allocate memory
	struct pixmap* img = calloc( 1, sizeof( struct pixmap ) );
	if ( NULL == img ) return PixmapAllocError( img );
	img->width = width;
	img->height = height;
	img->chroma = PIXMAP_GOOD_CHROMA;
	img->quality = PIXMAP_GOOD_QUALITY;
	img->pixels = NULL; // for clean freeing
	img->pixels2 = NULL; // for clean freeing

	img->bytes = calloc( PIXMAP_COLORS*width*height, sizeof( unsigned char ) );
	if ( NULL == img->bytes ) return PixmapAllocError( img );
	img->pixels = calloc( width*height, sizeof( unsigned char* ) );
	if ( NULL == img->pixels ) return PixmapAllocError( img );
	img->pixels2 = calloc( height, sizeof( unsigned char** ) );
	if ( NULL == img->pixels2 ) return PixmapAllocError( img );

	for ( long h = 0; h < height; h++ ) img->pixels2[ h ] = NULL; // for clean freeing
	for ( long h = 0; h < height; h++ ) {
		img->pixels2[ h ] = calloc( width, sizeof( unsigned char* ) );
		if ( NULL == img->pixels2[ h ] ) return PixmapAllocError( img );
	}

	// calculate pointers
	for ( long p = 0; p < width*height; p++ ) {
		img->pixels[ p ] = img->bytes + PIXMAP_COLORS*p;
	}
	for ( long h = 0; h < height; h++ ) {
		for ( long w = 0; w < width; w++ ) {
			img->pixels2[ h ][ w ] = img->bytes + PIXMAP_COLORS*h*width + PIXMAP_COLORS*w;
		}
	}

	return img;
} // PixmapAlloc(  )
