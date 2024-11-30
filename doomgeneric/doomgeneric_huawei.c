//doomgeneric for Huawei

#include "doomkeys.h"
#include "m_argv.h"
#include "doomgeneric.h"

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <linux/fb.h>
#include <linux/input.h>

static int frameBufferFd = -1;
static uint8_t* frameBuffer = 0;
const int screenWidth = 800;
const int screenHeight = 480;
static int eventFd;

void DG_Init()
{
    frameBufferFd = open("/dev/fb0", O_RDWR);

    if (frameBufferFd == 0){
        printf("Opening FrameBuffer device failed!\n");
        exit(1);
    }
    frameBuffer = mmap(NULL, screenWidth * screenHeight * 3, PROT_READ | PROT_WRITE, MAP_SHARED, frameBufferFd, 0);

    if (frameBuffer == MAP_FAILED)
    {
        printf("FrameBuffer mmap failed\n");
        exit(1);
    }

    eventFd = open("/dev/input/event0", O_RDONLY);
    if (fd == -1) {
        perror("Cannot open input device");
        exit(1);
    }

    int flags = fcntl(eventFd, F_GETFL, 0);
    fcntl(eventFd, F_SETFL, flags | O_NONBLOCK);
}

void DG_DrawFrame()
{
    memcpy(frameBuffer, DG_ScreenBuffer, DOOMGENERIC_RESX * DOOMGENERIC_RESY * 3);
}

void DG_SleepMs(uint32_t ms)
{
    usleep (ms * 1000);
}

uint32_t DG_GetTicksMs()
{
    struct timeval  tp;
    struct timezone tzp;

    gettimeofday(&tp, &tzp);

    return (tp.tv_sec * 1000) + (tp.tv_usec / 1000); /* return milliseconds */
}

int DG_GetKey(int* pressed, unsigned char* doomKey)
{
    ssize_t bytes = read(eventFd, &ev, sizeof(struct input_event));
    struct input_event ev;
    
    if (bytes == sizeof(struct input_event)) {
        if (ev.type == EV_KEY) {
            if(ev.code == 12) doomKey = KEY_FIRE, pressed = ev.value, return 1;
            if(ev.code == 15) doomKey = KEY_USE, pressed = ev.value, return 1;
            if(ev.code == 28) doomKey = KEY_ESCAPE, pressed = ev.value, return 1;
            if(ev.code == 20) doomKey = KEY_ENTER, pressed = ev.value, return 1;
            if(ev.code == 16) doomKey = KEY_UP, pressed = ev.value, return 1;
            if(ev.code == 17) doomKey = KEY_DOWN, pressed = ev.value, return 1;
            if(ev.code == 18) doomKey = KEY_LEFT, pressed = ev.value, return 1;
            if(ev.code == 19) doomKey = KEY_RIGHT, pressed = ev.value, return 1;
        }
    } 
    return 0;
}   

void DG_SetWindowTitle(const char * title)
{
}

int main(int argc, char **argv)
{
    doomgeneric_Create(argc, argv);

    int i;
    for (i = 0; ; i++) {
        doomgeneric_Tick();
    }
    
    return 0;
}