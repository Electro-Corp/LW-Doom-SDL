/*
  SDL Rewrite of i_video

  because x11 sucks ass imo
*/

#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>


#include <netinet/in.h>
#include <errno.h>
#include <signal.h>

#include "doomstat.h"
#include "i_system.h"
#include "v_video.h"
#include "m_argv.h"
#include "d_main.h"

#include "doomdef.h"


// SDL shit
#include <SDL/SDL.h>

SDL_Surface *display;
SDL_Event event;
// debug
//#define VIDEODEBUG 1

/*
  Init SDL
*/
void I_InitGraphics (void){
  // init sdl window
  if(SDL_Init(SDL_INIT_VIDEO) < 0){
    I_Error("Failed to init SDL in I_InitGraphics");
  }

  display = SDL_SetVideoMode(SCREENWIDTH, SCREENHEIGHT, 8, SDL_HWSURFACE);
  if(!display){
    I_Error("Display was not created for some reason, figure it out.");
  }
  //SDL_WM_SetCaption("LW-Doom SDL","Doom");     
  // grab mouse
  //SDL_WM_GrabInput(SDL_GRAB_ON); // fire


  // init sdl keyboard inputs
  SDL_EnableUNICODE(1);
}
void I_ReadScreen(byte* src){
  
  memcpy(src, screens[0],SCREENWIDTH*SCREENHEIGHT);
}

void I_ShutdownGraphics(void){
  SDL_Quit();
}

void I_StartFrame(void){
  // errrm what the guh
}

// ticks
void I_StartTic(){
  if(!display)return;
  // keyboard 
  event_t eventt;
  while(SDL_PollEvent(&event)){
    switch( event.type ){
      case SDL_QUIT:
        exit(0);
        break;

      // bruh
      case SDL_KEYDOWN:
        event.type = ev_keydown;
        event.data1 = &event.key;
        D_PostEvent(&event);
        break;
    }
  }
}


// Takes full 8 bit values.
void I_SetPalette (byte* palette){
  
}

void I_UpdateNoBlit (void){
  
}
void I_FinishUpdate (void){
  int i = 0, x = 0, y = 0;
  while(i < SCREENWIDTH*SCREENHEIGHT){
    if(i % SCREENWIDTH == 0){
      x = 0;
      y++;
    }else{
      x++;
    }
    Uint8 *p = (Uint8 *)display->pixels + y * display->pitch + x * 4;
    *(Uint32 *)p = screens[0][i++];
  }
  //display = surface;  
  #ifdef VIDEODEBUG
  SDL_Rect sDim;
  sDim.w = 100;
	sDim.h = 100;
	sDim.x = SCREENWIDTH/2;
	sDim.y = SCREENHEIGHT/2;
  SDL_FillRect(display, &sDim, SDL_MapRGB(display->format, 0, 0, 255));
  #endif
  SDL_UpdateRect(display, 0, 0, 0, 0);
}