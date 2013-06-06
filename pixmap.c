// Copyright (c) 2012-2013 Bartosz Szczesny. All rights reserved.
// See LICENSE file or http://bszcz.eu/license/BSD/?year=2012-2013

/*
define these before including this file to enable additional file formats:
#define PIXMAP_ENABLE_PPM_GZ	// enables .ppm.gz format, requires '-lz'    compilation flag
#define PIXMAP_ENABLE_PNG	// enables .png    format, requires '-lpng'  compilation flag
#define PIXMAP_ENABLE_JPG	// enables .jpg    format, requires '-ljpeg' compilation flag
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

#ifdef PIXMAP_ENABLE_JPG
#include <jpeglib.h>
#endif

// Chroma Sampling
#define C_411	1	// 4:1:1
#define C_422	2	// 4:2:2
#define C_444	4	// 4:4:4

// Colour Channels
#define C_R	0	// Red
#define C_G	1	// Green
#define C_B	2	// Blue

// default values
static const int good_chroma = C_444;
static const int good_quality = 80;

struct pixmap {
	long width;
	long height;
	int chroma;	// jpeg only
	int quality;	// jpeg only
	unsigned char*		bytes;		// access by: bytes[0..3*width*height] ( RGBRGB... in row major order )
	unsigned char**		pixels;	// access by: pixels[0..width*height][C_R..C_B]
	unsigned char*** 	pixels2;	// access by: pixels2[0..width][0..height][C_R..C_B]
};

void* PixmapFree( struct pixmap* img ) {
	if ( NULL == img ) {
		fprintf( stderr, "PixmapWaring: pixmap pointer == NULL\n" );
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
	img->chroma = good_chroma;
	img->quality = good_quality;
	img->pixels = NULL; // for clean freeing
	img->pixels2 = NULL; // for clean freeing

	img->bytes = calloc( 3*width*height, sizeof( unsigned char ) );
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
		img->pixels[ p ] = img->bytes + 3*p;
	}
	for ( long h = 0; h < height; h++ ) {
		for ( long w = 0; w < width; w++ ) {
			img->pixels2[ h ][ w ] = img->bytes + 3*h*width + 3*w;
		}
	}

	return img;
} // PixmapAlloc(  )

int PixmapWritePPM( struct pixmap* img, char* fileName ) {
	int err = 0;

	FILE* file = fopen( fileName, "wb" );
	if ( NULL == file ) {
		fprintf( stderr, "PixmapError: cannot open PPM file '%s'\n", fileName );
		return 1;
	}
	if ( 0 > fprintf( file, "P6\n%ld %ld\n255\n", img->width, img->height ) ) {
		fprintf( stderr, "PixmapError: cannot write PPM header\n" );
		err = 1;
		goto fclosePPM;
	}
	const size_t data_size = 3*img->width*img->height;
	if ( data_size != fwrite( img->bytes, sizeof( unsigned char ), data_size, file ) ) {
		fprintf( stderr, "PixmapError: cannot write PPM data\n" );
		err = 1;
	}

	fclosePPM:
		if ( 0 != fclose( file ) ) {
			fprintf( stderr, "PixmapError: cannot close PPM file '%s'\n", fileName );
			err = 1;
		}
	return err;
} // PixmapWritePPM(  )

#ifdef PIXMAP_ENABLE_PPM_GZ
int PixmapWritePPM_GZ( const struct pixmap* img, const char* fileName ) {
	int err = 0;

	gzFile file = gzopen( fileName, "wb9" );	// '9' means maximum compression level
	if ( NULL == file ) {
		fprintf( stderr, "PixmapError: cannot open PPM.GZ file '%s'\n", fileName );
		return 1;
	}
	if ( 0 == gzprintf( file, "P6\n%ld %ld\n255\n", img->width, img->height ) ) {
		fprintf( stderr, "PixmapError: cannot write PPM.GZ header\n" );
		err = 1;
		goto gzclosePPM;
	}
	const long data_size = 3*img->width*img->height;
	if ( data_size != gzwrite( file, img->bytes, data_size*sizeof( unsigned char ) ) ) {
		fprintf( stderr, "PixmapError: cannot write PPM.GZ data\n" );
		err = 1;
		goto gzclosePPM;
	}

	gzclosePPM:
		if ( Z_OK != gzclose( file ) ) {
			fprintf( stderr, "PixmapError: cannot close PPM.GZ file '%s'\n", fileName );
			err = 1;
		}
	return err;
} // PixmapWritePPM_GZ(  )
#endif // #ifdef PIXMAP_ENABLE_PPM_GZ

#ifdef PIXMAP_ENABLE_PNG
int PixmapWritePNG( const struct pixmap* img, const char* fileName ) {
	int err = 0;

	FILE* file = fopen( fileName, "wb" );
	if ( NULL == file ) {
		fprintf( stderr, "PixmapError: cannot open PNG file '%s'\n", fileName );
		return 1;
	}
	png_structp png = png_create_write_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
	if ( NULL == png ) {
		fprintf( stderr, "PixmapError: cannot create PNG write struct" );
		err = 1;
		goto fclosePNG;
	}
	png_infop info = png_create_info_struct( png );
	if ( NULL == info ) {
		fprintf( stderr, "PixmapError: cannot create PNG info struct" );
		err = 1;
		goto destroyPNG;
	}
	png_bytepp rows = calloc( img->height, sizeof( png_bytep ) );
	if ( NULL == rows ) {
		fprintf( stderr, "PixmapError: cannot allocate memory\n" );
		err = 1;
		goto destroyPNG;
	}
	for ( long h = 0; h < img->height; h++ ) {
		rows[ h ] = img->bytes + 3*h*img->width;
	}

	const long depth = 8;
	png_init_io( png, file );
	png_set_IHDR( png, info, img->width, img->height, depth,
		PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE );
	png_write_info( png, info );
	png_write_rows( png, rows, img->height );
	png_write_end( png, info );
	free( rows );

	destroyPNG:
		png_destroy_write_struct( &png, &info );
	fclosePNG:
		if ( 0 != fclose( file ) ) {
			fprintf( stderr, "PixmapError: cannot close PNG file '%s'\n", fileName );
			err = 1;
		}
	return err;
} // PixmapWritePNG(  )
#endif // #ifdef PIXMAP_ENABLE_PNG

#ifdef PIXMAP_ENABLE_JPG
int PixmapWriteJPG( const struct pixmap* img, const char* fileName ) {
	int err = 0;

	FILE* file = fopen( fileName, "wb" );
	if ( NULL == file ) {
		fprintf( stderr, "PixmapError: cannot open JPG file '%s'\n", fileName );
		return 1;
	}

	j_compress_ptr jpg = calloc( 1, sizeof( struct jpeg_compress_struct ) );
	if ( NULL == jpg ) {
		fprintf( stderr, "PixmapError: cannot create JPG compress struct" );
		err = 1;
		goto fcloseJPG;
	}
	struct jpeg_error_mgr jerr;
	jpg->err = jpeg_std_error( &jerr );
	jpeg_create_compress( jpg );
	jpeg_stdio_dest( jpg, file );

	jpg->image_width = img->width;
	jpg->image_height = img->height;
	jpg->input_components = 3; // RGB
	jpg->in_color_space = JCS_RGB;
	jpeg_set_defaults( jpg );

	if ( 0 < img->quality || img->quality < 100 ) {
		jpeg_set_quality( jpg, img->quality, TRUE );
	} else {
		fprintf( stderr, "PixmapError: invalid quality value (%d), setting to default\n", img->quality );
		jpeg_set_quality( jpg, good_quality, TRUE );
	}

	switch ( img-> chroma ) {
	default:
		fprintf( stderr, "PixmapError: invalid chroma value (%d), setting to default\n", img->chroma );
	case C_444:
		jpg->comp_info[0].h_samp_factor = 1;
		jpg->comp_info[0].v_samp_factor = 1;
		break;
	case C_422:
		jpg->comp_info[0].h_samp_factor = 2;
		jpg->comp_info[0].v_samp_factor = 1;
		break;
	case C_411:
		jpg->comp_info[0].h_samp_factor = 2;
		jpg->comp_info[0].v_samp_factor = 2;
		break;
	}
	jpg->comp_info[1].h_samp_factor = 1;
	jpg->comp_info[1].v_samp_factor = 1;
	jpg->comp_info[2].h_samp_factor = 1;
	jpg->comp_info[2].v_samp_factor = 1;

	jpeg_start_compress( jpg, TRUE );
	JSAMPROW* rows = calloc( img->height, sizeof( JSAMPROW ) );
	if ( NULL == rows ) {
		fprintf( stderr, "PixmapError: cannot allocate memory\n" );
		err = 1;
		goto freeJPG;
	}
	for ( long h = 0; h < img->height; h++ ) {
		rows[ h ] = img->bytes + 3*h*img->width;
	}
	if ( img->height != jpeg_write_scanlines( jpg, rows, img->height ) ) {
		fprintf( stderr, "PixmapError: cannot write scanlines\n" );
		err = 1;
	}

	freeJPG:
		jpeg_finish_compress( jpg );
		jpeg_destroy_compress( jpg );
		free( rows );
		free( jpg );
	fcloseJPG:
		if ( 0 != fclose( file ) ) {
			fprintf( stderr, "PixmapError: cannot close JPG file '%s'\n", fileName );
			err = 1;
		}
	return err;
}
#endif // #ifdef PIXMAP_ENABLE_JPG

#endif // __PIXMAP_C__
