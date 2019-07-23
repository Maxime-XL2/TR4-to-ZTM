#include "../common.h"

uint8_t * read_tr4_room_light(uint8_t * const srcPtr, tr5_room_light_t & light)
{
    uint8_t * src = srcPtr;
    src=read_tr_vertex32(src, light.pos);
    src=read_tr_colour(src, light.color);
    light.light_type = *(uint8_t*)src; src+=1;
    light.unknown = *(uint8_t*)src; src+=1;
    light.intensity1 = *(uint8_t*)src; src+=1;
    light.intensity = light.intensity1;
    light.intensity /= 32;
    light.r_inner = *(float*)(src); src+=4;
    light.r_outer = *(float*)(src); src+=4;
    light.length = *(float*)(src); src+=4;
    light.cutoff = *(float*)(src); src+=4;
    src=read_tr4_vertex_float(src, light.dir);

    return src;
}
