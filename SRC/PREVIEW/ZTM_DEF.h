#ifndef ZTM_DEF
#define ZTM_DEF

#include "../common.h"

/**MACROS**/
#define Draw_distance (2048.0f)
#define TOTAL_OBJECTS (4096*2)

extern float tolfac;

#define polygon_tolerance (65)
#define UP_BOUND (65536000.0f)
#define ZT_SQR(a) (a*a)


/** YOU CAN CHANGE PDATA HERE TO SOMETHING ELSE, SUCH AS XPDATA **/


#if 0
#define USE_PALETTE
#define USE_256BNK_ONLY
//#define USE_LOOKUP_TABLE
#endif

/**** IMPORTANT : CHOOSE THE MAP PARTITION METHOD HERE - THE GRID ISN'T SUPPORTED ANYMORE AND WON'T WORK****/
/**CHOOSE 1 ONLY!!!!!!!**/
//#define USE_GRID //Not supported!!
#define USE_BSP
//#define USE_OCTREE

#ifdef USE_OCTREE
    #ifdef USE_BSP
        ERROR! ONLY CHOOSE ONE PARTITION METHOD!
    #endif
#endif
/*********************************************************/
#define NO_COLLISION (1)
//#define USE_QUAKE_MAP_FORMAT  //Uncomment to use Quake map format (the CSG isn't working atm, while the outside polygon removal and the Saturn friendly quads aren't written yet)

typedef     float VECTOR[3];
typedef     float POINT_T[3];
//typedef     float FIXED;

typedef struct
{
    float x;
    float y;
    float z;
} vector3;

typedef struct
{
    float x;
    float y;
    float z;
    float length;
} vector4;

typedef struct
{
    char KEY_TIMER;
    char RESET_VIEW;
    uint16_t ANIM;
} anim_t;


struct player_t
{
    vector3         POSITION;
    vector3         NEXT_POSITION;
    float           ROTATION[3]; //16 bits on Saturn, so aligned with the next 2 chars
    int8_t           PLAYER_ID;
    int8_t           OWNED_WEAPONS;
    float           SPEED[3];
    int16_t        roomId;

    float           MOVEMENT_SPEED;
    float           LATERAL_SPEED;

    long            LEAF;  //Short is way enough...
    anim_t          ANIM;
    int8_t           AMMO[8];
    uint16_t          STATUS;
    uint16_t          LAST_INPUT;
    int8_t           IS_FIRING;
    int8_t           WEAPON;
};


typedef struct
{
    vector3 MaxValue;
    vector3 MinValue;
    vector3 Rotation;
    int     MaskID;
} _BB_Hitbox;

typedef struct {
    POINT_T   pos;
    VECTOR  len;
    /*VECTOR	 norm ;  //Put it vector3s on Saturn to save space

    Sint32  boxMin[3];
    Sint32  boxMax[3];*/

    int8_t   planeType;
    int8_t   dummy;
    int16_t	att;  //2 bytes word on Saturn to save space
} COLLISION; //16 bytes


typedef struct {
	int32_t		 nbCo;
	COLLISION	*cotbl;
}CDATA;

enum
{
    Use_256Bnk_Palette,
    Use_VDP2_Trans,
    Use_16CLUT_Gouraud
};

typedef struct
{
    int32_t              TEXT_SIZE;  //to quickly load from disk
    uint16_t              TOT_TEXT;
    uint16_t              TOT_ANIMATIONS;
    uint16_t              TOTAL_NODES;  //total * sizeof(node)
    uint16_t              TOT_LEAFS;
    uint16_t              TOTAL_MESH;
    uint16_t              TOT_VERTICES;
    uint16_t              TOT_POLYGONS;
    uint16_t              TOT_COLL;
    uint16_t              TOT_PORTALS;
    uint16_t              MUSIC_ID;
    uint16_t              TOT_ENTITIES;
    uint16_t              MAP_STATUS;
    uint16_t              TOT_BRUSHES;
    uint16_t              PORTAL_SIZE;
    int32_t              PVS_SIZE;

    float Length;  //Length of the grid's cubes. Should be 256 or 512
    vector3 Origin; //Origin point used to "center" the collision data and your position with the current grid
} _MAP_DATA;

typedef struct
{
    POINT_T pnt[4];
    VECTOR  norm;
    VECTOR minPt;
    VECTOR maxPt;
} SaturnPortal_t;

#define	IS_RBG0_GROUND_SOLID (1<<0)
#define IS_RBG0_SCROLLING (1<<1)
#define IS_NBG1_SCROLLING (1<<2)

#define KB_A 65
#define KB_D 68
#define KB_W 87
#define KB_S 83
#define KB_Z 90
#define KB_Q 81
#define KB_E 69
#define KB_C 67
#define KB_ESC 0x1B

#define	    GRTBL(r,g,b)	        (((b&0x1f)<<10) | ((g&0x1f)<<5) | (r&0x1f) )


typedef struct
{
    unsigned short width;
    unsigned short height;
    unsigned short palette_id[256*256];  //4 bits per pixel after conversion.
    unsigned short palette[16];
} img_4_bpp;
extern uint16_t Total_4bppImages;

/**temporary stuff quickly made to add entities**/

typedef struct
{
    int16_t  id;
    int16_t  currentNode;
    POINT_T   pos;
    int16_t  ang;
    int16_t  status;
    int32_t  tick;
    void * NEXT;
} entity_database;  //24 bytes each




typedef struct
{
    short           parentID; //Might not be needed
    short           PDATAid; //-1 if none, if >=0 it's a leaf, no need to go further down
    short           child[8]; //-1 if empty
    unsigned int    PVSIndex;
    int32_t          tick; //For the PVS, if tick==current tick then is in PVS
    vector4         bv;  //Center point of the bounding volume + length
    //FIXED           radius;

    int16_t          DUMMY; //Dynamic light ID?
    int16_t          totEntities;
    CDATA * col;
    entity_database ent[50];  //lazy way
    //int32_t nbCol;
    //COLLISON * col;


    vector3         minPt;
    vector3         maxPt;
}  _octree_node;  //44 bytes per node  + 224

typedef struct
{
    unsigned short  textureId;
    unsigned char   colorDepth;
    unsigned char   timer;
    unsigned short  pointer;
    unsigned short  lodLevel;
} animArray_t;

typedef struct
{
    POINT_T Pos;
    VECTOR Normal;
    float   dist;
    long    id;
    float   color[3];
} lightSource_t;

#ifndef VCopy
#define VCopy
inline void VectorCopy(vector3 * src, vector3 * dst)
{
    dst->x=src->x;
    dst->y=src->y;
    dst->z=src->z;
}
#endif



typedef struct
{
    vector3 PointOnPlane; //btw, useless if you use the distance...
    vector3 Normal;
    float   Distance;
} plane_t;



/**Custom Z-Treme polygonal data... work in progress**/
typedef int16_t ZPOINT[4]; //vertex data = 3 Points, 1 color data

typedef struct {
    int8_t   vertices[4];
    uint16_t  texno;
    uint16_t  cmdctrl;
    uint16_t  cmdpmod;
    uint16_t  cmdcolr;
} ZPOLYGON; //12 bytes each

typedef struct {
    uint16_t StartPoint;
    uint16_t EndPoint;
    uint16_t StartPol;
    uint16_t EndPol;
    uint16_t LightID; //A copy to transfer to the slave
    int16_t LightDistance;
} ZPDATA;

typedef struct {
    //uint16_t      VertIndices[4]; //The plane's vertices
    //uint16_t      Flag; //Occlusion? Collision? Transparent?
    //uint16_t      CollisionFlag;  //Kills you? Hurts? Does nothing?
    //uint16_t      Trigger;
    //uint16_t      Target;
    int32_t      ticks;
    VECTOR      norm;
    FIXED       distance;
    VECTOR      minPt; //Bounding box's min point
    VECTOR      maxPt; //Bounding box's max point
    /*int8_t       nbPoint;
    int8_t       nbPolygon;
    int8_t       nbPointLOD;
    int8_t       nbPolygonLOD;*/
    ZPDATA      mesh[2];
    uint16_t      lightDistance;
    int8_t       lightID;
    int8_t       nbCollision;

    /*ZPOINT   *  pntbl;
    ZPOLYGON *  pltbl;
    ZPOLYGON *  pltblLOD;*/
    void     *  collision;  //Probably not even needed anymore
} ZPLANE; //68 bytes



typedef struct
{
	bool bPressed;
	bool bReleased;
	bool bHeld;
} sKeyState;
/***
typedef struct {
    uint16_t      VertIndices[4]; //The plane's vertices
    uint16_t      Flag; //Occlusion? Collision? Transparent?
    uint16_t      CollisionFlag;  //Kills you? Hurts? Does nothing?
    uint16_t      Trigger;
    uint16_t      Target;

    int32_t      ticks;
    VECTOR      norm;
    FIXED       distance;
    VECTOR      minPt; //Bounding box's min point
    VECTOR      maxPt; //Bounding box's max point

    int8_t       nbPoint;
    int8_t       nbPolygon;
    int8_t       nbPointLOD;
    int8_t       nbPolygonLOD;

    uint16_t      lightDistance;
    int8_t       lightID;
    int8_t       nbCollision;

    ZPOINT   *  pntbl;
    ZPOLYGON *  pltbl;
    ZPOLYGON *  pltblLOD;
    void     *  collision;  //Probably not even needed anymore
} ZPDATA; //84 bytes
**/

/**END OF CUSTOM POLYGON FORMAT **/


/*****GLOBALS*******/
extern int ZT_FRAMERATE;
/*extern vPDATA mesh_array[];
extern vPDATA LOD_mesh[];
extern CDATA collision_data[];
extern _octree_node node[]; //Ugly global variable again, I know...
extern POINT_T			GourDef[];
extern int ZT_FRAMERATE;
extern char TextureSpecialAttribute[];
extern _MAP_DATA MAP_DATA;

extern entity_database entData[];
//extern _BB_Hitbox  BSP_BB[TOTAL_OBJECTS] = {0};
extern img_t img[1280];
extern img_4_bpp pimg[1280];
extern unsigned short CRAMpalette[2048];
extern unsigned short CRAMtotal;

extern animArray_t animArray[64];
extern int AlternatePVS;

extern lightSource_t LightArray[2048];
extern unsigned int  NbLights;

extern int Pre_Mipmap;
extern _MAP_DATA MAP_DATA;
extern std::string TextureArray[1280];
extern ATTR TextureAttributeArray[1280];
extern char TextureSpecialAttribute[1280];
extern VECTOR light_source;
extern bool   use_directionnal_light;
extern unsigned short light_color;
extern unsigned int octreeMaxDepthCondition;
extern unsigned int octreePolycountCondition;
extern bool BoxInFrustum (vector3 bMin, vector3 bMax);

extern unsigned int mapTicks;*/

/**HEADERS**/
/*#include "bsp.h"
#include "collision.h"
#include "load.h"
#include "lightning.h"
#include "partition.h"
#include "portal.h"
#include "pvs.h"
#include "rendering.h"
#include "textures.h"
#include "tools.h"
#include "vectors.h"
#include "writeBinary.h"
#include "writeHeader.h"
#include "../portals/portal_rendering.h"
#include "../MAP_LOADING/map.h"*/

/**GAME HEADERS**/
#include "player.h"
#include "collision.h"
#include "controls.h"



/*****FUNCTIONS*****/
void specialConditions(unsigned short startPtr, unsigned short endPtr);




#endif
