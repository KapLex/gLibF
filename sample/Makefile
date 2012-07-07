TARGET = glibfsample
OBJS = ../glib2d.o ../glibf.o main.o

INCDIR =
CFLAGS = -G0 -Wall -O2
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR = 
LDFLAGS =
LIBS= -lpsppower -lpspgu -lpspvram -lpng -ljpeg -lz -lm -lfreetype -I /usr/local/pspdev/psp/include/freetype2/

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = gLibF Sample

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
