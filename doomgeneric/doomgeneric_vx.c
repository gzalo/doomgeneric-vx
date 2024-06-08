#include "doomgeneric.h"
#include "doomkeys.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <svc.H>

/*int system(char* command){
  
}

int strcasecmp(const char *s1, const char *s2) {
    while (*s1 != '\0' && *s2 != '\0') {
        if (tolower((unsigned char)*s1) != tolower((unsigned char)*s2)) {
            return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
        }
        s1++;
        s2++;
    }

    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}*/

int strncasecmp(const char *s1, const char *s2, int n)
{
    if (n && s1 != s2)
    {
        do {
            int d = tolower(*s1) - tolower(*s2);
            if (d || *s1 == '\0' || *s2 == '\0') return d;
            s1++;
            s2++;
        } while (--n);
    }
    return 0;
}

char *strdup(const char *s1) {
  char *str;
  size_t size = strlen(s1) + 1;

  str = malloc(size);
  if (str) {
    memcpy(str, s1, size);
  }
  return str;
}

/*
void _fp_init(){
  
}*/

//static displayInfo_t _display;
//static uint16_t *framebuffer = NULL;
static int console = -1;
static int externalkbd = -1;
static int card = -1;
//static int serial = -1;
static FILE *out;
static FILE *err;
static int frameKeys = 0;
static int prevFrameKeys = 0;
static int frames = 0;

static int externalKeys = 0;
static int prevexternalKeys = 0;

int vxprintf( const char *fmt, ... ){
  va_list argptr;
  va_start(argptr, fmt);  
  int x = vfprintf(out, fmt, argptr);
  va_end(argptr);
  fflush(out);
  return x;
}
int vxprintferr( const char *fmt, ... ){
  va_list argptr;
  va_start(argptr, fmt);  
  int x = vfprintf(err, fmt, argptr);
  va_end(argptr);
  fflush(err);
  return x;
}
int vxfflushout(){
  return fflush(out);
}
int vxfflusherr(){
  return fflush(err);
}
int vxputs ( const char * data){
  fputs(data, out);
  fflush(out);
  return 0;
}
int vxputchar( int data){
  fputc(data, out);
  fflush(out);
  return 0;
}
int vxvprintferr( const char *fmt, va_list args){
  int x = vfprintf(err, fmt, args);
  fflush(err);
  return x;
}

void DG_Init(void){
  set_backlight_level(100);

  console = open(DEV_CONSOLE, 0);
  key_beeps(0);

  //SVC_INFO_DISPLAY_EXT(&_display);
  // framebuffer = (uint16_t *) malloc(_display.width * _display.height * sizeof(uint16_t));

  if (get_display_coordinate_mode() != PIXEL_MODE) {
		set_display_coordinate_mode(PIXEL_MODE);
	}
}

static char buf[500] = "";
static char kbdbuf[32] = "";
static int pressedUp = 0;
static int pressedDown = 0;
static int pressedLeft = 0;
static int pressedRight = 0;
static int pressedFire = 0;
static int pressedEnter = 0;
static int releasedUp = 0;
static int releasedDown = 0;
static int releasedLeft = 0;
static int releasedRight = 0;
static int releasedFire = 0;
static int releasedEnter = 0;

void DG_DrawFrame(void){
  //vxputs("Drawing frame\n");
  display_frame_buffer(0, 0, 240, 320, (short*)DG_ScreenBuffer);

  prevFrameKeys = frameKeys;

  if(card_pending()){
    read(card, buf, sizeof(buf));
    frameKeys |= 128;
  }


  //if(read_evt(EVT_COM1)&EVT_COM1){
    int x = read(externalkbd, kbdbuf, 4);
    if(x > 0){
      prevexternalKeys = externalKeys;
      externalKeys = kbdbuf[x-1];

      // UP LEFT FIRE DOWN RIGHT ENTER
      if(externalKeys & 1 && !(prevexternalKeys & 1)) pressedUp = 1;
      if(externalKeys & 2 && !(prevexternalKeys & 2)) pressedLeft = 1;
      if(externalKeys & 4 && !(prevexternalKeys & 4)) pressedFire = 1;
      if(externalKeys & 8 && !(prevexternalKeys & 8)) pressedDown = 1;
      if(externalKeys & 16 && !(prevexternalKeys & 16)) pressedRight = 1;
      if(externalKeys & 32 && !(prevexternalKeys & 32)) pressedEnter = 1;

      if(!(externalKeys & 1) && (prevexternalKeys & 1)) releasedUp = 1;
      if(!(externalKeys & 2) && (prevexternalKeys & 2)) releasedLeft = 1;
      if(!(externalKeys & 4) && (prevexternalKeys & 4)) releasedFire = 1;
      if(!(externalKeys & 8) && (prevexternalKeys & 8)) releasedDown = 1;
      if(!(externalKeys & 16) && (prevexternalKeys & 16)) releasedRight = 1;
      if(!(externalKeys & 32) && (prevexternalKeys & 32)) releasedEnter = 1;
    }
  //}

  int pending = kbd_pending_count();
  if(pending){
    char key[1];
		read(console, key, 1);
		key[0] &= 0x7f;

    if(key[0] == '2') {
      frameKeys |= 1;
    } else if(key[0] == '4') {
      frameKeys |= 2;
    } else if(key[0] == '8') {
      frameKeys |= 4;
    } else if(key[0] == '6') {
      frameKeys |= 8;
    } else if(key[0] == '5') {
      frameKeys |= 16;
    } else if(key[0] == '3') {
      frameKeys |= 32;
    } else if(key[0] == '1') {
      frameKeys |= 64;
    } else if(key[0] == '9') {
      frameKeys |= 128;
    }
  } else{
    frames++;
    if(frames == 4){
      frames = 0;
      frameKeys = 0;
    }
  }
}
void DG_SleepMs(uint32_t ms){
  SVC_WAIT(ms);
}
uint32_t DG_GetTicksMs(void){
  return read_ticks();
}

int DG_GetKey(int* pressed, unsigned char* key){
  if(frameKeys & 1 && !(prevFrameKeys & 1)){
    *pressed = 1;
    *key = KEY_LEFTARROW;
    prevFrameKeys |= 1;
    return 1;
  }
  if(frameKeys & 2 && !(prevFrameKeys & 2)){
    *pressed = 1;
    *key = KEY_DOWNARROW;
    prevFrameKeys |= 2;
    return 1;
  }
  if(frameKeys & 4 && !(prevFrameKeys & 4)){
    *pressed = 1;
    *key = KEY_RIGHTARROW;
    prevFrameKeys |= 4;
    return 1;
  }
  if(frameKeys & 8 && !(prevFrameKeys & 8)){
    *pressed = 1;
    *key = KEY_UPARROW;
    prevFrameKeys |= 8;
    return 1;
  }
  if(frameKeys & 16 && !(prevFrameKeys & 16)){
    *pressed = 1;
    *key = KEY_ENTER;
    prevFrameKeys |= 16;
    return 1;
  }
  if(frameKeys & 32 && !(prevFrameKeys & 32)){
    *pressed = 1;
    *key = KEY_ESCAPE;
    prevFrameKeys |= 32;
    return 1;
  }
  if(frameKeys & 64 && !(prevFrameKeys & 64)){
    *pressed = 1;
    *key = KEY_FIRE;
    prevFrameKeys |= 64;
    return 1;
  }
  if(frameKeys & 128 && !(prevFrameKeys & 128)){
    *pressed = 1;
    *key = KEY_USE;
    prevFrameKeys |= 128;
    return 1;
  }
  // RELEASE

  if(!(frameKeys & 1) && (prevFrameKeys & 1)){
    *pressed = 0;
    *key = KEY_LEFTARROW;
    prevFrameKeys &= ~1;
    return 1;
  }
  if(!(frameKeys & 2) && (prevFrameKeys & 2)){
    *pressed = 0;
    *key = KEY_DOWNARROW;
    prevFrameKeys &= ~2;
    return 1;
  }
  if(!(frameKeys & 4) && (prevFrameKeys & 4)){
    *pressed = 0;
    *key = KEY_RIGHTARROW;
    prevFrameKeys &= ~4;
    return 1;
  }
  if(!(frameKeys & 8) && (prevFrameKeys & 8)){
    *pressed = 0;
    *key = KEY_UPARROW;
    prevFrameKeys &= ~8;
    return 1;
  }
  if(!(frameKeys & 16) && (prevFrameKeys & 16)){
    *pressed = 0;
    *key = KEY_ENTER;
    prevFrameKeys &= ~16;
    return 1;
  }
  if(!(frameKeys & 32) && (prevFrameKeys & 32)){
    *pressed = 0;
    *key = KEY_ESCAPE;
    prevFrameKeys &= ~32;
    return 1;
  }
  if(!(frameKeys & 64) && (prevFrameKeys & 64)){
    *pressed = 0;
    *key = KEY_FIRE;
    prevFrameKeys &= ~64;
    return 1;
  }
  if(!(frameKeys & 128) && (prevFrameKeys & 128)){
    *pressed = 0;
    *key = KEY_USE;
    prevFrameKeys &= ~128;
    return 1;
  }

  //UP LEFT FIRE DOWN RIGHT

  if(pressedUp){
    *pressed = 1;
    *key = KEY_UPARROW;
    pressedUp = 0;
    return 1;
  }
  if(pressedLeft){
    *pressed = 1;
    *key = KEY_LEFTARROW;
    pressedLeft = 0;
    return 1;
  }
  if(pressedFire){
    *pressed = 1;
    *key = KEY_FIRE;
    pressedFire = 0;
    return 1;
  }
  if(pressedDown){
    *pressed = 1;
    *key = KEY_DOWNARROW;
    pressedDown = 0;
    return 1;
  }
  if(pressedRight){
    *pressed = 1;
    *key = KEY_RIGHTARROW;
    pressedRight = 0;
    return 1;
  }
  if(pressedEnter){
    *pressed = 1;
    *key = KEY_ENTER;
    pressedEnter = 0;
    return 1;
  }

  if(releasedUp){
    *pressed = 0;
    *key = KEY_UPARROW;
    releasedUp = 0;
    return 1;
  }
  if(releasedLeft){
    *pressed = 0;
    *key = KEY_LEFTARROW;
    releasedLeft = 0;
    return 1;
  }
  if(releasedFire){
    *pressed = 0;
    *key = KEY_FIRE;
    releasedFire = 0;
    return 1;
  }
  if(releasedDown){
    *pressed = 0;
    *key = KEY_DOWNARROW;
    releasedDown = 0;
    return 1;
  }
  if(releasedRight){
    *pressed = 0;
    *key = KEY_RIGHTARROW;
    releasedRight = 0;
    return 1;
  }
  if(releasedEnter){
    *pressed = 0;
    *key = KEY_ENTER;
    releasedEnter = 0;
    return 1;
  }
  
  return 0;
}
void DG_SetWindowTitle(const char * title){
}

int main(int argc, char **argv){
    out = fopen("out.log", "w");
    err = fopen("err.log", "w");
    card = open(DEV_CARD, 0);
    externalkbd = open(DEV_COM1, 0);

	  open_block_t xcomblock;
    xcomblock.rate = Rt_9600;
    xcomblock.format = Fmt_A8N1 | Fmt_auto;
	  xcomblock.protocol = P_char_mode;
    xcomblock.parameter = 0;
	  set_opn_blk(externalkbd, &xcomblock);

    if((_SYS_VERSION>=0x301) && (_syslib_version()>=0x301)) {
      //setFree(FREE_TYPE_NO_VALIDATE);
    }

    doomgeneric_Create(argc, argv);

    while (1)
    {
        doomgeneric_Tick();
    }
    
}
