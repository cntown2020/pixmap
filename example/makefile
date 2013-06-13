CC=c99 -I${CODEPATH}
CFLAGS=-g -Wall -Wextra
PIXMAPLIBS=-ljpeg -lpng -lz

BINDIR=bin
OBJDIR=obj
SRCDIR=src
PIXMAPDIR=${CODEPATH}/github.com/bszcz/pixmap

VPATH=$(SRCDIR):$(PIXMAPDIR)
HEADERFILES=$(notdir $(shell ls $(PIXMAPDIR)/*.h ) )
OBJFILES=$(HEADERFILES:.h=.o)
OBJS=$(addprefix $(OBJDIR)/, $(OBJFILES))

pixmap_example: $(SRCDIR)/pixmap_example.c $(OBJS) $(PIXMAPLIBS)
	$(CC) $(CFLAGS) -o $(BINDIR)/pixmap_example $(SRCDIR)/pixmap_example.c $(OBJS) $(PIXMAPLIBS)

$(OBJDIR)/%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $(OBJDIR)/$(@F)

clean:
	@rm -i bin/pixmap_example obj/*.o
