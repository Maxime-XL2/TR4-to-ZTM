#include "../common.h"

texture_t * textArray;

bool checkSimilarColor(uint16_t color1, uint16_t color2, int maxDistance)
{
    float c1[XYZ] = {(float)(color1&0x1F), (float)((color1>>5)&0x1F), (float)((color1>>10)&0x1F)};
    float c2[XYZ] = {(float)(color2&0x1F), (float)((color2>>5)&0x1F), (float)((color2>>10) & 0x1F)};

    float dif[XYZ] = {c1[X]-c2[X], c1[Y]-c2[Y], c1[Z]-c2[Z]};
    float dist = sqrtf(dif[X]*dif[X]+dif[Y]*dif[Y]+dif[Z]*dif[Z]);

    if (dist <= (float)maxDistance)
        return true;
    return false;
}

/**This function fills the current image's palette in a 16 colors palette**/
void FillPalette(texture_t * texture, int compression)
{
    if (compression > 20) {cout << "Texture no. " << texture->textureId <<" has more than 16 colors...can't compress more\n"; return;}
    for (unsigned int i=0; i<16; i++) texture->clut[i]=0x00;
    unsigned short total_palette = 0;

   // uint16_t tmpPalette[texture->width*texture->height]={0};
    //First pass to reserve the first palette entry for transparency, only if transparency is used
    for (unsigned int i=0; i< (unsigned int)(texture->width*texture->height); i++)
    {
        if (texture->pixel[i].rgb==0)
        {
            total_palette=1;
            texture->clut[0]=0;
            break;
        }
    }

    for (unsigned int i=0; i< (unsigned int)(texture->width*texture->height); i++)
    {
        bool duplicate=false;
        for (unsigned short ii=0; ii<total_palette; ii++){
            if (checkSimilarColor(texture->pixel[i].rgb, texture->clut[ii], compression+1))
            {
                duplicate = true;
                texture->pixel[i].palette_idx=ii;
                break;
            }
        }
        if (duplicate == false){
            if (total_palette >= 16)
                {texture->pixel[i].palette_idx=15; total_palette++;}
            else{
                texture->pixel[i].palette_idx=total_palette;
                texture->clut[total_palette]=texture->pixel[i].rgb;
                total_palette++;
            }
        }
    }
    if (total_palette > 16)
    {
        int compr=compression + 1;
        cout << "More than 16 colors...reducing the amount of colors...\n";
        FillPalette(texture, compr);
    }
    else
    {
        texture->colorDepth=1; //4 bpp
        if (compression >= 0)
            cout << "The image contains a total of " << total_palette << " colors after compression.\n";
        else
            cout << "The image contains a total of " << total_palette << " colors.\n";
    }
}
