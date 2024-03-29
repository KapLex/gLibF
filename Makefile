PSPSDK = $(shell psp-config --pspsdk-path)
PSPDIR = $(shell psp-config --psp-prefix)

CFLAGS = -O2 -G0 -g

OBJS = glibf.o glib2d.o
TARGET_LIB = libglibf.a

include $(PSPSDK)/lib/build.mak

install: all
	mkdir -p $(PSPDIR)/include $(PSPDIR)/lib
	cp *.h $(PSPDIR)/include
	cp *.a $(PSPDIR)/lib
