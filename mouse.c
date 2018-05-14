#include <exec/types.h>

#include "hardware.h"
#include "mouse.h"

#define PRA_FIR0_BIT6 (1 << 6)

volatile UBYTE *ciaa_pra = (volatile UBYTE *) CIAAPRA;

int LeftMouseDown(void)
{
    return (*ciaa_pra & PRA_FIR0_BIT6);
}
