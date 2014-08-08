// Copyright (c) 2012-2013 Bartosz Szczesny
// LICENSE: The MIT License (MIT)

#ifndef PIXMAP_JPG_H // enables .jpg format, requires '-ljpeg' compilation flag
#define PIXMAP_JPG_H

#include "pixmap.h"
#include <jpeglib.h>

int pixmap_write_jpg(const struct pixmap* img, const char* filename);

#endif
