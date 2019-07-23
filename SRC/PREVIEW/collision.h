#ifndef _COLLISION_H
#define _COLLISION_H


#define pl_width (50.0f)
#define pl_width2 (25.0f)
#define pl_height (100.0f)
#define pl_height2 (50.0f)
#define CAN_JUMP (1)

#define returnVectorDist(a, b) (checkLightDistance(a,b))


// 1/32 epsilon to keep floating point happy
#define	DIST_EPSILON	(0.03125)

#define SURFACE_CLIP_EPSILON (CLASSIFY_FACTOR)
//#define	SURFACE_CLIP_EPSILON	(0.125)
#define vec3_t vector3
// plane types are used to speed some tests
// 0-2 are axial planes
#define	PLANE_X			0
#define	PLANE_Y			1
#define	PLANE_Z			2
#define	PLANE_NON_AXIAL	3
/*
#define TT_RAY 0
#define TT_SPHERE 1
#define TT_BOX 2
typedef int		clipHandle_t;
typedef struct cplane_s {
	vec3_t	normal;
	float	dist;
	byte	type;			// for fast side tests: 0,1,2 = axial, 3 = nonaxial
	byte	signbits;		// signx + (signy<<1) + (signz<<2), used as lookup during collision
	byte	pad[2];
} cplane_t;
typedef struct {
	bool	allsolid;	// if true, plane is not valid
	bool	startsolid;	// if true, the initial point was in a solid area
	float		fraction;	// time completed, 1.0 = didn't hit anything
	vec3_t		endpos;		// final position
	cplane_t	    plane;		// surface normal at impact, transformed to world space
	int			surfaceFlags;	// surface hit
	int			contents;	// contents on other side of surface hit
	int			entityNum;	// entity the contacted sirface is a part of
} trace_t;
typedef struct {
	vec3_t		start;
	vec3_t		end;
	vec3_t		size[2];	// size of the box being swept through the model
	vec3_t		offsets[8];	// [signbits][x] = either size[0][x] or size[1][x]
	float		maxOffset;	// longest corner length from origin
	vec3_t		extents;	// greatest of abs(size[0]) and abs(size[1])
	vec3_t		bounds[2];	// enclosing box of start and end surrounding by size
	vec3_t		modelOrigin;// origin of the model tracing through
	int			contents;	// ored contents of the model tracing through
	bool	    isPoint;	// optimized case
	trace_t		trace;		// returned from trace call
	//sphere_t	sphere;		// sphere for oriendted capsule collision
} traceWork_t;

struct colData  //Pos + Plane2
{
   vector3 Start;
   vector3 End;
   vector3 Normal;
   vector3 Speed;
   vector3 plane;
   float   dist;
   float   time;
   float   percent;
}; //Just trying stuff

void TraceRay( vector3 * inputStart, vector3 * inputEnd );
void TraceBox( vector3 * inputStart, vector3 * inputEnd, vector3 inputMins, vector3 inputMaxs );
void TraceSphere( vector3 * inputStart, vector3 * inputEnd, float inputRadius );

void Trace( vector3 * inputStart, vector3 * inputEnd );
void CheckNode( int Node, float startFraction, float endFraction, vector3 startPos, vector3 endPos, traceWork_t * tw );
//void CheckNode( int Node, float startFraction, float endFraction, vector3 startPos, vector3 endPos );
//bool RayToWorld(D3DXVECTOR3 *Start,D3DXVECTOR3 *End, player_t * player, colData * col, long Node);
//bool CheckNode(vector3 * startPos, vector3 * endPos, player_t * player, float startFraction, float endFraction, long Node);
int checkColPolygon(player_t * player, LEAF * leaf);*/

#endif // _COLLISION_H
