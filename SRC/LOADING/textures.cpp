#include "../common.h"

void ReadTextiles32(void * const src, tr4_textile32_t * textile)
{

    uint8_t * ptr = (uint8_t*)src;
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++)
        {
            textile->pixels[i][j]=*(uint32_t*)ptr;

            textile->pixels[i][j] = ((textile->pixels[i][j] & 0xff00ff00) | ((textile->pixels[i][j] & 0x00ff0000) >> 16) | ((textile->pixels[i][j] & 0x000000ff) << 16));
            ptr+=4;
        }
    }
}


uint8_t * read_tr4_object_texture_vert(tr4_level_load * level, uint8_t * srcPtr, tr4_object_texture_vert_t * vert)
{
    //cout << "read_tr4_object_texture_vert   ";
    uint8_t * src = srcPtr;
    vert->xcoordinate =         *(int8_t*)src; src+=1; //read_bit8(src);
    vert->xpixel =              *(uint8_t*)src; src+=1;//read_bitu8(src);
    vert->ycoordinate =         *(int8_t*)src; src+=1; //read_bit8(src);
    vert->ypixel =              *(uint8_t*)src; src+=1;//read_bitu8(src);
    if (vert->xcoordinate == 0)
        vert->xcoordinate = 1;
    if (vert->ycoordinate == 0)
        vert->ycoordinate = 1;

    if (vert->xcoordinate > 1) cout << vert->xcoordinate;
    return src;
}

uint8_t * read_tr4_object_texture(tr4_level_load * level, uint8_t * srcPtr, tr4_object_texture_t * object_texture)
{
    uint8_t * src = srcPtr;
    object_texture->transparency_flags = *(uint16_t*)src; src+=2; //read_bitu16(src);
    object_texture->tile_and_flag = *(uint16_t*)src; src+=2; //read_bitu16(src);
    if ((object_texture->tile_and_flag & 0x7FFF) > 128) cout << "object_texture.tile > 128\n";

    object_texture->flags = *(uint16_t*)src; src+=2; //read_bitu16(src);

    src=read_tr4_object_texture_vert(level, src, &object_texture->vertices[0]);
    src=read_tr4_object_texture_vert(level, src, &object_texture->vertices[1]);
    src=read_tr4_object_texture_vert(level, src, &object_texture->vertices[2]);
    src=read_tr4_object_texture_vert(level, src, &object_texture->vertices[3]);
    object_texture->unknown1 = *(uint32_t*)src; src+=4;//read_bitu32(src);
    object_texture->unknown2 = *(uint32_t*)src; src+=4; //read_bitu32(src);
    object_texture->x_size = *(uint32_t*)src; src+=4; //read_bitu32(src);
    object_texture->y_size = *(uint32_t*)src; src+=4; //read_bitu32(src);

    return src;
}
