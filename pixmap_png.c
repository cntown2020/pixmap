// Copyright (c) 2012-2014 Bartosz Szczesny
// LICENSE: The MIT License (MIT)

#include "pixmap_png.h"

int pixmap_write_png(const struct pixmap* img, const char* filename) {
	int err = 0;

	FILE* file = fopen(filename, "wb");
	if (file == NULL) {
		fprintf(stderr, "pixmap error: cannot open PNG file '%s'\n", filename);
		return 1;
	}
	png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png == NULL) {
		fprintf(stderr, "pixmap error: cannot create PNG write struct");
		err = 1;
		goto fclose_png;
	}
	png_infop info = png_create_info_struct(png);
	if (info == NULL) {
		fprintf(stderr, "pixmap error: cannot create PNG info struct");
		err = 1;
		goto destroy_png;
	}
	png_bytepp rows = calloc(img->height, sizeof(png_bytep));
	if (rows == NULL) {
		fprintf(stderr, "pixmap error: cannot allocate memory\n");
		err = 1;
		goto destroy_png;
	}
	for (int h = 0; h < img->height; h++) {
		rows[h] = img->bytes + PIXMAP_COLORS*h*img->width;
	}

	const int depth = 8;
	png_init_io(png, file);
	png_set_IHDR(png, info, img->width, img->height, depth,
	             PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	png_write_info(png, info);
	png_write_rows(png, rows, img->height);
	png_write_end(png, info);
	free(rows);

destroy_png:
	png_destroy_write_struct(&png, &info);
fclose_png:
	if (fclose(file) != 0) {
		fprintf(stderr, "pixmap error: cannot close PNG file '%s'\n", filename);
		err = 1;
	}
	return err;
} // pixmap_write_png()
