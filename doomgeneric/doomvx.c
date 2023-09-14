#include "doomgeneric.h"
#include "doomkeys.h"
#include <SVC.H>

char *strdup(const char *s1) {
  char *str;
  size_t size = strlen(s1) + 1;

  str = malloc(size);
  if (str) {
    memcpy(str, s1, size);
  }
  return str;
}

static displayInfo_t _display;
static uint16_t *framebuffer = NULL;
static int console = -1;

void DG_Init(void){
  set_backlight_level(100);

  console = open(DEV_CONSOLE, 0);
  key_beeps(0);

  SVC_INFO_DISPLAY_EXT(&_display);
  framebuffer = (uint16_t *) malloc(_display.width * _display.height * sizeof(uint16_t));

  if (get_display_coordinate_mode() != PIXEL_MODE) {
		set_display_coordinate_mode(PIXEL_MODE);
	}
}
void DG_DrawFrame(void){
  display_frame_buffer(0, 0, _display.width, _display.height, framebuffer);
}
void DG_SleepMs(uint32_t ms){
  SVC_WAIT(ms);
}
uint32_t DG_GetTicksMs(void){
  return read_ticks();
}

int DG_GetKey(int* pressed, unsigned char* key){
  int pendingKeys = kbd_pending_count();
  if(pendingKeys){
    char key[1];
		read(console, key, 1);
		key[0] &= 0x7f;

    if(key[0] == '2') {
      *pressed = 1;
      *key = KEY_LEFTARROW;
    } else if(key[0] == '4') {
      *pressed = 1;
      *key = KEY_DOWNARROW;
    } else if(key[0] == '8') {
      *pressed = 1;
      *key = KEY_RIGHTARROW;
    } else if(key[0] == '6') {
      *pressed = 1;
      *key = KEY_UPARROW;
    } else if(key[0] == '5') {
      *pressed = 1;
      *key = KEY_ENTER;
    } else if(key[0] == '3') {
      *pressed = 1;
      *key = KEY_ESCAPE;
    } else if(key[0] == '1') {
      *pressed = 1;
      *key = KEY_FIRE;
    } else if(key[0] == '9') {
      *pressed = 1;
      *key = KEY_USE;
    }
  }
}
void DG_SetWindowTitle(const char * title){
}

int main(int argc, char **argv){
    doomgeneric_Create(argc, argv);

    while (1)
    {
        doomgeneric_Tick();
    }
    
}
