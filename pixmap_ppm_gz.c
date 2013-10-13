// Copyright (c) 2012-2013 Bartosz Szczesny
// LICENSE: The MIT License (MIT)

#include <github.com/bszcz/pixmap/pixmap.h>
#include <github.com/bszcz/pixmap/pixmap_ppm_gz.h>
#include <zlib.h>

int pixmap_write_ppm_gz( const struct pixmap* img, const char* filename ) {
	int err = 0;

	gzFile file = gzopen( filename, "wb9" ); // '9' means maximum compression level
	if ( NULL == file ) {
		fprintf( stderr, "pixmap error: cannot open PPM.GZ file '%s'\n", filename );
		return 1;
	}
	if ( 0 == gzprintf( file, "P6\n%d %d\n255\n", img->width, img->height ) ) {
		fprintf( stderr, "pixmap error: cannot write PPM.GZ header\n" );
		err = 1;
		goto gzclose_ppm;
	}
	const int data_size = PIXMAP_COLORS*img->width*img->height;
	if ( data_size != gzwrite( file, img->bytes, data_size*sizeof( unsigned char ) ) ) {
		fprintf( stderr, "pixmap error: cannot write PPM.GZ data\n" );
		err = 1;
		goto gzclose_ppm;
	}

	gzclose_ppm:
		if ( Z_OK != gzclose( file ) ) {
			fprintf( stderr, "pixmap error: cannot close PPM.GZ file '%s'\n", filename );
			err = 1;
		}
	return err;
} // pixmap_write_ppm_gz(  )
