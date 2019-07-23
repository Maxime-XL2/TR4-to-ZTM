#include "../common.h"


uint8_t * read_tr4_vertex_float(uint8_t * const srcPtr, tr5_vertex_t & vertex)
{
    uint8_t * src = srcPtr;
    vertex.x =  (*(float*)src); src+=4;
    vertex.y = (*(float*)src); src+=4;
    vertex.z = (*(float*)src); src+=4;
    return src;
}

uint8_t * read_tr_vertex32(uint8_t * const srcPtr, tr5_vertex_t & vertex)
{
	// read vertex and change coordinate system
	uint8_t * src = srcPtr;
	vertex.x =  (float)(*(int32_t*)src); src+=4;
	vertex.y = (float)(*(int32_t*)src); src+=4;
	vertex.z = (float)(*(int32_t*)src); src+=4;
	return src;
}

uint8_t * read_tr_vertex16(uint8_t * const srcPtr, tr5_vertex_t & vertex)
{
    uint8_t * src = srcPtr;
    // read vertex and change coordinate system
    vertex.x = (float)(*(int16_t*)src); src+=2;
    vertex.y = (float)(*(int16_t*)src); src+=2;  //-
    vertex.z = (float)(*(int16_t*)src); src+=2;  //-
    return src;
}

uint8_t * read_tr4_room_vertex(uint8_t * srcPtr, tr5_room_vertex_t & room_vertex)
{
    uint8_t * src = srcPtr;
    room_vertex.vertex.x = (float)(*(int16_t*)src); src+=2;
    room_vertex.vertex.y = (float)(*(int16_t*)src); src+=2;
    room_vertex.vertex.z = (float)(*(int16_t*)src); src+=2;
    // read and make consistent
    room_vertex.lighting1 = (*(uint16_t*)src); src+=2;
    room_vertex.attributes = (*(uint16_t*)src); src+=2;
    room_vertex.lighting2 = (*(uint16_t*)src); src+=2;
    // only in TR5
    room_vertex.normal.x = 0;
    room_vertex.normal.y = 0;
    room_vertex.normal.z = 0;

    room_vertex.colour.r = ((room_vertex.lighting2 & 0x7C00) >> 10  ) / 31.0f;
    room_vertex.colour.g = ((room_vertex.lighting2 & 0x03E0) >> 5   ) / 31.0f;
    room_vertex.colour.b = ((room_vertex.lighting2 & 0x001F)        ) / 31.0f;
    room_vertex.colour.a = 1.0f;

    return src;
}
