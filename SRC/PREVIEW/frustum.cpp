#include "../common.h"

plane_t       frustum[6];

bool BoxInFrustum (vector3 bMin, vector3 bMax)
{
    vector3 NearPoint;//,FarPoint;
    plane_t *Plane=frustum;
    for (int i=0;i<4;i++)
    {
        NearPoint.x = Plane->Normal.x < 0.0f ? bMin.x : bMax.x;
        NearPoint.y = Plane->Normal.y < 0.0f ? bMin.y : bMax.y;
        NearPoint.z = Plane->Normal.z < 0.0f ? bMin.z : bMax.z;
    if (DotProduct((float*)&Plane->Normal,(float*)&NearPoint)+Plane->Distance<=0) return false;
    Plane++;
    }
    return true;
}

void ExtractFrustum()
{
    float proj[16];
    float modl[16];
    float clip[16];
    float t;
    /* Get the current PROJECTION matrix from OpenGL */
    glGetFloatv( GL_PROJECTION_MATRIX, proj );
    /* Get the current MODELVIEW matrix from OpenGL */
    glGetFloatv( GL_MODELVIEW_MATRIX, modl );
    /* Combine the two matrices (multiply projection by modelview) */

    //cout << modl[0] << ", " << proj[0] << ", " << modl[1] << ", " << proj[4] << ", " << modl[2] << ", " << proj[8] << ", " << modl[3] << ", " << proj[12] << "\n";

   /* for (int test=0; test<4; test++)
    {
        cout << proj[test*4] << "    " << proj[test*4 + 1] << "    " << proj[test*4+2] << "    " << proj[test*4+3] << "\n";
    }*/

    clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
    clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
    clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
    clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];
    clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
    clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
    clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
    clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];
    clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
    clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
    clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
    clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];
    clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
    clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
    clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
    clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];

    /* Extract the numbers for the RIGHT plane */
    frustum[RIGHT_PLANE].Normal.x = clip[ 3] - clip[ 0];
    frustum[RIGHT_PLANE].Normal.y = clip[ 7] - clip[ 4];
    frustum[RIGHT_PLANE].Normal.z = clip[11] - clip[ 8];
    frustum[RIGHT_PLANE].Distance = clip[15] - clip[12];
    /* Normalize the result */
    t = sqrt( frustum[RIGHT_PLANE].Normal.x * frustum[RIGHT_PLANE].Normal.x + frustum[RIGHT_PLANE].Normal.y * frustum[RIGHT_PLANE].Normal.y + frustum[RIGHT_PLANE].Normal.z * frustum[RIGHT_PLANE].Normal.z);
    if (t==0) t=0.0001;
    frustum[RIGHT_PLANE].Normal.x /= t;
    frustum[RIGHT_PLANE].Normal.y /= t;
    frustum[RIGHT_PLANE].Normal.z /= t;
    frustum[RIGHT_PLANE].Distance /= t;
    /* Extract the numbers for the LEFT plane */
    frustum[LEFT_PLANE].Normal.x = clip[ 3] + clip[ 0];
    frustum[LEFT_PLANE].Normal.y = clip[ 7] + clip[ 4];
    frustum[LEFT_PLANE].Normal.z = clip[11] + clip[ 8];
    frustum[LEFT_PLANE].Distance = clip[15] + clip[12];
    /* Normalize the result */
    t = sqrt( frustum[LEFT_PLANE].Normal.x * frustum[LEFT_PLANE].Normal.x + frustum[LEFT_PLANE].Normal.y * frustum[LEFT_PLANE].Normal.y + frustum[LEFT_PLANE].Normal.z * frustum[LEFT_PLANE].Normal.z);
    if (t==0) t=0.0001;
    frustum[LEFT_PLANE].Normal.x /= t;
    frustum[LEFT_PLANE].Normal.y /= t;
    frustum[LEFT_PLANE].Normal.z /= t;
    frustum[LEFT_PLANE].Distance /= t;
    /* Extract the BOTTOM plane */
    frustum[BOTTOM_PLANE].Normal.x = clip[ 3] + clip[ 1];
    frustum[BOTTOM_PLANE].Normal.y = clip[ 7] + clip[ 5];
    frustum[BOTTOM_PLANE].Normal.z = clip[11] + clip[ 9];
    frustum[BOTTOM_PLANE].Distance = clip[15] + clip[13];
    /* Normalize the result */
    t = sqrt( frustum[BOTTOM_PLANE].Normal.x * frustum[BOTTOM_PLANE].Normal.x + frustum[BOTTOM_PLANE].Normal.y * frustum[BOTTOM_PLANE].Normal.y + frustum[BOTTOM_PLANE].Normal.z * frustum[BOTTOM_PLANE].Normal.z);
    if (t==0) t=0.0001;
    frustum[BOTTOM_PLANE].Normal.x /= t;
    frustum[BOTTOM_PLANE].Normal.y /= t;
    frustum[BOTTOM_PLANE].Normal.z /= t;
    frustum[BOTTOM_PLANE].Distance /= t;
    /* Extract the TOP plane */
    frustum[TOP_PLANE].Normal.x = clip[ 3] - clip[ 1];
    frustum[TOP_PLANE].Normal.y = clip[ 7] - clip[ 5];
    frustum[TOP_PLANE].Normal.z = clip[11] - clip[ 9];
    frustum[TOP_PLANE].Distance = clip[15] - clip[13];
    /* Normalize the result */
    t = sqrt( frustum[TOP_PLANE].Normal.x * frustum[TOP_PLANE].Normal.x + frustum[TOP_PLANE].Normal.y * frustum[TOP_PLANE].Normal.y + frustum[TOP_PLANE].Normal.z * frustum[TOP_PLANE].Normal.z);
    if (t==0) t=0.0001;
    frustum[TOP_PLANE].Normal.x /= t;
    frustum[TOP_PLANE].Normal.y /= t;
    frustum[TOP_PLANE].Normal.z /= t;
    frustum[TOP_PLANE].Distance /= t;
    /* Extract the FAR plane */
    frustum[4].Normal.x = clip[ 3] - clip[ 2];
    frustum[4].Normal.y = clip[ 7] - clip[ 6];
    frustum[4].Normal.z = clip[11] - clip[10];
    frustum[4].Distance = clip[15] - clip[14];
    /* Normalize the result */
    t = sqrt( frustum[4].Normal.x * frustum[4].Normal.x + frustum[4].Normal.y * frustum[4].Normal.y + frustum[4].Normal.z * frustum[4].Normal.z);
    if (t==0) t=0.0001;
    frustum[4].Normal.x /= t;
    frustum[4].Normal.y /= t;
    frustum[4].Normal.z /= t;
    frustum[4].Distance /= t;
    /* Extract the NEAR plane */
    frustum[5].Normal.x = clip[ 3] + clip[ 2];
    frustum[5].Normal.y = clip[ 7] + clip[ 6];
    frustum[5].Normal.z = clip[11] + clip[10];
    frustum[5].Distance = clip[15] + clip[14];

    /* Normalize the result */
    t = sqrt( frustum[5].Normal.x * frustum[5].Normal.x + frustum[5].Normal.y * frustum[5].Normal.y + frustum[5].Normal.z * frustum[5].Normal.z);
    if (t==0) t=0.0001;
    t=1/t;
    frustum[5].Normal.x *= t;
    frustum[5].Normal.y *= t;
    frustum[5].Normal.z *= t;
    frustum[5].Distance *= t;

   // for (int testP=0; testP<6; testP++)
   // cout << testP << ", X: " << frustum[testP].Normal.x << ", Y: " << frustum[testP].Normal.y << ", Z: " << frustum[testP].Normal.z << ", D: " << frustum[testP].Distance  << "\n";
}
