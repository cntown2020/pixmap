// Copyright (c) 2012-2013 Bartosz Szczesny
// LICENSE: The MIT License (MIT)

#ifndef __PIXMAP_PPM_GZ_H__ // enables .ppm.gz format, requires '-lz' compilation flag
#define __PIXMAP_PPM_GZ_H__

#include "pixmap.h"
#include <zlib.h>

int pixmap_write_ppm_gz( const struct pixmap* img, const char* filename );

#endif
