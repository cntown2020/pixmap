// Copyright (c) 2012-2014 Bartosz Szczesny <bszcz@bszcz.org>
// This program is free software under the MIT license.

#include "pixmap_ppm_gz.h"

int pixmap_write_ppm_gz(const struct pixmap* img, const char* filename) {
	int err = 0;

	gzFile file = gzopen(filename, "wb9"); // '9' means maximum compression level
	if (file == NULL) {
		fprintf(stderr, "pixmap error: cannot open PPM.GZ file '%s'\n", filename);
		return 1;
	}
	if (gzprintf(file, "P6\n%d %d\n255\n", img->width, img->height) == 0) {
		fprintf(stderr, "pixmap error: cannot write PPM.GZ header\n");
		err = 1;
		goto gzclose_ppm;
	}
	const int data_size = PIXMAP_COLORS*img->width*img->height;
	if (gzwrite(file, img->bytes, data_size*sizeof(unsigned char)) != data_size) {
		fprintf(stderr, "pixmap error: cannot write PPM.GZ data\n");
		err = 1;
		goto gzclose_ppm;
	}

gzclose_ppm:
	if (gzclose(file) != Z_OK) {
		fprintf(stderr, "pixmap error: cannot close PPM.GZ file '%s'\n", filename);
		err = 1;
	}
	return err;
} // pixmap_write_ppm_gz()
