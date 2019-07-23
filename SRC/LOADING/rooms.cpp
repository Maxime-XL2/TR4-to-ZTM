#include "../common.h"

uint8_t * readTR4room(uint8_t * srcPtr, tr5_room_t & room)
{
    uint8_t * src = srcPtr;
    uint32_t num_data_words;
    uint32_t i;
    uint8_t * pos;

    // read and change coordinate system
    room.offset.x = (float)(*(int32_t*)src); src+=4;
    room.offset.y = 0;
    room.offset.z = (float)(*(int32_t*)src); src+=4;
    room.y_bottom = (float)(*(int32_t*)src); src+=4;
    room.y_top = (float)(*(int32_t*)src); src+=4;

    num_data_words = *(uint32_t*)(src); src+=4;

    cout << "Room offset x=" << (int)room.offset.x << ", y=" << (int)room.offset.y << ", z=" << (int)room.offset.z << ", y bot=" <<  (int)room.y_bottom << ", y top=" << (int)room.y_top <<"\n";
    cout << "Number data words = " << num_data_words << "\n";
    pos=(uint8_t*)src;
    //pos = SDL_RWseek(src, 0, SEEK_CUR);
    room.num_layers = 0;



    /**
          Level->Rooms[i].Data = (bitu8 *)TR2alloc(sizeof(bitu16) * Level->Rooms[i].NumDataWords);
      TR2fread(Level->Rooms[i].Data, sizeof(bitu16), Level->Rooms[i].NumDataWords, fp);
    **/

    room.num_vertices = *(uint16_t*)(src); src+=2;
    cout << "Number of vertices = " << room.num_vertices << "\n";
    room.vertices = (tr5_room_vertex_t*)calloc(room.num_vertices, sizeof(tr5_room_vertex_t));
    for (i = 0; i < room.num_vertices; i++)
        src=read_tr4_room_vertex(src, room.vertices[i]);

    room.num_rectangles = *(uint16_t*)src; src+=2; //read_bitu16(src);
    cout << "Number of rectangles : " << room.num_rectangles << "\n";
    room.rectangles = (tr4_face4_t*)malloc(room.num_rectangles * sizeof(tr4_face4_t));
    for (i = 0; i < room.num_rectangles; i++)
        src=read_tr4_face4(src, room.rectangles[i]);


    room.num_triangles = *(uint16_t*)src; src+=2;
    cout << "Number of triangles : " << room.num_triangles << "\n";
    room.triangles = (tr4_face3_t*)malloc(room.num_triangles * sizeof(tr4_face3_t));
    for (i = 0; i < room.num_triangles; i++)
        src=read_tr4_face3(src, room.triangles[i]);

    room.num_sprites = *(uint16_t*)src; src+=2;
    cout << "Number of sprites : " << room.num_sprites << "\n";
    room.sprites = (tr_room_sprite_t*)malloc(room.num_sprites * sizeof(tr_room_sprite_t));
    for (i = 0; i < room.num_sprites; i++)
        src=read_tr_room_sprite(src, room.sprites[i]);


     src = (uint8_t*)pos;
     src += (num_data_words*2);
    // set to the right position in case that there is some unused data
    //SDL_RWseek(src, pos + (num_data_words * 2), SEEK_SET);

    room.num_portals = *(uint16_t*)(src); src+=2;
    cout << "Number of portals : " << room.num_portals << "\n";
    room.portals = (tr_room_portal_t*)malloc(room.num_portals * sizeof(tr_room_portal_t));
    for (i = 0; i < room.num_portals; i++)
        src=read_tr_room_portal(src, room.portals[i]);

    room.num_zsectors = *(uint16_t*)(src); src+=2;
    room.num_xsectors = *(uint16_t*)(src); src+=2;
    room.sector_list = (tr_room_sector_t*)malloc(room.num_zsectors * room.num_xsectors * sizeof(tr_room_sector_t));
    cout << "Room num sectors : x=" << room.num_xsectors << ", z=" << room.num_zsectors << "\n";
    for (i = 0; i < (uint32_t)(room.num_zsectors * room.num_xsectors); i++)
        src=read_tr_room_sector(src, room.sector_list[i]);

    room.light_colour.b = *(uint8_t*)(src) / 255.0f; src+=1;
    room.light_colour.g = *(uint8_t*)(src) / 255.0f; src+=1;
    room.light_colour.r = *(uint8_t*)(src) / 255.0f; src+=1;
    room.light_colour.a = *(uint8_t*)(src) / 255.0f; src+=1;

    // only in TR2
    room.light_mode = 0;

    room.num_lights = *(uint16_t*)(src); src+=2;
    cout << "Room num lights = " << room.num_lights << "\n";
    room.lights = (tr5_room_light_t*)malloc(room.num_lights * sizeof(tr5_room_light_t));
    for (i = 0; i < room.num_lights; i++)
        src=read_tr4_room_light(src, room.lights[i]);

    room.num_static_meshes = *(uint16_t*)(src); src+=2;
    room.static_meshes = (tr2_room_staticmesh_t*)malloc(room.num_static_meshes * sizeof(tr2_room_staticmesh_t));
    for (i = 0; i < room.num_static_meshes; i++)
        src=read_tr4_room_staticmesh(src, room.static_meshes[i]);
cout << "Room num static meshes : " << room.num_static_meshes << "\n";

    room.alternate_room = *(uint16_t*)(src); src+=2;
    room.flags = *(uint16_t*)(src); src+=2;

    // Only in TR3-5
    room.water_scheme = *(uint8_t*)(src); src+=1;
    room.reverb_info = *(uint8_t*)(src); src+=1;

    // Only in TR4-5
    room.alternate_group = *(uint8_t*)(src); src+=1;

    cout << "Alternate room : " << room.alternate_room << ", flags : " << room.flags << ", water scheme : " << (uint32_t)room.water_scheme << "\n";
    cout << "Reverb info : " << (uint32_t)room.reverb_info << ", Alternate group : " << (uint32_t)room.alternate_group << "\n";

    return src;
}



/** \brief reads a room portal definition.
  *
  * A check is preformed to see wether the normal lies on a coordinate axis, if not an exception gets thrown.
  */
uint8_t * read_tr_room_portal(uint8_t * const srcPtr, tr_room_portal_t & portal)
{
    uint8_t * src = srcPtr;
	portal.adjoining_room = *(uint16_t*)(src); src+=2;
	src=read_tr_vertex16(src, portal.normal);
	src=read_tr_vertex16(src, portal.vertices[0]);
	src=read_tr_vertex16(src, portal.vertices[1]);
	src=read_tr_vertex16(src, portal.vertices[2]);
	src=read_tr_vertex16(src, portal.vertices[3]);
	if ((portal.normal.x == 1.0f) && (portal.normal.y == 0.0f) && (portal.normal.z == 0.0f))
		return src;
	if ((portal.normal.x == -1.0f) && (portal.normal.y == 0.0f) && (portal.normal.z == 0.0f))
		return src;
	if ((portal.normal.x == 0.0f) && (portal.normal.y == 1.0f) && (portal.normal.z == 0.0f))
		return src;
	if ((portal.normal.x == 0.0f) && (portal.normal.y == -1.0f) && (portal.normal.z == 0.0f))
		return src;
	if ((portal.normal.x == 0.0f) && (portal.normal.y == 0.0f) && (portal.normal.z == 1.0f))
		return src;
	if ((portal.normal.x == 0.0f) && (portal.normal.y == 0.0f) && (portal.normal.z == -1.0f))
		return src;
//	cout << "read_tr_room_portal: normal not on world axis\n";
	return src;
}



/// \brief reads a room sector definition.
uint8_t * read_tr_room_sector(uint8_t * const srcPtr, tr_room_sector_t & sector)
{
    uint8_t * src = srcPtr;
	sector.fd_index = *(uint16_t*)(src); src+=2;
	sector.box_index = *(uint16_t*)(src); src+=2;
	sector.room_below = *(uint8_t*)(src); src+=1;
	sector.floor = *(uint8_t*)(src); src+=1;
	sector.room_above = *(uint8_t*)(src); src+=1;
	sector.ceiling = *(uint8_t*)(src); src+=1;
	return src;
}



uint8_t * read_tr4_room_staticmesh(uint8_t * const srcPtr, tr2_room_staticmesh_t & room_static_mesh)
{
    uint8_t * src = srcPtr;
    src=read_tr_vertex32(src, room_static_mesh.pos);
    room_static_mesh.rotation = (float)(*(uint16_t*)src) / 16384.0f * -90; src+=2;  /**What the fuck?**/
    room_static_mesh.intensity1 = (*(uint16_t*)src); src+=2;
    room_static_mesh.intensity2 = (*(uint16_t*)src); src+=2;
    room_static_mesh.object_id = (*(uint16_t*)src); src+=2;

    room_static_mesh.tint.r = ((room_static_mesh.intensity1 & 0x001F)        ) / 31.0f;

    room_static_mesh.tint.g = ((room_static_mesh.intensity1 & 0x03E0) >> 5   ) / 31.0f;

    room_static_mesh.tint.b = ((room_static_mesh.intensity1 & 0x7C00) >> 10  ) / 31.0f;
    room_static_mesh.tint.a = 1.0f;
    return src;
}
