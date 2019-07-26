#include "SRC/common.h"

img_t img[64];


LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);
void initOpenGL(void);


int ZT_FRAMERATE = 1;
int DisplayTEST=0;


/**CONVERTER STUFF**/
uint16_t          swap_endian_ushort(uint16_t value)  { return ((((value) >> 8) & 0xff) | (((value) & 0xff) << 8));}
int16_t           swap_endian_sshort(int16_t  value)  { return (((value >> 8)&0xFF) | ((value & 0xFF) << 8));}
uint32_t          swap_endian_uint  (uint32_t value)  { return ((((value) & 0xff000000) >> 24) | (((value) & 0x00ff0000) >>  8) | (((value) & 0x0000ff00) <<  8) | (((value) & 0x000000ff) << 24)); }
int32_t           swap_endian_sint  (int32_t  value)  { return ((((value) & 0xff000000) >> 24) | (((value) & 0x00ff0000) >>  8) | (((value) & 0x0000ff00) <<  8) | (((value) & 0x000000ff) << 24)); }
float DotProduct(float * vct1, float * vct2){return (vct1[X]*vct2[X] + vct1[Y]*vct2[Y] + vct1[Z]*vct2[Z]);}

#define R_to_24(v) ((v&0x1f)*8)
#define G_to_24(v) (((v>>5)&0x1f)*8)
#define B_to_24(v) (((v>>10)&0x1f)*8)
#define C_RGB_to_24(R,G,B) ((R)| ((G)<<8) | ((B)<<16))
unsigned int texId[1280];
unsigned int SaturnTextId[1280];
unsigned int textMaxSize;

unsigned int totTextures=0;
typedef struct
{
    uint16_t id;
    uint8_t textileId;
    uint8_t minX, minY;
    uint8_t maxX, maxY;
} textInfo_t;
textInfo_t textInfo[4000];

uint32_t textRef[4000]={0};

unsigned int checkDuplicatesTextures(uint8_t minX, uint8_t maxX, uint8_t minY, uint8_t maxY, uint32_t textileId, uint32_t refId)
{
    for (unsigned int i=0; i<totTextures; i++)
    {
        if (textInfo[i].textileId != textileId) continue;
        if (textInfo[i].maxX != maxX) continue;
        if (textInfo[i].maxY != maxY) continue;
        if (textInfo[i].minX != minX) continue;
        if (textInfo[i].minY != minY) continue;
        textRef[refId]=i;
        return 1;
    }
    textInfo[totTextures].id=totTextures;
    textInfo[totTextures].textileId=textileId;
    textInfo[totTextures].maxX=maxX;
    textInfo[totTextures].maxY=maxY;
    textInfo[totTextures].minX=minX;
    textInfo[totTextures].minY=minY;
    textRef[refId]=totTextures;
    totTextures++;

    return 0;
}

int setTextures(tr4_level_load * lvl)
{
    textArray = new texture_t[1024];

    /***/
    uint8_t ARRAY[level.object_textures_count]={0};
    for (unsigned int i=0; i<level.numRooms; i++) {
        for (unsigned int ii=0; ii<level.Rooms[i].num_rectangles; ii++) {
            ARRAY[level.Rooms[i].rectangles[ii].texture&0x7FFF]=1; }
            for (unsigned int ii=0; ii<level.Rooms[i].num_triangles; ii++) {
                ARRAY[level.Rooms[i].triangles[ii].texture&0x7FFF]=1; }
    }

    textMaxSize=0;
    totTextures=0;
    for (unsigned int i=0; i<level.object_textures_count; i++)
    {
        if (!ARRAY[i]) continue;
        tr4_object_texture_t * t = &level.object_textures[i];

        uint8_t minX=255, minY=255, maxX=0, maxY=0;

        for (unsigned int j=0; j<4; j++)
            {
                if (t->vertices[j].xpixel > maxX) maxX=t->vertices[j].xpixel;
                if (t->vertices[j].ypixel > maxY) maxY=t->vertices[j].ypixel;
                if (t->vertices[j].xpixel < minX) minX=t->vertices[j].xpixel;
                if (t->vertices[j].ypixel < minY) minY=t->vertices[j].ypixel;
            }
        while (minX%8!=0) --minX;
        while (minY%8!=0) --minY;
        while ((maxX+1)%8!=0) ++maxX;
        while ((maxY+1)%8!=0) ++maxY;
        uint16_t sizeX= (maxX-minX+1), sizeY=(maxY-minY+1);


        unsigned int cnt=0;



        if (checkDuplicatesTextures(minX, maxX, minY, maxY, t->tile_and_flag&0x7fff, i)) continue;
textMaxSize+=(sizeX*sizeY)/2;
        GLubyte imgData[(sizeX*sizeY*4)]={0};
        cout << "Texture " << (int)(totTextures-1) << " ; x:" << (int)minX << " size : " << (int)sizeX << ", y:"
        << (int)minY << ", size : "  << (int)sizeY << "\n";

        for (uint16_t ii=minY+sizeY; ii>minY; ii--)
        {
            for (uint16_t j=minX; j<minX+sizeX; j++)
            {
                imgData[cnt] = (lvl->Textile32[t->tile_and_flag&0x7fff].pixels[ii][j])&255;
                imgData[cnt+1] = (lvl->Textile32[t->tile_and_flag&0x7fff].pixels[ii][j]>>8)&255;
                imgData[cnt+2] = (lvl->Textile32[t->tile_and_flag&0x7fff].pixels[ii][j]>>16)&255;
                imgData[cnt+3] = 255;
                cnt+=4;
            }
        }

        glGenTextures(1, &SaturnTextId[totTextures-1]);                                // Generate OpenGL texture IDs
        glBindTexture(GL_TEXTURE_2D, SaturnTextId[totTextures-1]);
        //totTextures++;
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, sizeX, sizeY, GL_RGBA, GL_UNSIGNED_BYTE, (void*)imgData);
    }
    /*******/


/*
    for (unsigned int i=0; i<(unsigned int)lvl->TotalTextile32-lvl->NumMiscTextiles; i++)
    {
        unsigned int offsetX=0,offsetY=0;
        for (offsetY=0; offsetY<256; offsetY+=64)
        {
            for (offsetX=0; offsetX<256; offsetX+=64)
            {
                glGenTextures(1, &SaturnTextId[totTextures]);                                // Generate OpenGL texture IDs
                glBindTexture(GL_TEXTURE_2D, SaturnTextId[totTextures]);
                GLubyte imgData[64*64*4]={0};
                unsigned int cnt=0;
                textRef[totTextures]=totTextures; //Just a small test
                totTextures++;
                for (unsigned int ii=offsetY; ii<offsetY+64; ii++)
                {
                    for (unsigned int j=offsetX; j<offsetX+64; j++)
                    {
                        imgData[cnt] = (lvl->Textile32[i].pixels[ii][j])&255;
                        imgData[cnt+1] = (lvl->Textile32[i].pixels[ii][j]>>8)&255;
                        imgData[cnt+2] = (lvl->Textile32[i].pixels[ii][j]>>16)&255;
                        imgData[cnt+3] = 255;
                        cnt+=4;
                    }
                }

                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
                gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, 64, 64, GL_RGBA, GL_UNSIGNED_BYTE, (void*)imgData);
            }
        }
    }*/
    /***********/





    for (unsigned int i=0; i<(unsigned int)lvl->TotalTextile32-lvl->NumMiscTextiles; i++)
    {
        //cout << "\ni=" << i << "\n";
        glGenTextures(1, &texId[i]);                                // Generate OpenGL texture IDs
        glBindTexture(GL_TEXTURE_2D, texId[i]);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        GLubyte imgData[256*256*4]={0};
        unsigned int cnt=0;

        for (unsigned int ii=0; ii<256; ii++)
        {
            for (unsigned int j=0; j<256; j++)
            {
                imgData[cnt++] = (lvl->Textile32[i].pixels[ii][j])&255;
                imgData[cnt++] = (lvl->Textile32[i].pixels[ii][j]>>8)&255;
                imgData[cnt++] = (lvl->Textile32[i].pixels[ii][j]>>16)&255;
                imgData[cnt++] = 255;

               /* imgData[cnt] = (uint32_t)lvl->Textile32[i].pixels[ii][j];
                cnt+=4;*/

            }
                /*imgData[ii]= R_to_24(lvl->Textile32[i].pixel[] [ii]);
                imgData[ii+1]= G_to_24(lvl->Textile32[i].Tile[ii]);
                imgData[ii+2]= B_to_24(lvl->Textile32[i].Tile[ii]);
                imgData[ii+3]= 255;*/

        }
        /*
        int j=0;
        for (int ii=0; ii<img[0].width*img[0].height*4; ii+=4)
        {

            if (img[0].data[j]==0)
            {
                imgData[ii]= 0;
                imgData[ii+1]= 0;
                imgData[ii+2]= 0;
                imgData[ii+3]= 0;
            }
            else
            {
                imgData[ii]= R_to_24(img[0].data[j]);
                imgData[ii+1]= G_to_24(img[0].data[j]);
                imgData[ii+2]= B_to_24(img[0].data[j]);
                imgData[ii+3]= 255;
            }

            j++;

        }*/

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, 256, 256, GL_RGBA, GL_UNSIGNED_BYTE, (void*)imgData);
        //gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, 256, 256, GL_RGBA, GL_UNSIGNED_BYTE, (void*)imgData);
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)imgData);
    }

    return textMaxSize;
}



float returnDistance(float * pos1, float * pos2)
{
    float v[3];
    for (int i=0; i<3; i++) {
        v[i]=pos1[i]-pos2[i];
        v[i]=v[i]*v[i];
    }
    return sqrtf(v[0]+v[1]+v[2]);
}


const float bright = 0.2;

float textCoord[4][2] =
{
    {0,     1},
    {1,     1},
    {1,     0},
    {0,     0},
};

bool SaturnTest=1;

void drawTR4Quad(float * position, tr5_room_t * room, tr4_face4_t * face)
{
    tr5_room_vertex_t * vertex;

    /*
    It doesn't contain normals, so the backface culling has to be done after calculations
    int outside=0;
    for (unsigned int j = 0; j < 4; j++)
    {
        vertex = &room->vertices[face->vertices[j]];
        float d=DotProduct((float*)&vertex->vertex, (float*)&vertex->normal);
        cout << toFIXED(vertex->normal.x) << ", " << toFIXED(vertex->normal.y) << ", " << toFIXED(vertex->normal.z) << "\n";
        if (DotProduct(position,(float*)&vertex->normal)+ d < 0) outside++;
    }
    if (outside>=3) return;*/
    /***TEST***/

    if (SaturnTest)
        glBindTexture(GL_TEXTURE_2D, SaturnTextId[textRef[face->texture&0x7FFF]]);
    else
        glBindTexture(GL_TEXTURE_2D, texId[level.object_textures[face->texture&0x7FFF].tile_and_flag&0x7fff]);

   glBegin(GL_QUADS);
   {
        for (unsigned int j = 0; j < 4; j++)
        {
            if (SaturnTest)
                glTexCoord2f(textCoord[j][0], textCoord[j][1]);
            else
                glTexCoord2f((level.object_textures[face->texture&0x7FFF].vertices[j].xpixel)*1.0/255.0 ,
                (level.object_textures[face->texture&0x7FFF].vertices[j].ypixel)*1.0/255.0) ;

            vertex = &room->vertices[face->vertices[j]];
            glColor4f(vertex->colour.r+bright,vertex->colour.g+bright,vertex->colour.b+bright,vertex->colour.a);
            glVertex3f(vertex->vertex.x,vertex->vertex.y, vertex->vertex.z);


        }
   }
   glEnd();
}


float CUBEverts[][3]=
{
    {-15.0, -15.0, 15.0},
    {15.0, -15.0, 15.0},
    {15.0, 15.0, 15.0},
    {-15.0, 15.0, 15.0},
    {-15.0, -15.0, -15.0},
    {15.0, -15.0, -15.0},
    {15.0, 15.0, -15.0},
    {-15.0, 15.0, -15.0}
};


int16_t CUBEindices[] = {
	0, 1, 2, 3,
	4, 0, 3, 7,
	5, 4, 7, 6,
	1, 5, 6, 2,
	4, 5, 1, 0,
	3, 2, 6, 7
};

void drawCube(vector3 position)
{
    glPushMatrix();
    glTranslatef(position.x,position.y,position.z);
    for (int cube=0; cube<6; cube++)
    {
        glBegin(GL_QUADS);
        for (int cubeV=0; cubeV<4; cubeV++)
        {
            glColor4f(1,1,1,0.75);

            glVertex3f(CUBEverts[CUBEindices[cube*4 +cubeV]][X]*15,
                        CUBEverts[CUBEindices[cube*4 +cubeV]][Y]*15,
                        CUBEverts[CUBEindices[cube*4 +cubeV]][Z]*15);
        }
        glEnd();
    }
    glPopMatrix();
}

int bulletCollision(vector3 position, vector3 direction, int16_t startRoom);
void shootBullet(player_t * player)
{
    vector3 projDir;
    projDir.x= std::sin(player[0].ROTATION[Z]*conv);
    projDir.y= std::sin(player[0].ROTATION[X]*conv);
    projDir.z= std::cos(player[0].ROTATION[Z]*conv);
    cout << "ProjDir : " << (int)(projDir.x*65536) << ", " << (int)(projDir.z*65536) << "\n";
    bulletCollision(player->POSITION,projDir, player->roomId);

//    drawCube(projDir);
}




/***
Shitty function to test portal rendering.
Spoilers : It doesn't work
***/
int userClip[4]={0, 0, 1280, 960};
void recursiveDraw(player_t * player, int16_t roomID)
{
    tr5_room_t * cRoom = &level.Rooms[roomID];
    glPushMatrix();
    glTranslatef(cRoom->offset.x*scaleF, 0, cRoom->offset.z*scaleF);
    tr5_room_vertex_t * vertex;
    for (uint32_t ii=0; ii<cRoom->num_rectangles; ii++)
    {
        tr4_face4_t *face= &cRoom->rectangles[ii];
        drawTR4Quad((float*)&player[0].POSITION, cRoom, face);
    }

    for (uint32_t ii=0; ii<cRoom->num_triangles; ii++)
    {
        tr4_face3_t * face= &cRoom->triangles[ii];
        glBindTexture(GL_TEXTURE_2D, texId[level.object_textures[face->texture&0x7FFF].tile_and_flag&0x7Fff]);
        glBegin(GL_TRIANGLES);
        {
            for (unsigned int j = 0; j < 3; j++)
            {
                glTexCoord2f((level.object_textures[face->texture&0x7FFF].vertices[j].xpixel)*1.0/255.0,
                    (level.object_textures[face->texture&0x7FFF].vertices[j].ypixel)*1.0/255.0);//,  0.0f, 1.0f);
                vertex = &cRoom->vertices[face->vertices[j]];
                glColor4f(vertex->colour.r+bright,vertex->colour.g+bright,vertex->colour.b+bright,vertex->colour.a);
                glVertex3f(vertex->vertex.x,vertex->vertex.y, vertex->vertex.z);
            }
        }
        glEnd();
    }
    glPopMatrix();




    /************TESTING********/
cout << "Num portals : " << cRoom->num_portals << "\n";
    ///Portal clipping poor attempt

    for (uint32_t i = 0; i<cRoom->num_portals; i++)
    {
       tr_room_portal_t * portal = &cRoom->portals[i];
       if (portal->adjoining_room == player[0].roomId) continue; //Prevents nasty crashes because of infinite loops, but I will need to add a counter flag

       vector3 pV = {portal->vertices[0].x+cRoom->offset.x, portal->vertices[0].y, portal->vertices[0].z+cRoom->offset.z};
       float d = DotProduct((float*)&pV, (float*)&portal->normal);
       if (DotProduct((float*)&player->POSITION, (float*)&portal->normal)-d < 0) //Problem here when you cross the portal, easy fix by keeping track of if you already rendered it
       {
           cout << "D =" << (int)d << "\n";
           continue;
       }


        double proj[16];
        double modl[16];
      //  double clip[16];
        GLint view[4];


    glGetDoublev( GL_PROJECTION_MATRIX, proj );
    glGetDoublev( GL_MODELVIEW_MATRIX, modl );
    glGetIntegerv(GL_VIEWPORT, view);

    vector3 screenCoord[4]={0,0,0,0};
    double minV[2]={65536, 65536}, maxV[2]={-65536, -65536};
       for (uint32_t j=0; j<4; j++)
       {
           //Not efficient at all, it's just for testing...
           gluProject(portal->vertices[j].x+cRoom->offset.x,portal->vertices[j].y,portal->vertices[j].z+cRoom->offset.z, modl, proj, view,
                      (double*)&screenCoord[j].x, (double*)&screenCoord[j].y, (double*)&screenCoord[j].z);
            if(screenCoord[j].z<0)continue;
            if (screenCoord[j].x < minV[X]) minV[X]=screenCoord[j].x;
            if (screenCoord[j].x > maxV[X]) maxV[X]=screenCoord[j].x;
            if (screenCoord[j].y < minV[Y]) minV[Y]=screenCoord[j].y;
            if (screenCoord[j].y > maxV[Y]) maxV[Y]=screenCoord[j].y;




       }


       if (screenCoord[0].z<0 && screenCoord[1].z<0 && screenCoord[2].z<0 && screenCoord[3].z<0) continue;
       if (minV[0]<0) minV[0]=0;
       if (minV[1]<0) minV[1]=0;
       if (maxV[0]>=1280 || maxV[0]<0) maxV[0]=1280-1;
       if (maxV[1]>=960 || maxV[1]<0) maxV[1]=960-1;

        cout << (int)minV[0] << ", " << (int)minV[1] << ", " << (int)maxV[0] << ", " << (int)maxV[1] << "\n";
        for (int j=0; j<4; j++)
        cout << (int)screenCoord[j].x << ", " << (int)screenCoord[j].y << ", " << (int)screenCoord[j].z << "\n";

  //      glScissor((int)minV[0],(int)minV[1],(int)maxV[0],(int)maxV[1]);
       recursiveDraw(player, portal->adjoining_room);

       //dist = DotProduct(&PlaneArray[NodeArray[i].Plane].PointOnPlane, &PlaneArray[NodeArray[i].Plane].Normal);
    }

}


player_t player[4];
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{


    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;


    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC; wcex.lpfnWndProc = WindowProc; wcex.cbClsExtra = 0; wcex.cbWndExtra = 0; wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION); wcex.hCursor = LoadCursor(NULL, IDC_ARROW); wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL; wcex.lpszClassName = "SATURN"; wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);


    if (!RegisterClassEx(&wcex))
        return 0;


    hwnd = CreateWindowEx(0,"SATURN","TOMB TO ZTM CONVERTER",WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,1280,780,NULL,NULL,hInstance,NULL);   /* create main window */
    ShowWindow(hwnd, nCmdShow);
    EnableOpenGL(hwnd, &hDC, &hRC);/* enable OpenGL for the window */
    initOpenGL();

    extern resetPlayer(player_t *);
    extern myControls(player_t *);




if (!LOAD_TR4("IN/MAP/tut1.tr4")) return 666;
resetPlayer(&player[0]);

//cout << "Texture size if 4 bpp : " << level.Textile32_UncompSize/8;
/*
cout << "DEBUG OBJECT TEXTURE : \n";
for (uint32_t test=0; test<level.object_textures_count; test++)
{
    //if (level.object_textures[test].y_size ==0) continue;
    cout << level.object_textures[test].x_size << "      ";
    cout << level.object_textures[test].y_size << "      ";
    cout << (level.object_textures[test].tile_and_flag&0x7FFF) << ", "
    << (int)level.object_textures[test].vertices[2].xpixel << ", "
    << (int)level.object_textures[test].vertices[2].ypixel << "\n";
}*/


tr5_room_vertex_t * tmpVertex;
for (unsigned int j=0; j<level.numRooms; j++ )
{
    for (unsigned int jj=0; jj<level.Rooms[j].num_vertices; jj++ )
    {
        tmpVertex=&level.Rooms[j].vertices[jj];
        tmpVertex->vertex.x *= scaleF;
        tmpVertex->vertex.y *= scaleF;
        tmpVertex->vertex.z *= scaleF;

    }
}

    while (!bQuit)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)//
            {
                bQuit = TRUE;
            }
            else {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        if (bQuit) break;


            bQuit = myControls(&player[0]);

            player[0].POSITION.x = player[0].NEXT_POSITION.x;
            player[0].POSITION.y = player[0].NEXT_POSITION.y;
            player[0].POSITION.z = player[0].NEXT_POSITION.z;
            /* OpenGL animation code goes here */
            glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glColor4f(1.0f,1.0f,1.0f,1.0f);

            glMatrixMode(GL_MODELVIEW);

            glPushMatrix();

            glRotatef(player[0].ROTATION[X], 1.0f, 0.0f, 0.0f);
            glRotatef(player[0].ROTATION[Y], 0.0f, 1.0f, 0.0f);
            glTranslatef(-player[0].POSITION.x, -player[0].POSITION.y, -player[0].POSITION.z);

            if (DisplayTEST==0)glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            else  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

            glDisable(GL_CULL_FACE);
            //glEnable(GL_CULL_FACE);
            glEnable(GL_SCISSOR_TEST);
            glScissor(0, 0, 1280, 960);

            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

            /*for (uint32_t i=0; i<level.numRooms; i++)
            {

            }*/
recursiveDraw(&player[0], player[0].roomId);
            //glDisable(GL_BLEND);
            shootBullet(&player[0]);


            glPopMatrix();

            SwapBuffers(hDC);
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            Sleep (ZT_FRAMERATE/60.0 * 1000.0f);
    }

    unsigned int tmp = (textMaxSize + textMaxSize/4 + 1023)/1024;
    cout << "\nTexture size is : " << tmp << "KB...\n";
    cout << "The Saturn Z-Treme engine allows a maximum of 400 KB of textures.\n";
    if (tmp>400) cout << "Your textures take too much memory. Make sure you reduce the total size...\n";

    DisableOpenGL(hwnd, hDC, hRC);/* shutdown OpenGL */
    DestroyWindow(hwnd);/* destroy the window explicitly */

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                break;
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}



