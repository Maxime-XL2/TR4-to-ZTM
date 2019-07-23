#include "../common.h"

uint8_t * read_tr_room_sprite(uint8_t * const srcPtr, tr_room_sprite_t & room_sprite)
{
    uint8_t * src = srcPtr;
	room_sprite.vertex = *(uint16_t*)(src); src+=2;
	room_sprite.texture = *(uint16_t*)(src); src+=2;
	return src;
}

