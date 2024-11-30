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

static int frameBufferFd = -1;
static uint8_t* frameBuffer = 0;
const int screenWidth = 800;
const int screenHeight = 480;

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