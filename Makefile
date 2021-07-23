TARGET = mb

BUILD_PRX=1
PSP_FW_VERSION = 371

PSP_EBOOT_UNKPNG = PIC0.PNG
#PSP_EBOOT_PIC1 = PIC1.PNG
PSP_EBOOT_ICON = ICON0.PNG
PSP_EBOOT_SND0 = SND0.AT3

PSPSDK=$(shell psp-config --pspsdk-path)
PSPDIR=$(shell psp-config --psp-prefix)
PSPBIN = $(PSPSDK)/../bin

SDL_CONFIG = $(PSPBIN)/sdl-config


OBJS = SDL_framerate.o  bitgroup.o  doc.o     gfxoff.o  maps.o     player.o  suck.o \
baddie.o       bititem.o     game.o    global.o  misc.o     power.o   target.o \
bitdraw.o      boss.o        gasobj.o  goodie.o  objlist.o  sound.o   weapon.o \
data/gfxdata.o  data/gfxdata2.o  data/gfxdata3.o  data/mapdata.o  data/snddata.o\
mikmod/audiodrv.o


CFLAGS = -I. -I../JLib -O3  -G0 -Wall -g  $(shell $(SDL_CONFIG) --cflags) -DUNIX   -Wall  -ffast-math  -Dfont_prefix=\"fonts\"     -fsingle-precision-constant -mno-check-zero-division  -funsafe-math-optimizations -fpeel-loops $(DEFINES) -I ../linux -DMETHANE_MIKMOD -I ../mikmod
#CFLAGS = -I. -I../JLib -O3  -G0 -Wall -g  $(shell $(SDL_CONFIG) --cflags) -DUNIX   -Wall  -ffast-math  -Dfont_prefix=\"fonts\"     -fsingle-precision-constant -mno-check-zero-division  -funsafe-math-optimizations -fpeel-loops $(DEFINES) -I ../linux 


#LDFLAGS=-L. -ffast-math -lSDL_image -lSDL_ttf -lSDL_mixer `$(SDL_CONFIG) --libs`

#LIBS =  -lSDL_mixer -lsmpeg  -lstdc++  -lvorbisidec -lSDL_ttf -lfreetype -lSDL_image -lpng -lz -ljpeg  -lSDL_gfx -lm $(shell $(SDL_CONFIG) --libs) -lpsprtc  -lpspvfpu  -lpsppower
LIBS =    -lstdc++   -lSDL_image -lpng -lz -ljpeg  -lmikmod  -lm $(shell $(SDL_CONFIG) --libs) -lpsprtc  -lpspvfpu  -lpsppower -lpspaudio


EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = methane_bros

include $(PSPSDK)/lib/build.mak



