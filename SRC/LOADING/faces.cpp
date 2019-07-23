#include "../common.h"


uint8_t * read_tr4_face4(uint8_t * srcPtr, tr4_face4_t & meshface)
{
    uint8_t * src = srcPtr;
    meshface.vertices[0] = *(uint16_t*)(src); src+=2;
    meshface.vertices[1] = *(uint16_t*)(src); src+=2;
    meshface.vertices[2] = *(uint16_t*)(src); src+=2;
    meshface.vertices[3] = *(uint16_t*)(src); src+=2;
    meshface.texture = *(uint16_t*)(src); src+=2;
    meshface.lighting =0;// *(uint16_t*)(src); src+=2;
    return src;
}

uint8_t * read_tr4_face3(uint8_t * srcPtr, tr4_face3_t & meshface)
{
    uint8_t * src = srcPtr;
    meshface.vertices[0] = *(uint16_t*)(src); src+=2;
    meshface.vertices[1] = *(uint16_t*)(src); src+=2;
    meshface.vertices[2] = *(uint16_t*)(src); src+=2;
    meshface.texture = *(uint16_t*)(src); src+=2;
    meshface.lighting = 0;//*(uint16_t*)(src); src+=2;
    return src;
}
