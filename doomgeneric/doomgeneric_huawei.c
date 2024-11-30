//doomgeneric for Huawei

#include "doomkeys.h"
#include "m_argv.h"
#include "doomgeneric.h"

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <sys/ioctl.h>
#include <sys/mman.h>

static int frameBufferFd = -1;
static int* frameBuffer = 0;
const int screenWidth = 800;
const int screenHeight = 480;

void DG_Init()
{
    frameBufferFd = open("/dev/fb0", 0);

    if (frameBufferFd == 0){
        printf("Opening FrameBuffer device failed!\n");
        exit(1);
    }
    frameBuffer = mmap(NULL, screenWidth * screenHeight * 3, PROT_READ | PROT_WRITE, 0, FrameBufferFd, 0);

    if (FrameBuffer == (int*)-1)
    {
        printf("FrameBuffer mmap failed\n");
        exit(1);
    }
}

void DG_DrawFrame()
{
    for (int i = 0; i < DOOMGENERIC_RESY; ++i)
    {
        memcpy(FrameBuffer + s_PositionX + (i + s_PositionY) * s_ScreenWidth, DG_ScreenBuffer + i * DOOMGENERIC_RESX, DOOMGENERIC_RESX * 3);
    }
}

void DG_SleepMs(uint32_t ms)
{
    sleep_ms(ms);
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

    for (int i = 0; ; i++) {
        doomgeneric_Tick();
    }
    
    return 0;
}