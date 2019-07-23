/**LoadTR4**/
int LOAD_TR4(string filename);
int readTR4geometry(tr4_level_load * lvl);

/**Vertices**/
uint8_t * read_tr4_vertex_float(uint8_t * const srcPtr, tr5_vertex_t & vertex);
uint8_t * read_tr_vertex32(uint8_t * const srcPtr, tr5_vertex_t & vertex);
uint8_t * read_tr_vertex16(uint8_t * const srcPtr, tr5_vertex_t & vertex);
uint8_t * read_tr4_room_vertex(uint8_t * srcPtr, tr5_room_vertex_t & room_vertex);

/**Textures**/
void ReadTextiles32(void * const src, tr4_textile32_t * textile);
uint8_t * read_tr4_object_texture_vert(tr4_level_load * level, uint8_t * srcPtr, tr4_object_texture_vert_t * vert);
uint8_t * read_tr4_object_texture(tr4_level_load * level, uint8_t * srcPtr, tr4_object_texture_t * object_texture);

/**Rooms**/
uint8_t * readTR4room(uint8_t * srcPtr, tr5_room_t & room);
uint8_t * read_tr_room_portal(uint8_t * const srcPtr, tr_room_portal_t & portal);
uint8_t * read_tr_room_sector(uint8_t * const srcPtr, tr_room_sector_t & sector);
uint8_t * read_tr4_room_staticmesh(uint8_t * const srcPtr, tr2_room_staticmesh_t & room_static_mesh);

/**Faces (polygons)**/

uint8_t * read_tr4_face4(uint8_t * srcPtr, tr4_face4_t & meshface);
uint8_t * read_tr4_face3(uint8_t * srcPtr, tr4_face3_t & meshface);


/**Sprites**/
uint8_t * read_tr_room_sprite(uint8_t * const srcPtr, tr_room_sprite_t & room_sprite);

/**Lights**/
uint8_t * read_tr4_room_light(uint8_t * const srcPtr, tr5_room_light_t & light);

/**Colors**/
uint8_t * read_tr_colour(uint8_t * const srcPtr, tr2_colour_t & colour);


/**Mesh**/
uint8_t * read_mesh_data(tr4_level_load * level, uint8_t * srcPtr);
uint8_t * read_tr4_mesh(tr4_mesh_t & mesh, uint8_t * src);

/**Items**/
uint8_t * read_tr4_item(tr4_level_load * level, uint8_t * srcPtr, tr2_item_t & item);

/**Sprites**/
uint8_t * read_tr_room_sprite(uint8_t * const srcPtr, tr_room_sprite_t & room_sprite);

/**Moveables**/
uint8_t * read_frame_moveable_data(tr4_level_load * level, uint8_t * pointer);

/**Variables**/

extern uint8_t * ptr;
extern uint8_t * buffer;
extern tr4_level_load level;
