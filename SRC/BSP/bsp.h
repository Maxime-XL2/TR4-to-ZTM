
typedef struct
{
    double position[3];
    float  textCoord[2];
    float  light[3];
    float  normal[3];
} bsp_vert_t;


typedef struct
{
    bool            usedToSplit = 0;
    uint32_t        nbPoints;
    uint32_t        nbIndices;

    bsp_vert_t *    verts;
    uint32_t *      indices;
    uint32_t        textureId;
    float           normal[3];
} bsp_polygon_t;



