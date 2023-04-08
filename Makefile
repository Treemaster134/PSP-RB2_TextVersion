TARGET = controller_basic
OBJS = main.o

INCDIR = 
CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =

LIBS =  -lpspdebug -lpspctrl -lpspdisplay -lpspkernel

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Basic controller sample
PSP_EBOOT_ICON = ICON0.png

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
