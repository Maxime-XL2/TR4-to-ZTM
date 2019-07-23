#include "../common.h"

vertex_t * vertexArray=nullptr;
/*
typedef struct tr4_face4_s
{
    uint16_t vertices[4];
    uint16_t texture;
    uint16_t lighting;
} tr4_face4_t;*/


workPolygon_t * workPoly=nullptr;

void initConversion(tr4_level_load * level)//Initialize the conversion using a temporary structure
{
    //if (vertexArray) delete [] vertexArray;
    //vertexArray= (vertex_t*)calloc(65536, sizeof(vertex_t)); //Initialize a huge amount, because why not?
    if (workPoly) delete [] workPoly;

    workPoly = (workPolygon_t*)calloc(65536, sizeof(workPolygon_t));

    for (uint32_t i=0; i<level->numRooms; i++)
    {
        /*for (uint32_t ii=0; ii<level.Rooms[i].num_rectangles; ii++)
        {

        }*/
    }


    /**
                for (uint32_t i=0; i<level.numRooms; i++)
            {
                glPushMatrix();
                glTranslatef(level.Rooms[i].offset.x*scaleF, 0, level.Rooms[i].offset.z*scaleF);
                tr5_room_vertex_t * vertex;
                for (uint32_t ii=0; ii<level.Rooms[i].num_rectangles; ii++)
                {
                    tr4_face4_t *face= &level.Rooms[i].rectangles[ii];
                    drawTR4Quad((float*)&player[0].POSITION, &level.Rooms[i], face);
                }
                for (uint32_t ii=0; ii<level.Rooms[i].num_triangles; ii++)
                {
                    tr4_face3_t * face= &level.Rooms[i].triangles[ii];
                    glBindTexture(GL_TEXTURE_2D, texId[level.object_textures[face->texture&0x7FFF].tile_and_flag&0x7Fff]);
                    glBegin(GL_TRIANGLES);
                    {

                        for (unsigned int j = 0; j < 3; j++)
                        {
                            glTexCoord2f((level.object_textures[face->texture&0x7FFF].vertices[j].xpixel)*1.0/255.0,
                                         (level.object_textures[face->texture&0x7FFF].vertices[j].ypixel)*1.0/255.0);//,  0.0f, 1.0f);
                            vertex = &level.Rooms[i].vertices[face->vertices[j]];
                            glColor4f(vertex->colour.r+bright,vertex->colour.g+bright,vertex->colour.b+bright,vertex->colour.a);
                            glVertex3f(vertex->vertex.x,vertex->vertex.y, vertex->vertex.z);
                        }
                    }
                    glEnd();
                }
                glPopMatrix();
            }
    **/
    //room.vertices = (tr5_room_vertex_t*)calloc(room.num_vertices, sizeof(tr5_room_vertex_t));
}
