// Copyright (c) 2012-2013 Bartosz Szczesny. All rights reserved.
// See LICENSE file or http://bszcz.eu/license/BSD/?year=2012-2013

#include <github.com/bszcz/pixmap/pixmap.h>
#include <github.com/bszcz/pixmap/pixmap_png.h>
#include <png.h>

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
		rows[ h ] = img->bytes + PIXMAP_COLORS*h*img->width;
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
