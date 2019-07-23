#ifndef _COMMON_H_

#define _COMMON_H_
using namespace std;
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <windows.h>
#include <gl/gl.h>
#include <GL/glu.h>


#include "TINF/tinf.h"
#include <GL/glu.h>
#include <zlib.h>

typedef	    int16_t 	ANGLE ;
#define	    DEGtoANG(d)		((ANGLE)((65536.0 * (d)) / 360.0))
#define     RADtoANG(d)     ((ANGLE)((65536.0 * (d)) / (2*M_PI)))
#define     ANGtoRAD(d)     ((float)(((d)/65536.0) * M_PI/180.0f))
#define     toFIXED(d)      ((int32_t)(d*65536.0))

enum
{
    X=0,
    Y=1,
    Z=2,
    XY=2,
    XYZ=3,
    XYZS=4
} ;

typedef enum
{
	LEFT_PLANE = 0,
	RIGHT_PLANE = 1,
	TOP_PLANE = 2,
	BOTTOM_PLANE = 3,
	FAR_PLANE = 4,
	NEAR_PLANE = 5,
} FRUSTUM_PLANE;

const float scaleF = 1;//0.25f;
const float TR_METERING_SECTORSIZE = 1024.0 * scaleF;



#include "tr4_types.h"
float DotProduct(float * vct1, float * vct2);

#include "PREVIEW/ZTM_DEF.h"
#include "LOADING/LoadTR4.h"
#include "CONVERT_2_ZTM/converter.h"


#endif // _COMMON_H_
