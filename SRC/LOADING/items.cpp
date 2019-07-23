#include "../common.h"

uint8_t * read_tr4_item(tr4_level_load * level, uint8_t * srcPtr, tr2_item_t & item)
{
    uint8_t * src = srcPtr;
    item.object_id = *(int16_t*)src; src+=2; //read_bit16(src);
    item.room = *(int16_t*)src; src+=2;        //read_bit16(src);
    src=read_tr_vertex32(src, item.pos);
    item.rotation = *(uint16_t*)(src); /*/ 16384.0f * -90);*/ src+=2;
    item.intensity1 = *(uint16_t*)src; src+=2;// read_bitu16(src);
    item.intensity2 = item.intensity1;
    item.ocb = *(uint16_t*)src; src+=2;  //read_bitu16(src);
    item.flags = *(uint16_t*)src; src+=2; //read_bitu16(src);

    return src;
}
