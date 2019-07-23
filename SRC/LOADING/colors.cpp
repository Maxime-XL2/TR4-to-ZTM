#include "../common.h"


/** \brief reads rgb colour.
  *
  * Reads three rgb colour components. The read 6-bit values get shifted, so they are 8-bit.
  * The alpha value of tr2_colour_t gets set to 0.
  */
uint8_t * read_tr_colour(uint8_t * const srcPtr, tr2_colour_t & colour)
{
    uint8_t * src = srcPtr;
    // read 6 bit color and change to 8 bit
    colour.r = (*(uint8_t*)src) << 2;
    colour.g = (*(uint8_t*)src) << 2;
    colour.b = (*(uint8_t*)src) << 2;
    colour.a = 0;

    src+=3;
    return src;
}
