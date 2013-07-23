// Copyright (c) 2012-2013 Bartosz Szczesny
// LICENSE: The MIT License (MIT)

#ifndef __PIXMAP_JPG_H__ // enables .jpg format, requires '-ljpeg' compilation flag
#define __PIXMAP_JPG_H__

int PixmapWriteJPG( const struct pixmap* img, const char* fileName );

#endif
