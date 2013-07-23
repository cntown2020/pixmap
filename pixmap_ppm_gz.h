// Copyright (c) 2012-2013 Bartosz Szczesny
// LICENSE: The MIT License (MIT)

#ifndef __PIXMAP_PPM_GZ_H__ // enables .ppm.gz format, requires '-lz' compilation flag
#define __PIXMAP_PPM_GZ_H__

int PixmapWritePPM_GZ( const struct pixmap* img, const char* fileName );

#endif
