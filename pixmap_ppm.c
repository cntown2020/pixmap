// Copyright (c) 2012-2013 Bartosz Szczesny
// LICENSE: The MIT License (MIT)

#include <github.com/bszcz/pixmap/pixmap.h>
#include <github.com/bszcz/pixmap/pixmap_ppm.h>

int PixmapWritePPM( const struct pixmap* img, const char* fileName ) {
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
	const size_t data_size = PIXMAP_COLORS*img->width*img->height;
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
