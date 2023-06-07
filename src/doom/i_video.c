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
  // kys (keep yourself safe)
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

/*
  Convert SDL Key to doom key
*/
int translatekey(SDL_KeyboardEvent* key){
  int rc;
  switch(key->keysym.sym){
    case SDLK_LEFT: rc = KEY_LEFTARROW; break;
    case SDLK_RIGHT: rc = KEY_RIGHTARROW; break;
    case SDLK_UP: rc = KEY_UPARROW; break;
    case SDLK_DOWN: rc = KEY_DOWNARROW; break;

    case SDLK_RETURN: rc = KEY_ENTER; break;
    case SDLK_TAB:	rc = KEY_TAB;		break;
    case SDLK_F1:	rc = KEY_F1;		break;
    case SDLK_F2:	rc = KEY_F2;		break;
    case SDLK_F3:	rc = KEY_F3;		break;
    case SDLK_F4:	rc = KEY_F4;		break;
    case SDLK_F5:	rc = KEY_F5;		break;
    case SDLK_F6:	rc = KEY_F6;		break;
    case SDLK_F7:	rc = KEY_F7;		break;
    case SDLK_F8:	rc = KEY_F8;		break;
    case SDLK_F9:	rc = KEY_F9;		break;
    case SDLK_F10:	rc = KEY_F11;		break;
    case SDLK_F12:	rc = KEY_F12;		break;
    case SDLK_ESCAPE: rc = KEY_ESCAPE; break;
    case SDLK_LCTRL: rc = KEY_RCTRL; break;
    case SDLK_LSHIFT: rc = KEY_RSHIFT; break;
    //case SDLK_SPACE: rc = KEY_SPACE; break;
    default: 
    if (key->keysym.sym == SDLK_SPACE) rc = ' ';
	  else rc = key->keysym.sym;
    break;
      
  }
  return rc;
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
        eventt.type = ev_keydown;
        eventt.data1 = translatekey(&event.key);
        D_PostEvent(&eventt);
        break;
      case SDL_KEYUP:
        eventt.type = ev_keyup;
        eventt.data1 = translatekey(&event.key);
        D_PostEvent(&eventt);
        break;
    }
  }      
}

static SDL_Color colors[256];
/*
  Sets the color pallete 
*/
void I_SetPalette (byte* palette){
  int	c;
  for(int i = 0; i < 256; i++){
    c = gammatable[usegamma][*palette++]; 
    colors[i].r = (c << 8) + c; 
    c = gammatable[usegamma][*palette++];
		colors[i].g = (c << 8) + c;
    c = gammatable[usegamma][*palette++];
		colors[i].b = (c << 8) + c;
  }
  SDL_SetPalette(display, SDL_LOGPAL|SDL_PHYSPAL, colors, 0, 256);
}

void I_UpdateNoBlit (void){
  
}
void I_FinishUpdate (void){
  // 
  
  int i = 0, x = 0, y = 0, t = 0;
  while(i < SCREENWIDTH*SCREENHEIGHT*2){ //
    if(i % (SCREENWIDTH*2) == 0){
      x = 0;
      y++;
      if(gamestate == GS_LEVEL && y % 2 == 0 && y != 0){
        //i -= (SCREENWIDTH*2);
      }
    }else{
      x++;
    }
    Uint8 *p = ((Uint8 *) display->pixels+ y * display->pitch+ x * display->format->BytesPerPixel);
    
    *(Uint32 *)p = screens[0][i++];
  }
  printf("x,y,i = %d,%d,%d\n",)
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