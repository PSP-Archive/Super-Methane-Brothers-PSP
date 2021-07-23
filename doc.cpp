/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 * Program WebSite: http://methane.sourceforge.net/index.html              *
 * Project Email: rombust@postmaster.co.uk                                 *
 *                                                                         *
 ***************************************************************************/


//------------------------------------------------------------------------------
// The GP2X main document file
//------------------------------------------------------------------------------
#ifdef GP2X_WIN32_DEBUG
#include <windows.h>
#endif

#include <stdlib.h>
#include <pspkernel.h>
#include <pspctrl.h>

#include "SDL.h"
#include "SDL_framerate.h"
#include "doc.h"
#include "splash.cpp"
#include <SDL/SDL_image.h>
//cheats
int levelcheat=0;
int plus5=0;
int fast=0;

#ifdef METHANE_MIKMOD
#include "mikmod/audiodrv.h"
#endif
int VOL;

PSP_HEAP_SIZE_KB(20480);

int up1p        = 8;
int down1p      = 6;
int left1p      = 7;
int right1p     = 9;
int fire1p      = 2;
int up2p        = 15;
int down2p      = 15;
int left2p      = 15;
int right2p     = 15;
int fire2p      = 1;

int mypause=0;

void one_player_layout() {
 	up1p        = 8;
 	down1p      = 6;
 	left1p      = 7;
 	right1p     = 9;
 	fire1p      = 2;
 	up2p        = 15;
 	down2p      = 15;
 	left2p      = 15;
 	right2p     = 15;
 	fire2p      = 1;
} 

void two_player_layout() {
 	up1p        = 8;
 	down1p      = 6;
 	left1p      = 7;
 	right1p     = 9;
 	fire1p      = 4;
 	up2p        = 0;
 	down2p      = 2;
 	left2p      = 3;
 	right2p     = 1;
 	fire2p      = 5;
} 

//------------------------------------------------------------------------------
// Private Functions
//------------------------------------------------------------------------------
extern void main_code(void);

//------------------------------------------------------------------------------
// The Game Instance
//------------------------------------------------------------------------------
CMethDoc Game;
FPSmanager FpsManager;
SDL_Surface * SdlScreen = 0;


static char TheScreen[SCR_WIDTH * SCR_HEIGHT];
static int SampleChannel = 5;	// Used by CMethDoc::PlaySample

//------------------------------------------------------------------------------
// The HighScore table filename
//------------------------------------------------------------------------------
static char HighScoreFileName[] = "Methane.HiScores";
#define HighScoreLoadBufferSize (MAX_HISCORES * 64)


//------------------------------------------------------------------------------
// The Main Function
//------------------------------------------------------------------------------

#ifdef GP2X_WIN32_DEBUG
int APIENTRY WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
)
#else
extern "C" int SDL_main( int argc, char *argv[] )
#endif
{
	// init
	if (SDL_Init(SDL_INIT_JOYSTICK|SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0) {
		fprintf (stderr, "Can't init SDL : %s", SDL_GetError());
		return 1;
	}
	atexit (SDL_Quit);
	SDL_JoystickOpen (0);

    SDL_EnableKeyRepeat (
		SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	// run
	main_code();					// The main routine


	return (0) ;
}

void fade_in(SDL_Surface *target,SDL_Surface *image) {
	SDL_Rect destination;
	destination.x = 0;
	destination.y = -256;
	destination.w = 320;
	destination.h = 256;
	for (int i=-256; i<1; i+=2) {	
     		SDL_BlitSurface(image,NULL,target,&destination); 
		destination.y=i;
          	SDL_Flip(target);
          	//SDL_Delay(1);
	}
}

void fade_out(SDL_Surface *target)
{
     /*
        target is our target surface, which would normally
        be the screen.
     */
     
     SDL_Surface *temp;
     temp = SDL_DisplayFormat(target);
     if (temp == NULL)
     {
        printf("SDL_DisplayFormat() failed.\n");
        exit(1);
     }
     /*This starts it out with 50% opacity.*/
     SDL_SetAlpha(temp,SDL_SRCALPHA,128);     
     SDL_BlitSurface(target,NULL,temp,NULL);     

     for (int i=128; i >= 110; i--)
     {
          SDL_SetAlpha(temp,SDL_SRCALPHA,i);
          SDL_BlitSurface(target,NULL,temp,NULL);               
          SDL_FillRect(target,NULL,0x000000);     
          SDL_BlitSurface(temp,NULL,target,NULL);
          SDL_Flip(target);
          SDL_Delay(10);
     }
     SDL_FreeSurface(temp);
}

//------------------------------------------------------------------------------
//! \brief The program main code
//------------------------------------------------------------------------------
void main_code(void)
{
VOL=64;
	JOYSTICK *jptr1;
	JOYSTICK *jptr2;
	//check the cheats
	FILE * fp;

	fp = fopen("levelcheat","r");
	if (fp!=NULL) { 
		levelcheat=1;
		fclose(fp);
	}
	fp = fopen("plus5","r");
	if (fp!=NULL) { 
		plus5=1;
		fclose(fp);
	}
	fp = fopen("fast","r");
	if (fp!=NULL) { 
		fast=1;
		fclose(fp);
	}
	//deniska: display compo splash page
	SdlScreen = SDL_SetVideoMode (
		480, 272, 32, SDL_SWSURFACE);
	if (!SdlScreen)
	{
		fprintf (stderr, "Couldn't set video mode : %s\n", SDL_GetError());
		return;
	}
	SDL_ShowCursor (SDL_DISABLE);
/*
	printf("The GNU General Public License V2 applies to this game.\n\n");
	printf("See: http://methane.sourceforge.net\n");
	printf("Instructions:\n\n");
	printf("Press X to start game (You can't enter player names).\n");
	printf("Press A to fire gas from the gun.\n");
	printf("Press X to jump.\n");
	printf("Hold A to suck a trapped baddie into the gun.\n");
	printf("Release A to throw the trapped baddie from the gun.\n");
	printf("Throw baddies at the wall to destroy them.\n\n");
	printf("START = Quit (and save high scores)\n");
	printf("SELECT = Change player graphic during game\n");
	//printf("F1 = Next Level (DISABLED)\n");
*/
	SDL_RWops *rwops = SDL_RWFromMem(splash,sizeof(splash) );
	if (rwops==NULL) {

                printf("!could not load the splash image\n");
	}
	 SDL_Surface *s1,*s2;
        if((s1=SDL_LoadBMP_RW(rwops,1))==NULL) {
                printf("could not load the splash image\n");
        } else {
                if((s2=SDL_DisplayFormat(s1))==NULL)
                printf("cant convert splash image to display format");
         else
                SDL_BlitSurface(s2,NULL,SdlScreen,NULL);
        }
        SDL_Flip(SdlScreen);
	SDL_Delay(3000); 
	fade_out(SdlScreen);
	SDL_FreeSurface(SdlScreen);
	SDL_FreeSurface(s1);
	SDL_FreeSurface(s2);

	SdlScreen = SDL_SetVideoMode (
		GP2X_SCREEN_W, GP2X_SCREEN_H, 8, SDL_SWSURFACE);
	if (!SdlScreen)
	{
		fprintf (stderr, "Couldn't set video mode : %s\n", SDL_GetError());
		return;
	}

	printf("Game.InitSoundDriver()\n");
	Game.InitSoundDriver();
	printf("Game.InitGame()\n");
	Game.InitGame ();
	Game.LoadScores();
	Game.StartGame();
	jptr1 = &Game.m_GameTarget.m_Joy1;
	jptr2 = &Game.m_GameTarget.m_Joy2;

	SDL_initFramerate (&FpsManager);

	rwops = SDL_RWFromMem(intro1,sizeof(intro1) );
	if (rwops==NULL) {

                printf("!could not load the splash image\n");
	}
        if((s1=SDL_LoadBMP_RW(rwops,1))==NULL) {
                printf("could not load the splash image\n");
        } else {
                if((s2=SDL_DisplayFormat(s1))==NULL)
                printf("cant convert splash image to display format");
         else
		fade_in(SdlScreen,s2);
        }
        SDL_BlitSurface(s2,NULL,SdlScreen,NULL);
        SDL_Flip(SdlScreen);
	SDL_Delay(3000); 
	SDL_FreeSurface(s1);
	SDL_FreeSurface(s2);
	rwops = SDL_RWFromMem(intro2,sizeof(intro3) );
	if (rwops==NULL) {

                printf("!could not load the splash image\n");
	}
        if((s1=SDL_LoadBMP_RW(rwops,1))==NULL) {
                printf("could not load the splash image\n");
        } else {
                if((s2=SDL_DisplayFormat(s1))==NULL)
                printf("cant convert splash image to display format");
         else
		fade_in(SdlScreen,s2);
        }
        SDL_BlitSurface(s2,NULL,SdlScreen,NULL);
        SDL_Flip(SdlScreen);
	SDL_Delay(3000); 
	SDL_FreeSurface(s1);
	SDL_FreeSurface(s2);
	rwops = SDL_RWFromMem(intro3,sizeof(intro3) );
	if (rwops==NULL) {

                printf("!could not load the splash image\n");
	}
        if((s1=SDL_LoadBMP_RW(rwops,1))==NULL) {
                printf("could not load the splash image\n");
        } else {
                if((s2=SDL_DisplayFormat(s1))==NULL)
                printf("cant convert splash image to display format");
         else
		fade_in(SdlScreen,s2);
        }
        SDL_BlitSurface(s2,NULL,SdlScreen,NULL);
        SDL_Flip(SdlScreen);
	SDL_Delay(3000); 
	SDL_FreeSurface(s1);
	SDL_FreeSurface(s2);

	int run = 1;
	while(run)
	{
#ifdef GP2X_WIN32_DEBUG
		SDL_PumpEvents();
		Uint8 * key = SDL_GetKeyState (NULL);

		if (key[SDLK_ESCAPE]) break;
		if (key[SDLK_TAB])
		{
			Game.m_GameTarget.m_Game.TogglePuffBlow();
		}
//		jptr1->key = key;
//		jptr2->key = key;

		if (key[SDLK_RIGHT]) {
			jptr1->right = 1;
		}
		else {
			jptr1->right = 0;
		}

		jptr1->left  = key[SDLK_LEFT] > 0;
		jptr1->up    = key[SDLK_UP] > 0;
		jptr1->down  = key[SDLK_DOWN] > 0;
		jptr1->fire  = (key[SDLK_LCTRL]) || (key[SDLK_RCTRL]);
		jptr1->key = 13;	// Fake key press (required for high score table)
#else
		
		// event loop
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_JOYAXISMOTION:
					 if(event.jaxis.axis==0) {
                                                if(event.jaxis.value <= -16384) {
                                                         jptr1->left=1;
                                                } else {
                                                         jptr1->left=0;
                                                }
                                                if(event.jaxis.value >= 16384) {
                                                         jptr1->right=1;
                                                } else {
                                                         jptr1->right=0;
                                                }
                                        }
					if(event.jaxis.axis==1) {
                                                if(event.jaxis.value <= -16384) {
                                                         jptr1->up=1;
                                                } else {
                                                         jptr1->up=0;
                                                }
                                                if(event.jaxis.value >= 16384) {
                                                         jptr1->down=1;
                                                } else {
                                                         jptr1->down=0;
                                                }
                                        }


					break;
				case SDL_JOYBUTTONDOWN:
					if (event.jbutton.button==up1p) {
						 jptr1->up  = 1; 
						 jptr1->key=jptr1->key+1;
						if (jptr1->key>122)
							jptr1->key=122;
					}
					if (event.jbutton.button==down1p) {
						 jptr1->key=jptr1->key-1;
						if (jptr1->key<32)
							jptr1->key=32;
						 jptr1->down  = 1; 
					}
					if (event.jbutton.button==left1p) jptr1->left  = 1; 
					if (event.jbutton.button==right1p) jptr1->right  = 1; 
					if (event.jbutton.button==fire1p) { jptr1->fire  = 1;  jptr1->key=13; }
					if (event.jbutton.button==up2p) jptr2->up  = 1; 
					if (event.jbutton.button==down2p) jptr2->down  = 1; 
					if (event.jbutton.button==left2p) jptr2->left  = 1; 
					if (event.jbutton.button==right2p) jptr2->right  = 1; 
					if (event.jbutton.button==fire2p) jptr2->fire  = 1; 
					if (event.jbutton.button==10 && levelcheat==1) jptr1->next_level = 1; 
					if (event.jbutton.button==12) run = 0; 
					if (event.jbutton.button==13)  Game.m_GameTarget.m_Game.TogglePuffBlow(); 
					break;
				case SDL_JOYBUTTONUP:
					if (event.jbutton.button==up1p) jptr1->up  = 0; 
					if (event.jbutton.button==down1p) jptr1->down  = 0; 
					if (event.jbutton.button==left1p) jptr1->left  = 0; 
					if (event.jbutton.button==right1p) jptr1->right  = 0; 
					if (event.jbutton.button==fire1p) jptr1->fire  = 0; 
					if (event.jbutton.button==up2p) jptr2->up  = 0; 
					if (event.jbutton.button==down2p) jptr2->down  = 0; 
					if (event.jbutton.button==left2p) jptr2->left  = 0; 
					if (event.jbutton.button==right2p) jptr2->right  = 0; 
					if (event.jbutton.button==fire2p) jptr2->fire  = 0; 
					if (event.jbutton.button==10) jptr1->next_level = 0; 
					if (event.jbutton.button==11)  {
						Game.m_GameTarget.m_Game.m_pSound->PlaySample(SND_FINLEV1);
						if (mypause==0)
							mypause=1;
						else mypause=0;
					} 
					break;
				default:
					break;
			}
		}

		//jptr1->key = 13;	// Fake key press (required to pass getPlayerName screen)
#endif

		// (CHEAT MODE DISABLED) --> jptr1->next_level = 0;
		Game.MainLoop (mypause);
		SDL_framerateDelay (&FpsManager);
	}
	Game.SaveScores ();

	SDL_FreeSurface (SdlScreen);
}

//------------------------------------------------------------------------------
//! \brief Initialise Document
//------------------------------------------------------------------------------
CMethDoc::CMethDoc()
{
#ifdef METHANE_MIKMOD
	SMB_NEW(m_pMikModDrv,CMikModDrv);
#endif
	m_GameTarget.Init(this);
}

//------------------------------------------------------------------------------
//! \brief Destroy Document
//------------------------------------------------------------------------------
CMethDoc::~CMethDoc()
{
#ifdef METHANE_MIKMOD
	if (m_pMikModDrv)
	{
		delete(m_pMikModDrv);
		m_pMikModDrv = 0;
	}
#endif
}

//------------------------------------------------------------------------------
//! \brief Initialise the game
//------------------------------------------------------------------------------
void CMethDoc::InitGame(void)
{
	m_GameTarget.InitGame (TheScreen);
	m_GameTarget.PrepareSoundDriver ();
}

//------------------------------------------------------------------------------
//! \brief Initialise the sound driver
//------------------------------------------------------------------------------
void CMethDoc::InitSoundDriver(void)
{
#ifdef METHANE_MIKMOD
	m_pMikModDrv->InitDriver();
#endif
}

//------------------------------------------------------------------------------
//! \brief Remove the sound driver
//------------------------------------------------------------------------------
void CMethDoc::RemoveSoundDriver(void)
{
#ifdef METHANE_MIKMOD
	m_pMikModDrv->RemoveDriver();
#endif
}

//------------------------------------------------------------------------------
//! \brief Start the game
//------------------------------------------------------------------------------
void CMethDoc::StartGame(void)
{
	m_GameTarget.StartGame();
}

//------------------------------------------------------------------------------
//! \brief Redraw the game main view
//!
//! 	\param pal_change_flag : 0 = Palette not changed
//------------------------------------------------------------------------------
void CMethDoc::RedrawMainView( int pal_change_flag )
{
	// Function not used
}

//------------------------------------------------------------------------------
//! \brief Draw the screen
//!
//! 	\param screen_ptr = UNUSED
//------------------------------------------------------------------------------
void CMethDoc::DrawScreen(  )
{
	METHANE_RGB *pptr;
	int cnt;
    SDL_Color colors [PALETTE_SIZE];

    if (SDL_MUSTLOCK (SdlScreen))
	{
		SDL_LockSurface (SdlScreen);
	}

	// Set the game palette
	pptr = m_GameTarget.m_rgbPalette;
	for (cnt=0; cnt < PALETTE_SIZE; cnt++, pptr++)
	{
		colors[cnt].r = pptr->red;
		colors[cnt].g = pptr->green;
		colors[cnt].b = pptr->blue;
	}
	SDL_SetPalette (SdlScreen, SDL_LOGPAL|SDL_PHYSPAL, colors, 0, PALETTE_SIZE);

	// Copy the pixels
	char * dptr = (char *) SdlScreen->pixels;
	char * sptr = TheScreen + 4 * GP2X_SCREEN_W;
	for (int y = 0; y < GP2X_SCREEN_H; y++) {
		for (int x = 0; x < GP2X_SCREEN_W; x++) {
			*dptr++ = *sptr++;
		}
		dptr += (SdlScreen->pitch - GP2X_SCREEN_W);
	}

    if (SDL_MUSTLOCK (SdlScreen))
	{
		SDL_UnlockSurface (SdlScreen);
	}

	// Show the new screen
	SDL_Flip (SdlScreen);
}

//------------------------------------------------------------------------------
//! \brief The Game Main Loop
//!
//! 	\param screen_ptr = UNUSED
//------------------------------------------------------------------------------
//void CMethDoc::MainLoop( void *screen_ptr )
void CMethDoc::MainLoop( int i )
{
	m_GameTarget.MainLoop(i);
	DrawScreen(  );
#ifdef METHANE_MIKMOD
	m_pMikModDrv->Update();
#endif
}

//------------------------------------------------------------------------------
//! \brief Play a sample (called from the game)
//!
//! 	\param id = SND_xxx id
//!	\param pos = Sample Position to use 0 to 255
//!	\param rate = The rate
//------------------------------------------------------------------------------
void CMethDoc::PlaySample(int id, int pos, int rate)
{
#ifdef METHANE_MIKMOD
	m_pMikModDrv->PlaySample(id, pos, rate);
#endif
}

//------------------------------------------------------------------------------
//! \brief Stop the module (called from the game)
//------------------------------------------------------------------------------
void CMethDoc::StopModule(void)
{
#ifdef METHANE_MIKMOD
	m_pMikModDrv->StopModule();
#endif
}

//------------------------------------------------------------------------------
//! \brief Play a module (called from the game)
//!
//! 	\param id = SMOD_xxx id
//------------------------------------------------------------------------------
void CMethDoc::PlayModule(int id)
{
#ifdef METHANE_MIKMOD
	m_pMikModDrv->PlayModule(id);
#endif
}

//------------------------------------------------------------------------------
//! \brief Update the current module (ie restart the module if it has stopped) (called from the game)
//!
//! 	\param id = SMOD_xxx id (The module that should be playing)
//------------------------------------------------------------------------------
void CMethDoc::UpdateModule(int id)
{
#ifdef METHANE_MIKMOD
	m_pMikModDrv->UpdateModule(id);
#endif
}

//------------------------------------------------------------------------------
//! \brief Load the high scores
//------------------------------------------------------------------------------
void CMethDoc::LoadScores(void)
{
	FILE *fptr;
	char *mptr;
	char *tptr;
	char let;
	int cnt;

	fptr = fopen(HighScoreFileName, "r");
	if (!fptr) return;	// No scores available

	// Allocate file memory, which is cleared to zero
	mptr = (char *) calloc(1, HighScoreLoadBufferSize);
	if (!mptr)		// No memory
	{
		fclose(fptr);
		return;
	}
	fread( mptr, 1, HighScoreLoadBufferSize-2, fptr);	// Get the file

	// (Note: mptr is zero terminated)
	tptr = mptr;
	for (cnt=0; cnt<MAX_HISCORES; cnt++)	// For each highscore
	{

		if (!tptr[0]) break;

		m_GameTarget.m_Game.InsertHiScore( atoi(&tptr[4]), tptr );

		do	// Find next name
		{
			let = *(tptr++);
		}while (!( (let=='$') || (!let) ));
		if (!let) break;	// Unexpected EOF
	}

	free(mptr);

	fclose(fptr);

}

//------------------------------------------------------------------------------
//! \brief Save the high scores
//------------------------------------------------------------------------------
void CMethDoc::SaveScores(void)
{
	FILE *fptr;
	int cnt;
	HISCORES *hs;

	fptr = fopen(HighScoreFileName, "w");
	if (!fptr) return;	// Cannot write scores
	for (cnt=0, hs=m_GameTarget.m_Game.m_HiScores; cnt<MAX_HISCORES; cnt++, hs++)
	{
		fprintf(fptr, "%c%c%c%c%d$", hs->name[0], hs->name[1], hs->name[2], hs->name[3], hs->score);
	}
	fclose(fptr);

}

