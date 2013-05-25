// Copyright (c) 2012-2013 Bartosz Szczesny. All rights reserved.
// See LICENSE file or http://bszcz.eu/license/BSD/?year=2012-2013

/*
define these before including this file to enable additional file formats:
#define PIXMAP_ENABLE_PPM_GZ	// enables .ppm.gz format, requires '-lz'   compilation flag
#define PIXMAP_ENABLE_PNG	// enables .png    format, requires '-lpng' compilation flag
*/


#ifndef __PIXMAP_C__
#define __PIXMAP_C__

#include <stdlib.h>
#include <stdio.h>

#ifdef PIXMAP_ENABLE_PPM_GZ
#include <zlib.h>
#endif

#ifdef PIXMAP_ENABLE_PNG
#include <png.h>
#endif

// Colours/Channels
#define C_R	0	// Red
#define C_G	1	// Green
#define C_B	2	// Blue


struct pixmap {
	long width;
	long height;
	unsigned char*		bytes;		// access by: bytes[0..3*width*height] ( RGBRGB... in row major order )
	unsigned char**		pixels1;	// access by: pixels1[0..width*height][C_R..C_B]
	unsigned char*** 	pixels2;	// access by: pixels2[0..width][0..height][C_R..C_B]
};


void* PixmapFree( struct pixmap* pixmapPtr ) {
	if ( NULL == pixmapPtr ) {
		fprintf( stderr, "PixmapWaring: pixmap pointer == NULL\n" );
	} else {
		if ( NULL != pixmapPtr->pixels2 ) {
			for ( long h = 0; h < pixmapPtr->height; h++ ) {
				free( pixmapPtr->pixels2[ h ] );
			}
		}
		free( pixmapPtr->pixels2 );
		free( pixmapPtr->pixels1 );
		free( pixmapPtr->bytes );
		free( pixmapPtr );
	}
	return NULL; // pixmapPtr = PixmapFree( pixmapPtr );
}


void* PixmapAllocError( struct pixmap* pixmapPtr ) {
	fprintf( stderr, "PixmapError: cannot allocate memory\n" );
	PixmapFree( pixmapPtr );
	return NULL;
}


struct pixmap*
PixmapAlloc( long width, long height ) {
	if ( width <= 0 || height <= 0 ) {
		fprintf( stderr, "PixmapError: width = %ld, height = %ld\n", width, height );
		return NULL;
	}

	// allocate memory
	struct pixmap* pixmapPtr = calloc( 1, sizeof( struct pixmap ) );
	if ( NULL == pixmapPtr ) return PixmapAllocError( pixmapPtr );
	pixmapPtr->width = width;
	pixmapPtr->height = height;
	pixmapPtr->pixels1 = NULL; // for clean freeing
	pixmapPtr->pixels2 = NULL; // for clean freeing

	pixmapPtr->bytes = calloc( 3*width*height, sizeof( unsigned char ) );
	if ( NULL == pixmapPtr->bytes ) return PixmapAllocError( pixmapPtr );
	pixmapPtr->pixels1 = calloc( width*height, sizeof( unsigned char* ) );
	if ( NULL == pixmapPtr->pixels1 ) return PixmapAllocError( pixmapPtr );
	pixmapPtr->pixels2 = calloc( height, sizeof( unsigned char** ) );
	if ( NULL == pixmapPtr->pixels2 ) return PixmapAllocError( pixmapPtr );

	for ( long h = 0; h < height; h++ ) pixmapPtr->pixels2[ h ] = NULL; // for clean freeing
	for ( long h = 0; h < height; h++ ) {
		pixmapPtr->pixels2[ h ] = calloc( width, sizeof( unsigned char* ) );
		if ( NULL == pixmapPtr->pixels2[ h ] ) return PixmapAllocError( pixmapPtr );
	}

	// calculate pointers
	for ( long p = 0; p < width*height; p++ ) {
		pixmapPtr->pixels1[ p ] = pixmapPtr->bytes + 3*p;
	}
	for ( long h = 0; h < height; h++ ) {
		for ( long w = 0; w < width; w++ ) {
			pixmapPtr->pixels2[ h ][ w ] = pixmapPtr->bytes + 3*h*width + 3*w;
		}
	}

	return pixmapPtr;
} // PixmapAlloc(  )


int PixmapWritePPM( struct pixmap* pixmapPtr, char* fileName ) {
	int err = 0;
	FILE* filePtr = fopen( fileName, "wb" );
	if ( NULL == filePtr ) {
		fprintf( stderr, "PixmapError: cannot open PPM file '%s'\n", fileName );
		return 1;
	}
	if ( 0 > fprintf( filePtr, "P6\n%ld %ld\n255\n", pixmapPtr->width, pixmapPtr->height ) ) {
		fprintf( stderr, "PixmapError: cannot write PPM header\n" );
		err = 1;
		goto fcloseFile;
	}
	const size_t dataSize = 3*pixmapPtr->width*pixmapPtr->height;
	if ( dataSize != fwrite( pixmapPtr->bytes, sizeof( unsigned char ), dataSize, filePtr ) ) {
		fprintf( stderr, "PixmapError: cannot write PPM data\n" );
		err = 1;
		goto fcloseFile;
	}

	fcloseFile:
		if ( 0 != fclose( filePtr ) ) {
			fprintf( stderr, "PixmapError: cannot close PPM file '%s'\n", fileName );
			err = 1;
		}

	return err;
} // PixmapWritePPM(  )


#ifdef PIXMAP_ENABLE_PPM_GZ
int PixmapWritePPM_GZ( struct pixmap* pixmapPtr, char* fileName ) {
	int err = 0;
	gzFile filePtr = gzopen( fileName, "wb9" );	// '9' means maximum compression level
	if ( NULL == filePtr ) {
		fprintf( stderr, "PixmapError: cannot open PPM.GZ file '%s'\n", fileName );
		return 1;
	}
	if ( 0 == gzprintf( filePtr, "P6\n%ld %ld\n255\n", pixmapPtr->width, pixmapPtr->height ) ) {
		fprintf( stderr, "PixmapError: cannot write PPM.GZ header\n" );
		err = 1;
		goto gzcloseFile;
	}
	const long dataSize = 3*pixmapPtr->width*pixmapPtr->height;
	if ( dataSize != gzwrite( filePtr, pixmapPtr->bytes, dataSize*sizeof( unsigned char ) ) ) {
		fprintf( stderr, "PixmapError: cannot write PPM.GZ data\n" );
		err = 1;
		goto gzcloseFile;
	}

	gzcloseFile:
		if ( Z_OK != gzclose( filePtr ) ) {
			fprintf( stderr, "PixmapError: cannot close PPM.GZ file '%s'\n", fileName );
			err = 1;
		}

	return err;
} // PixmapWritePPM_GZ(  )
#endif // #ifdef PIXMAP_ENABLE_PPM_GZ


#ifdef PIXMAP_ENABLE_PNG
int PixmapWritePNG( struct pixmap* pixmapPtr, char* fileName ) {
	int err = 0;
	FILE* filePtr = fopen( fileName, "wb" );
	if ( NULL == filePtr ) {
		fprintf( stderr, "PixmapError: cannot open PNG file '%s'\n", fileName );
		return 1;
	}
	png_structp pngPtr = png_create_write_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
	if ( NULL == pngPtr ) {
		fprintf( stderr, "PixmapError: cannot create PNG write struct" );
		err = 1;
		goto fcloseFile;
	}
	png_infop infoPtr = png_create_info_struct( pngPtr );
	if ( NULL == infoPtr ) {
		fprintf( stderr, "PixmapError: cannot create PNG info struct" );
		err = 1;
		goto pngDestroy;
	}
	png_bytepp rowsPtr = calloc( pixmapPtr->height, sizeof( png_bytep ) );
	if ( NULL == rowsPtr ) {
		fprintf( stderr, "PixmapError: cannot allocate memory\n" );
		err = 1;
		goto pngDestroy;
	}
	for ( long h = 0; h < pixmapPtr->height; h++ ) {
		rowsPtr[ h ] = pixmapPtr->bytes + 3*h*pixmapPtr->width;
	}

	const long depth = 8;
	png_init_io( pngPtr, filePtr );
	png_set_IHDR( pngPtr, infoPtr, pixmapPtr->width, pixmapPtr->height, depth,
		PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE );
	png_write_info( pngPtr, infoPtr );
	png_write_rows( pngPtr, rowsPtr, pixmapPtr->height );
	png_write_end( pngPtr, infoPtr );
	free( rowsPtr );

	pngDestroy:
		png_destroy_write_struct( &pngPtr, &infoPtr );
	fcloseFile:
		if ( 0 != fclose( filePtr ) ) {
			fprintf( stderr, "PixmapError: cannot close PNG file '%s'\n", fileName );
			err = 1;
		}

	return err;
} // PixmapWritePNG(  )
#endif // #ifdef PIXMAP_ENABLE_PNG


#endif // __PIXMAP_C__
