// Copyright (c) 2012-2013 Bartosz Szczesny
// LICENSE: The MIT License (MIT)

#include "pixmap_ppm.h"

int pixmap_write_ppm(const struct pixmap* img, const char* filename) {
	int err = 0;

	FILE* file = fopen(filename, "wb");
	if (NULL == file) {
		fprintf(stderr, "pixmap error: cannot open PPM file '%s'\n", filename);
		return 1;
	}
	if (0 > fprintf(file, "P6\n%d %d\n255\n", img->width, img->height)) {
		fprintf(stderr, "pixmap error: cannot write PPM header\n");
		err = 1;
		goto fclose_ppm;
	}
	const size_t data_size = PIXMAP_COLORS*img->width*img->height;
	if (data_size != fwrite(img->bytes, sizeof(unsigned char), data_size, file)) {
		fprintf(stderr, "pixmap error: cannot write PPM data\n");
		err = 1;
	}

fclose_ppm:
	if (0 != fclose(file)) {
		fprintf(stderr, "pixmap error: cannot close PPM file '%s'\n", filename);
		err = 1;
	}
	return err;
} // pixmap_write_ppm(  )
