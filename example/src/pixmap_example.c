// Copyright (c) 2012-2013 Bartosz Szczesny
// LICENSE: The MIT License (MIT)

#include "../../pixmap_ppm.h"
#include "../../pixmap_ppm_gz.h"
#include "../../pixmap_png.h"
#include "../../pixmap_jpg.h"

void pixmap_print(struct pixmap* img);

int main() {
	struct pixmap* img = pixmap_alloc(1600, 1200);
	if (NULL == img) {
		exit(EXIT_FAILURE); // error message is printed by pixmap_alloc()
	}

	double x = 0.666;
	for (int p = 0; p < PIXMAP_COLORS*img->width*img->height; p++) {
		x = 4 * x * (1.0 - x); // random numbers from chaotic logistic map
		img->bytes[ p ] = 256 * x;
	}
	//pixmap_print(img);

	pixmap_write_ppm(img, "pixmap.ppm");
	pixmap_write_ppm_gz(img, "pixmap.ppm.gz");
	pixmap_write_png(img, "pixmap.png");
	pixmap_write_jpg(img, "pixmap.jpg");

	img->chroma = PIXMAP_CHROMA_422;
	img->quality = 40;
	pixmap_write_jpg(img, "pixmap_c422_q40.jpg");
	img->chroma = PIXMAP_CHROMA_411;
	img->quality = 20;
	pixmap_write_jpg(img, "pixmap_c411_q20.jpg");

	pixmap_free(img);
	return 0;
}

void pixmap_print(struct pixmap* img) {
	for (int p = 0; p < PIXMAP_COLORS*img->width*img->height; p++) {
		printf("bytes[ %d ] = %u\n", p, img->bytes[ p ]);
	}
	for (int p = 0; p < img->width*img->height; p++) {
		for (int c = 0; c < PIXMAP_COLORS; c++) {
			printf("pixels[ %d ][ %d ] = %u\n", p, c, img->pixels[ p ][ c ]);
		}
	}
	for (int h = 0; h < img->height; h++) {
		for (int w = 0; w < img->width; w++) {
			for (int c = 0; c < PIXMAP_COLORS; c++) {
				printf("pixels2[ %d ][ %d ][ %d ] = %u\n", h, w, c, img->pixels2[ h ][ w ][ c ]);
			}
		}
	}
	return;
}
