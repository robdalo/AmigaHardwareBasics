#include <exec/types.h>

#include "hardware.h"
#include "interrupts.h"

#define DISABLE_INTERRUPTS 0x7fff
#define POTINP_SET_BIT 0xc000

volatile UWORD *intenar = (volatile UWORD *) INTENAR;
volatile UWORD *potinp = (volatile UWORD *) POTINP;

UWORD *savedInterrupts;

void DisableInterrupts(void)
{
    *savedInterrupts = *intenar;
    *potinp = DISABLE_INTERRUPTS;
}

void RestoreInterrupts(void)
{
    *potinp = (*savedInterrupts | POTINP_SET_BIT);
}
