CC=c99 -g -Wall -Wextra

VPATH=src:..

pixmap_example_deps=src/pixmap_example.c lib/pixmap.a -ljpeg -lpng -lz
pixmap_example: $(pixmap_example_deps)
	$(CC) -o bin/pixmap_example $(pixmap_example_deps)

lib_pixmap_deps=obj/pixmap.o obj/pixmap_ppm.o obj/pixmap_ppm_gz.o obj/pixmap_png.o obj/pixmap_jpg.o
lib/pixmap.a: $(lib_pixmap_deps)
	ar rsv lib/pixmap.a $(lib_pixmap_deps)

obj/%.o: %.c %.h
	$(CC) -c $< -o obj/$(@F)

clean:
	rm -fv bin/pixmap_example obj/*.o
