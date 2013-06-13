// Copyright (c) 2012-2013 Bartosz Szczesny. All rights reserved.
// See LICENSE file or http://bszcz.eu/license/BSD/?year=2012-2013

#include <github.com/bszcz/pixmap/pixmap.h>
#include <github.com/bszcz/pixmap/pixmap_ppm_gz.h>
#include <zlib.h>

int PixmapWritePPM_GZ( const struct pixmap* img, const char* fileName ) {
	int err = 0;

	gzFile file = gzopen( fileName, "wb9" ); // '9' means maximum compression level
	if ( NULL == file ) {
		fprintf( stderr, "PixmapError: cannot open PPM.GZ file '%s'\n", fileName );
		return 1;
	}
	if ( 0 == gzprintf( file, "P6\n%ld %ld\n255\n", img->width, img->height ) ) {
		fprintf( stderr, "PixmapError: cannot write PPM.GZ header\n" );
		err = 1;
		goto gzclosePPM;
	}
	const long data_size = PIXMAP_COLORS*img->width*img->height;
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
