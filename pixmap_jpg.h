// Copyright (c) 2012-2013 Bartosz Szczesny
// LICENSE: The MIT License (MIT)

#ifndef __PIXMAP_JPG_H__ // enables .jpg format, requires '-ljpeg' compilation flag
#define __PIXMAP_JPG_H__

#include <github.com/bszcz/pixmap/pixmap.h>

int pixmap_write_jpg( const struct pixmap* img, const char* filename );

#endif
