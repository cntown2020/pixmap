// Copyright (c) 2012-2013 Bartosz Szczesny
// LICENSE: The MIT License (MIT)

#ifndef __PIXMAP_PNG_H__ // enables .png format, requires '-lpng' compilation flag
#define __PIXMAP_PNG_H__

int PixmapWritePNG( const struct pixmap* img, const char* fileName );

#endif
