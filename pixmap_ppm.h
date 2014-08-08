// Copyright (c) 2012-2013 Bartosz Szczesny
// LICENSE: The MIT License (MIT)

#ifndef PIXMAP_PPM_H // enables .ppm format
#define PIXMAP_PPM_H

#include "pixmap.h"

int pixmap_write_ppm(const struct pixmap* img, const char* filename);

#endif
