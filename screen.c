#include <exec/types.h>
#include <proto/exec.h>
#include <proto/graphics.h>

#include "color.h"
#include "hardware.h"
#include "screen.h"

#define COP_WAIT 0xfffe
#define LINE_START 0x4f07
#define LINE_END 0xca07
#define LINE_HEIGHT 0x0800
#define MIN_LIB_VER 39
#define VPOSR_I0_BIT9 (1 << 9)

volatile UWORD *color00 = (volatile UWORD *) COLOR00;
volatile UWORD *color01 = (volatile UWORD *) COLOR01;
volatile ULONG *cop1lch = (volatile ULONG *) COP1LCH;

struct GfxBase *GfxBase;

static UWORD __chip copperList[] =
{
    // Vertical: 2c to 12c, Horizontal: 07 to df

    FMODE, 0x0000, // 0
    BPLCON0, 0x0200, // 2
    0x2b07, COP_WAIT, // 4
    COLOR00, COLOR_BLUE, // 6
    0x4c07, COP_WAIT, // 8
    COLOR00, COLOR_WHITE, // 10
    0x4e07, COP_WAIT, // 12
    COLOR00, COLOR_BLACK, // 14

    LINE_START, COP_WAIT, // 16
    COLOR00, COLOR_GREY, // 18
    LINE_START + LINE_HEIGHT, COP_WAIT, // 20
    COLOR00, COLOR_BLACK, // 22

    0xdd07, COP_WAIT, // 24
    COLOR00, COLOR_WHITE, // 26
    0xdf07, COP_WAIT, // 28
    COLOR00, COLOR_BLUE, // 30

    0xffff, COP_WAIT // 32
};

UWORD linePosY;
UWORD lineColor;
int linePosYIncrement;
ULONG savedCopperList;
int lineCycleTimer;

void InitScreen(void)
{
    linePosY = LINE_START;
    linePosYIncrement = 1;
    lineColor = COLOR_GREY;
    lineCycleTimer = 0;

    UpdateCopperList();
    SetCopperList();
}

void RestoreScreen(void)
{
    *cop1lch = savedCopperList;
}

void SetCopperList(void)
{
    GfxBase = (struct GfxBase *)(OpenLibrary("graphics.library", MIN_LIB_VER));

    savedCopperList = (ULONG)(GfxBase->copinit);
    *cop1lch = (ULONG)(copperList);

    if (GfxBase)
        CloseLibrary((struct Library *)(GfxBase));
}

void UpdateCopperList(void)
{
    copperList[16] = linePosY;
    copperList[19] = lineColor;
    copperList[20] = linePosY + LINE_HEIGHT;
}

void UpdateLinePosition(void)
{
    if (linePosY >= LINE_END - LINE_HEIGHT)
      linePosYIncrement = -0x0100;
    else if (linePosY <= LINE_START)
      linePosYIncrement = 0x0100;

    lineColor += 0x0001;

    if (lineColor == 0x9999)
        lineColor = 0x0000;

    linePosY += linePosYIncrement;
    lineCycleTimer = 0;
}

void UpdateScreen(void)
{
    // replace with proper timer

    lineCycleTimer++;

    if (lineCycleTimer == 500)
        UpdateLinePosition();

    UpdateCopperList();
}

void WaitEndOfFrame(void)
{
    GfxBase = (struct GfxBase *)(OpenLibrary("graphics.library", MIN_LIB_VER));

    WaitTOF();
    WaitTOF();

    if (GfxBase)
        CloseLibrary((struct Library *)(GfxBase));
}
