#include "../common.h"

extern tr4_level_load level;
extern player_t player[];

/*
struct room_sector_s *World_GetRoomSector(int room_id, int x, int y)
{
    if((room_id >= 0) && ((uint32_t)room_id < level.numRooms))
    {
        tr5_room_t * room = level.Rooms[room_id];
        if((x >= 0) && (y >= 0) && (x < room->sectors_x) && (y < room->sectors_y))
        {
            return room->content->sectors + x * room->sectors_y + y;
        }
    }

    return NULL;
}*/

bool checkLine(vector3 start, vector3 end, int16_t roomID)
{
    int	x1,y1,xt1,yt1,x2,y2,xt2,yt2;
	int	x,y;
	int	xdist,ydist,xstep,ystep;
//	int	temp;
	int	partial,delta;
	long	ltemp;
	int	xfrac,yfrac,deltafrac;
	unsigned	value,intercept;

    x1 = (int)start.x; ;		// 1/256 tile precision
	y1 = (int)start.z;// >> UNSIGNEDSHIFT;
	xt1 = x1/1024;// >> 8;
	yt1 = y1/1024;// >> 8;

	x2 = (int)end.x;//plux;
	y2 = (int)end.z;
	xt2 = (int)end.x/1024;
	yt2 = (int)end.z/1024; //player->tiley;

	xdist = abs(xt2-xt1);

	if (xdist > 0)
	{
		if (xt2 > xt1)
		{
			partial = 1024-(x1&0x3ff);
			xstep = 1;
		}
		else
		{
			partial = x1&0x3ff;
			xstep = -1;
		}

		deltafrac = abs(x2-x1);
		delta = y2-y1;
		ltemp = ((long)delta<<10)/deltafrac;
		if (ltemp > 0x7fffl)
			ystep = 0x7fff;
		else if (ltemp < -0x7fffl)
			ystep = -0x7fff;
		else
			ystep = ltemp;
		yfrac = y1 + (((long)ystep*partial) >>10);

		x = xt1+xstep;
		xt2 += xstep;
		do
		{
			y = yfrac>>10;
			yfrac += ystep;

			///value = (unsigned)tilemap[x][y];
			x += xstep;

			if (!value)
				continue;

			if (value<128 || value>256)
				return false;

			//
			// see if the door is open enough
			//
			value &= ~0x80;
			intercept = yfrac-ystep/2;

//			if (intercept>doorposition[value])
//				return false;

		} while (x != xt2);
	}

	ydist = abs(yt2-yt1);

	if (ydist > 0)
	{
		if (yt2 > yt1)
		{
			partial = 1024-(y1&0xff);
			ystep = 1;
		}
		else
		{
			partial = y1&0xff;
			ystep = -1;
		}

		deltafrac = abs(y2-y1);
		delta = x2-x1;
		ltemp = ((long)delta<<8)/deltafrac;
		if (ltemp > 0x7fffl)
			xstep = 0x7fff;
		else if (ltemp < -0x7fffl)
			xstep = -0x7fff;
		else
			xstep = ltemp;
		xfrac = x1 + (((long)xstep*partial) >>8);

		y = yt1 + ystep;
		yt2 += ystep;
		do
		{
			x = xfrac>>8;
			xfrac += xstep;

		//	value = (unsigned)tilemap[x][y];
			y += ystep;

			if (!value)
				continue;

			if (value<128 || value>256)
				return false;

			//
			// see if the door is open enough
			//
			value &= ~0x80;
			intercept = xfrac-xstep/2;

	//		if (intercept>doorposition[value])
	//			return false;
		} while (y != yt2);
	}

	return true;
}
/***

boolean CheckLine (objtype *ob)
{
	int	x1,y1,xt1,yt1,x2,y2,xt2,yt2;
	int	x,y;
	int	xdist,ydist,xstep,ystep;
	int	temp;
	int	partial,delta;
	long	ltemp;
	int	xfrac,yfrac,deltafrac;
	unsigned	value,intercept;

	x1 = ob->x >> UNSIGNEDSHIFT;		// 1/256 tile precision
	y1 = ob->y >> UNSIGNEDSHIFT;
	xt1 = x1 >> 8;
	yt1 = y1 >> 8;

	x2 = plux;
	y2 = pluy;
	xt2 = player->tilex;
	yt2 = player->tiley;


	xdist = abs(xt2-xt1);

	if (xdist > 0)
	{
		if (xt2 > xt1)
		{
			partial = 256-(x1&0xff);
			xstep = 1;
		}
		else
		{
			partial = x1&0xff;
			xstep = -1;
		}

		deltafrac = abs(x2-x1);
		delta = y2-y1;
		ltemp = ((long)delta<<8)/deltafrac;
		if (ltemp > 0x7fffl)
			ystep = 0x7fff;
		else if (ltemp < -0x7fffl)
			ystep = -0x7fff;
		else
			ystep = ltemp;
		yfrac = y1 + (((long)ystep*partial) >>8);

		x = xt1+xstep;
		xt2 += xstep;
		do
		{
			y = yfrac>>8;
			yfrac += ystep;

			value = (unsigned)tilemap[x][y];
			x += xstep;

			if (!value)
				continue;

			if (value<128 || value>256)
				return false;

			//
			// see if the door is open enough
			//
			value &= ~0x80;
			intercept = yfrac-ystep/2;

			if (intercept>doorposition[value])
				return false;

		} while (x != xt2);
	}

	ydist = abs(yt2-yt1);

	if (ydist > 0)
	{
		if (yt2 > yt1)
		{
			partial = 256-(y1&0xff);
			ystep = 1;
		}
		else
		{
			partial = y1&0xff;
			ystep = -1;
		}

		deltafrac = abs(y2-y1);
		delta = x2-x1;
		ltemp = ((long)delta<<8)/deltafrac;
		if (ltemp > 0x7fffl)
			xstep = 0x7fff;
		else if (ltemp < -0x7fffl)
			xstep = -0x7fff;
		else
			xstep = ltemp;
		xfrac = x1 + (((long)xstep*partial) >>8);

		y = yt1 + ystep;
		yt2 += ystep;
		do
		{
			x = xfrac>>8;
			xfrac += xstep;

			value = (unsigned)tilemap[x][y];
			y += ystep;

			if (!value)
				continue;

			if (value<128 || value>256)
				return false;

			//
			// see if the door is open enough
			//
			value &= ~0x80;
			intercept = xfrac-xstep/2;

			if (intercept>doorposition[value])
				return false;
		} while (y != yt2);
	}

	return true;
}
***/


int16_t findRoomByPos(float pos[3])
{
    //const float z_margin = TR_METERING_SECTORSIZE / 2.0f;
    int16_t bestRes=-1;
//    int newPos[3];// = {pos[X]/(1024*scaleF), pos[Y], pos[Z]/(1024*scaleF)};
    tr5_room_t * r;

    for(uint32_t i = 0; i < level.numRooms; i++, r++)
    {
        r = &level.Rooms[i];
        //Currently no idea what that real_room is...
        if (pos[X] < r->offset.x+1024) continue;
        if (pos[Z] < r->offset.z+1024) continue;

        if (pos[X] > r->offset.x + (r->num_xsectors*TR_METERING_SECTORSIZE*scaleF)) continue;
        if (pos[Z] > r->offset.z + (r->num_zsectors*TR_METERING_SECTORSIZE*scaleF)) continue;

        if (pos[Y] < r->y_top) continue;
        if (pos[Y] > r->y_bottom) continue;

        //if (((pos[X]-r->room_x)/1024) < r->num_xsectors)
            bestRes=i;
       /* if((r == r->real_room) &&
           (pos[0] >= r->bb_min[0]) && (pos[0] < r->bb_max[0]) &&
           (pos[1] >= r->bb_min[1]) && (pos[1] < r->bb_max[1]) &&
           (pos[2] >= r->bb_min[2] - z_margin) && (pos[2] < r->bb_max[2]))
        {
            room_sector_p orig_sector = Room_GetSectorRaw(r->real_room, pos);
            if(orig_sector && orig_sector->portal_to_room)
            {
                return orig_sector->portal_to_room->real_room;
            }
            return r->real_room;
        }*/
    }

    cout << "Player 1's room " << player[0].roomId << ", bestRes : " << bestRes << "\n";
    return bestRes;
}

/*
int16_t FindRoomByPosCogerrence(float pos[3], struct room_s *old_room)
{
    if(old_room == NULL)
    {
        return World_FindRoomByPos(pos);
    }

    old_room = old_room->real_room;
    room_sector_p orig_sector = Room_GetSectorRaw(old_room, pos);
    if(orig_sector && orig_sector->portal_to_room)
    {
        return orig_sector->portal_to_room->real_room;
    }

    if(orig_sector &&
       (pos[0] >= old_room->bb_min[0]) && (pos[0] < old_room->bb_max[0]) &&
       (pos[1] >= old_room->bb_min[1]) && (pos[1] < old_room->bb_max[1]))
    {
        if(orig_sector->room_below && (pos[2] < orig_sector->room_below->bb_max[2]))
        {
            return orig_sector->room_below->real_room;
        }
        else if((pos[2] >= old_room->bb_min[2]) && (pos[2] < old_room->bb_max[2]))
        {
            return old_room;
        }
        else if(orig_sector->room_above && (pos[2] >= orig_sector->room_above->bb_min[2]))
        {
            return orig_sector->room_above->real_room;
        }
    }

    const float z_margin = TR_METERING_SECTORSIZE / 2.0f;
    for(uint16_t i = 0; i < old_room->content->near_room_list_size; i++)
    {
        room_p r = old_room->content->near_room_list[i]->real_room;
        if((pos[0] >= r->bb_min[0]) && (pos[0] < r->bb_max[0]) &&
           (pos[1] >= r->bb_min[1]) && (pos[1] < r->bb_max[1]) &&
           (pos[2] >= r->bb_min[2] - z_margin) && (pos[2] < r->bb_max[2]))
        {
            return r;
        }
    }

    return World_FindRoomByPos(pos);
}*/
/*
int trace(float * startPos, float * endPos, int16_t roomId)
{
    tr5_room_t * cRoom = level.Rooms[roomId];

    level.Rooms[roomId].secto
}*/



/*
struct room_sector_s *Room_GetSectorXYZ(struct room_s *room, float pos[3])
{
    room_sector_p ret = NULL;
    int x = (int)(pos[0] - room->transform[12 + 0]) / TR_METERING_SECTORSIZE;
    int y = (int)(pos[1] - room->transform[12 + 1]) / TR_METERING_SECTORSIZE;

    room = room->real_room;

    if(x < 0 || x >= room->sectors_x || y < 0 || y >= room->sectors_y)
    {
        return NULL;
    }
    //
     // column index system
     // X - column number, Y - string number
     //
    ret = room->content->sectors + x * room->sectors_y + y;

    //
    //resolve Z overlapped neighboard rooms. room below has more priority.
     //
    if(ret->room_below && (pos[2] < ret->floor))
    {
        ret = Room_GetSectorRaw(ret->room_below->real_room, ret->pos);
    }

    if(ret->room_above && (pos[2] > ret->ceiling))
    {
        ret = Room_GetSectorRaw(ret->room_above->real_room, ret->pos);
    }

    return ret;
}*/
typedef struct
{
    vector3 origin;
    vector3 destination;
    float width; //Your player's body width
    float minY; //Distance to ground
    float maxY; //To "bump" your head
    float distHit;
    int16_t roomId;
    void * target;

} trace_t;


void createNormals(float * result, tr_room_sector_t * cSect)
{
    uint16_t fd_idx = level.FloorData[cSect->fd_index]&0x1F;
    switch (fd_idx)
    {
        case 0x02:
        {
            int8_t zSlope= (int8_t)level.FloorData[cSect->fd_index + 1];
            float angle = ((float)zSlope/4.0);
            result[X]=0;
            result[Y]=-1+fabs(angle);
            result[Z]=angle;
            return;
        }

        default:
            result[X]=0;
            result[Y]=-1.0;
            result[Z]=0;
            return;
    }

}

int32_t testPathBlocked(trace_t * trace)
{
    tr5_room_t * cRoom = &level.Rooms[trace->roomId];
    float xGrid = trace->destination.x-cRoom->offset.x;
    int16_t x1 = (int16_t)((xGrid-trace->width)/1024*scaleF);
    int16_t x2 = (int16_t)((xGrid+trace->width)/1024*scaleF);
    if (x1<0) x1=0; //Need to check for a portal, else return false
    if (x2>cRoom->num_xsectors) x2=cRoom->num_xsectors;

    float zGrid = trace->destination.z-cRoom->offset.z;
    int16_t z1 = (int16_t)((zGrid-trace->width)/1024*scaleF);
    int16_t z2 = (int16_t)((zGrid+trace->width)/1024*scaleF);
    if (z1<0) z1=0; //Need to check for a portal, else return false
    if (z2>cRoom->num_zsectors) z2=cRoom->num_zsectors;

    tr_room_sector_t * cSect ;

    float yFloor=trace->destination.y+trace->maxY;
    float yCeiling=trace->destination.y+trace->minY;

    int dirX = trace->origin.x <= trace->destination.x ? 1 : -1;
    int dirZ = trace->origin.z < trace->destination.z ? 1 : -1;

    int32_t floorCol, ceilingCol, hasCol=0;

    for (int16_t x=x1; x<=x2; x++)
    {
       for (int16_t z=z1; z<=z2; z++)
       {
            int16_t xz_sect = x*cRoom->num_zsectors + z;

            cSect = &cRoom->sector_list[xz_sect];



            floorCol = ((cSect->floor*256*scaleF < (int16_t)(yFloor)) & ((cSect->room_below == 0xFF)));

            if (floorCol)
            {
                if (cSect->ceiling!=cSect->floor)// && (cSect->floor*256*scaleF > (int16_t)trace->destination.y-))
                {
                    vector3 norm;
                    createNormals((float*)&norm, cSect);
                    vector3 dif = {trace->destination.x - (cRoom->offset.x+x*1024),
                                   trace->destination.y+trace->maxY -  (cSect->floor*256),
                                   trace->destination.z - (cRoom->offset.z+z*1024)};
                    float dist = DotProduct((float*)&dif, (float*)&norm);
                    cout << "DIST : " << ((int)(dist)) << ", normal : " << (int)(norm.x*65536.0) << ", " << (int)(norm.y*65536.0) << ", " << (int)(norm.z*65536) <<"\n";
                    if (dist<=0)
                         trace->destination.y=cSect->floor*256*scaleF - trace->maxY;
                        //trace->destination.y=cSect->floor*256*scaleF +dist;
                        //trace->destination.y=cSect->floor*256*scaleF - (trace->maxY-dist);

                    /*switch (level.FloorData[cSect->fd_index]&0x1F)
                    {
                    case 0x02:
                        {
                            int8_t zSlope= (int8_t)level.FloorData[cSect->fd_index + 1];
                            int8_t xSlope= (int8_t)level.FloorData[cSect->fd_index + 1]+1;
                            cout << ((float)zSlope/4.0) << "\n";
                            float angle = ((float)zSlope/4.0);
                            trace->destination.y = cSect->floor*256*scaleF - trace->maxY;
                            trace->destination.y += ((int)(trace->destination.z)&0x3FF)*(angle);
                            cout << ((int)(trace->destination.z)&0x3FF) << "\n";
                            cout << "ANGLE : " << (int)(angle*65536.0) << "\n\n\n\n";
                            break;
                        }

                    default:
                        trace->destination.y=cSect->floor*256*scaleF - trace->maxY;
                        break;
                    }*/


                }
                else
                {
                    trace->destination.x=trace->origin.x;
                    trace->destination.z=trace->origin.z;
                    hasCol |= 1<<1;
                }


            }
            ceilingCol = (((int16_t)(cSect->ceiling *256*scaleF) > (int16_t)(yCeiling)) & ((cSect->room_above==0xFF)));
            if (ceilingCol)
            {
                cout << "ceiling collision!!!!!!\n";
                trace->destination.y=cSect->ceiling*256.0*scaleF - trace->minY + 32;
                hasCol|=1;
            }
       }
   }

    return hasCol;
}

bool collisionCheck(player_t * player)
{
    trace_t newTrace;

    int16_t newRoomID=findRoomByPos((float*)&player->POSITION);
    if (newRoomID!=-1)   player->roomId=newRoomID;
    newTrace.roomId=player->roomId;

    newTrace.origin=player->POSITION;
    newTrace.destination=player->NEXT_POSITION;
    newTrace.width=128.0f*scaleF;
    newTrace.maxY=512*scaleF;
    newTrace.minY=-64*scaleF;


    if (testPathBlocked(&newTrace)& 0x02)
    {
        cout << "Path blocked...\n";
        while (1)
        {
            //vector3 saveDest = newTrace.destination;
            newTrace.destination.x=player->NEXT_POSITION.x;
            newTrace.destination.z=player->POSITION.z;
            if (!(testPathBlocked(&newTrace)&0x02)) break;
            newTrace.destination.z=player->NEXT_POSITION.z;
            newTrace.destination.x=player->POSITION.x;
            if (!(testPathBlocked(&newTrace)&0x02)) break;
            newTrace.destination.z=player->POSITION.z;
            //If the path is still blocked, should push the player or kill it, but whatever...
            break;
        }

    }
    else cout << "Can move...\n";

player->NEXT_POSITION=newTrace.destination;




   //+ (((player->POSITION.z-cRoom->offset.z)/1024*cRoom->num_xsectors))) ];

  /* else if (ceilingCol)
   {
        if (cSect->ceiling!=cSect->floor)
   }*/
  /*   cout << "Has floor collision? " << floorCol << ", " << (int16_t)cSect->ceiling*256 << ", " << (int16_t)cSect->floor*256 << "\n";

    cout << (int)cRoom->num_xsectors << ", " << (int)cRoom->num_zsectors << "\n";
   cout << "Player's position : " << (int)(player->POSITION.x) << ", " << (int)player->POSITION.y << ", " << (int)player->POSITION.z << ", rotation : "  << (int)(player->ROTATION[Y]) << "\n";
   cout << "Player's grid sector : " << (int)((player->NEXT_POSITION.x-1024)/1024)  << ", " << (int)((player->NEXT_POSITION.z-1024)/1024) << "\n";
  tr5_room_t * r = &level.Rooms[player->roomId];

   //cout << "Room's x/z : " << (int)r->room_x << ", " << (int)r->room_z << "\n";
   cout << "Room's grid : " << (int)(r->offset.x/1024) << ", " << (int)(r->offset.z/1024) <<  ", offsets : " <<
   (int)(r->offset.x/1024)+(int)(r->num_xsectors) << ", " << (int)(r->offset.z/1024)+(int)(r->num_zsectors) << "\n";*/

   return 0;
}


void drawCube(vector3);
int bulletCollision(vector3 position, vector3 direction, int16_t startRoom)
{
    if (!direction.x && !direction.z ) return 0;
    tr5_room_t * cRoom = &level.Rooms[startRoom];

    tr_room_sector_t * cSect ;

    float newPos[3];
    newPos[X] = (position.x - cRoom->offset.x)/1024;
    newPos[Y] = (position.y - cRoom->offset.y)/256;
    newPos[Z] = (position.z - cRoom->offset.z)/1024;


    float stepX = direction.x;
    if (direction.z != 0) //stepX = (direction.x / direction.z);
    stepX= (position.x -  position.x/1024)/1024;
    float stepZ = direction.z;
    if (direction.x != 0) //stepZ = (direction.z/direction.x);
        stepZ= (position.z -  position.z/1024)/1024;
stepX=0; stepZ= ( position.z/1024 - position.z)/1024; if (direction.z < 0) stepZ=min(-stepZ, stepZ); else stepZ=max(stepZ,-stepZ);

    while (1)
    {
        int16_t xz_sect = (int)newPos[X]*cRoom->num_zsectors + (int)newPos[Z];
        if (xz_sect < 0) return 0;
        if (xz_sect >= cRoom->num_xsectors*cRoom->num_zsectors) return 0;

        cSect = &cRoom->sector_list[xz_sect];
        if (cSect->ceiling==cSect->floor)
        {
            vector3 pos;

            pos.x=cRoom->offset.x + newPos[X]*1024;
            pos.y=position.y;
            pos.z=cRoom->offset.z+newPos[Z]*1024;
            drawCube(pos);
            break;
        }

        newPos[X]+= stepX;
        newPos[Z] += stepZ;

    }










    /*
        height = (y3-y1);
    dx_left  = (FIXED)((x3-x1)<<16)/height;
    dx_right = (FIXED)((x3-x2)<<16)/height;
    */

    /*int16_t x1 = (int16_t)((xGrid-trace->width)/1024*scaleF);
    int16_t x2 = (int16_t)((xGrid+trace->width)/1024*scaleF);*/

    return 1;
}
