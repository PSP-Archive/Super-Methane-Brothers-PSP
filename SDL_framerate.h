
/*

 SDL_framerate: framerate manager

 LGPL (c) A. Schiffler

 */

#ifndef _SDL_framerate_h
#define _SDL_framerate_h

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* --- */

#include "SDL.h"

/* --------- Definitions */

/* Some rates in Hz */

#define FPS_UPPER_LIMIT		50
#define FPS_LOWER_LIMIT		1
#define FPS_DEFAULT	 20

/* --------- Structure variables */

    typedef struct {
	Uint32 framecount;
	float rateticks;
	Uint32 lastticks;
	Uint32 rate;
    } FPSmanager;

/* --------- Function prototypes */

#define DLLINTERFACE

/* Functions return 0 or value for sucess and -1 for error */

    DLLINTERFACE void SDL_initFramerate(FPSmanager * manager);
    DLLINTERFACE int SDL_setFramerate(FPSmanager * manager, int rate);
    DLLINTERFACE int SDL_getFramerate(FPSmanager * manager);
    DLLINTERFACE void SDL_framerateDelay(FPSmanager * manager);

/* --- */

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif				/* _SDL_framerate_h */
