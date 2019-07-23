typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
    uint16_t rgb; //Saturn 16 bits format
    uint16_t palette_idx; //Palette/Color bank index
} pixel_t;

typedef struct {
    uint8_t	     flag ;
    uint8_t	     sorting ;
    uint16_t	 texno ;
    uint16_t	 atrb ;
    uint16_t	 colno ;
    uint16_t	 gstb ;
    uint16_t	 dir ;
} ATTR ;

typedef struct
{
    unsigned int    width=64;
    unsigned int    height=64;
    unsigned int    colorDepth=16;
    string          name="NONAME";
    unsigned int    textureId=0;
    int             paletteIdx=0;
    pixel_t *       pixel=nullptr;
    uint16_t        clut[16]={0};
    uint8_t         nbFrames=0;
    uint8_t         attributes=0;
    ATTR            SGL_ATTR={0}; //For SGL mainly, can be converted for ZTE format
} texture_t;

typedef struct
{
    float	x;
    float	y;
    float	z;
	float	color[3];
	float   normal[3];
	int32_t	specular;
	float	tu;
	float	tv;
} vertex_t;

typedef struct
{
    vertex_t vertices[4];
    uint16_t texture;
    float normal[3];
    float color[3]; //Shouldn't be used really...
    uint16_t SectorId; //Parent sector
} workPolygon_t;

extern texture_t * textArray;
extern workPolygon_t * workPoly;
