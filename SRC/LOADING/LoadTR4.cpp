#include "../common.h"

/****
This is BY FAR the shittiest map format I've ever witnessed.
WAY too complex, useless compression, no real way to skip ahead
Just getting to the texture informations takes forever...
I might just go with the build engine instead of this crap...
****/



/***
BEST READ : http://icculus.org/vt/vt/src/l_tr1.cpp
uint32_t Version; // version (4 bytes)
uint16_t NumRoomTextiles; // number of non bumped room tiles (2 bytes)
uint16_t NumObjTextiles; // number of object tiles (2 bytes)
uint16_t NumBumpTextiles; // number of bumped room tiles (2 bytes)
uint32_t Textile32_UncompSize; // uncompressed size (in bytes) of the 32-bit textures chunk (4 bytes)
uint32_t Textile32_CompSize; // compressed size (in bytes) of the 32-bit textures chunk (4 bytes)
uint8_t Textile32_Compressed[Textile32_CompSize]; // zlib-compressed 32-bit textures chunk (Textile32_CompSize bytes)
{
    tr4_textile32 Textile32[NumRoomTextiles + NumObjTextiles + NumBumpTextiles];
}
uint32_t Textile16_UncompSize; // uncompressed size (in bytes) of the 16-bit textures chunk (4 bytes)
uint32_t Textile16_CompSize; // compressed size (in bytes) of the 16-bit textures chunk (4 bytes)
uint8_t Textile16_Compressed[Textile32_CompSize]; // zlib-compressed 16-bit textures chunk (Textile16_CompSize bytes)
{
    tr_textile16 Textile16[NumRoomTextiles + NumObjTextiles + NumBumpTextiles];
}
uint32_t Textile32Misc_UncompSize; // uncompressed size (in bytes) of the 32-bit misc textures chunk (4 bytes), should always be 524288
uint32_t Textile32Misc_CompSize; // compressed size (in bytes) of the 32-bit misc textures chunk (4 bytes)
uint8_t Textile32Misc_Compressed[Textile32Misc_CompSize]; // zlib-compressed 32-bit misc textures chunk (Textile32Misc_CompSize bytes)
{
    tr4_textile32 Textile32Misc[2];
}
uint32_t LevelData_UncompSize; // uncompressed size (in bytes) of the level data chunk (4 bytes)
uint32_t LevelData_CompSize; // compressed size (in bytes) of the level data chunk (4 bytes)
uint8_t LevelData_Compressed[LevelData_CompSize]; // zlib-compressed level data chunk (LevelData_CompSize bytes)
{
    uint32_t Unused; // 32-bit unused value, always 0 (4 bytes)
    uint16_t NumRooms; // number of rooms (2 bytes)
    tr4_room Rooms[NumRooms]; // room list (variable length)
    uint32_t NumFloorData; // number of floor data uint16_t's to follow (4 bytes)
    uint16_t FloorData[NumFloorData]; // floor data (NumFloorData * 2 bytes)
    uint32_t NumMeshData; // number of uint16_t's of mesh data to follow (=Meshes[]) (4 bytes)
    tr4_mesh Meshes[NumMeshPointers]; // note that NumMeshPointers comes AFTER Meshes[]
    uint32_t NumMeshPointers; // number of mesh pointers to follow (4 bytes)
    uint32_t MeshPointers[NumMeshPointers]; // mesh pointer list (NumMeshPointers * 4 bytes)
    uint32_t NumAnimations; // number of animations to follow (4 bytes)
    tr4_animation Animations[NumAnimations]; // animation list (NumAnimations * 40 bytes)
    uint32_t NumStateChanges; // number of state changes to follow (4 bytes)
    tr_state_change StateChanges[NumStateChanges]; // state-change list (NumStructures * 6 bytes)
    uint32_t NumAnimDispatches; // number of animation dispatches to follow (4 bytes)
    tr_anim_dispatch AnimDispatches[NumAnimDispatches]; // animation-dispatch list list (NumAnimDispatches * 8 bytes)
    uint32_t NumAnimCommands; // number of animation commands to follow (4 bytes)
    tr_anim_command AnimCommands[NumAnimCommands]; // animation-command list (NumAnimCommands * 2 bytes)
    uint32_t NumMeshTrees; // number of MeshTrees to follow (4 bytes)
    tr_meshtree_node MeshTrees[NumMeshTrees]; // MeshTree list (NumMeshTrees * 4 bytes)
    uint32_t NumFrames; // number of words of frame data to follow (4 bytes)
    uint16_t Frames[NumFrames]; // frame data (NumFrames * 2 bytes)
    uint32_t NumModels; // number of models to follow (4 bytes)
    tr_model Models[NumModels]; // model list (NumModels * 18 bytes)
    uint32_t NumStaticMeshes; // number of StaticMesh data records to follow (4 bytes)
    tr_staticmesh StaticMeshes[NumStaticMeshes]; // StaticMesh data (NumStaticMesh * 32 bytes)
    uint8_t SPR[3]; // S P R (0x53, 0x50, 0x52)
    uint32_t NumSpriteTextures; // number of sprite textures to follow (4 bytes)
    tr_sprite_texture SpriteTextures[NumSpriteTextures]; // sprite texture list (NumSpriteTextures * 16 bytes)
    uint32_t NumSpriteSequences; // number of sprite sequences records to follow (4 bytes)
    tr_sprite_sequence SpriteSequences[NumSpriteSequences]; // sprite sequence data (NumSpriteSequences * 8 bytes)
    uint32_t NumCameras; // number of camera data records to follow (4 bytes)
    tr_camera Cameras[NumCameras]; // camera data (NumCameras * 16 bytes)
    uint32_t NumFlybyCameras; // number of flyby camera data records to follow (4 bytes)
    tr4_flyby_camera FlybyCameras[NumFlybyCameras]; // flyby camera data (NumFlybyCameras * 40 bytes)
    uint32_t NumSoundSources; // number of sound source data records to follow (4 bytes)
    tr_sound_source SoundSources[NumSoundSources]; // sound source data (NumSoundSources * 16 bytes)
    uint32_t NumBoxes; // number of box data records to follow (4 bytes)
    tr2_box Boxes[NumBoxes]; // box data (NumBoxes * 8 bytes)
    uint32_t NumOverlaps; // number of overlap records to follow (4 bytes)
    uint16_t Overlaps[NumOverlaps]; // overlap data (NumOverlaps * 2 bytes)
    int16_t Zones[10*NumBoxes]; // zone data (NumBoxes * 20 bytes)
    uint32_t NumAnimatedTextures; // number of animated texture records to follow (4 bytes)
    uint16_t AnimatedTextures[NumAnimatedTextures]; // animated texture data (NumAnimatedTextures * 2 bytes)
    uint8_t AnimatedTexturesUVCount;
    uint32_t NumObjectTextures; // number of object textures to follow (4 bytes) (after AnimatedTextures in TR3)
    tr4_object_texture ObjectTextures[NumObjectTextures]; // object texture list (NumObjectTextures * 38 bytes)
    uint32_t NumEntities; // number of entities to follow (4 bytes)
    tr4_entity Entities[NumEntities]; // entity list (NumEntities * 24 bytes)
    uint32_t NumAIObjects; // number of AI objects to follow (4 bytes)
    tr4_ai_object AIObjects[NumAIObjects]; // AI objects list (NumAIObjects * 24 bytes)
    uint16_t NumDemoData; // number of demo data records to follow (2 bytes)
    uint8_t DemoData[NumDemoData]; // demo data (NumDemoData bytes)
    int16_t SoundMap[370]; // sound map (740 bytes)
    uint32_t NumSoundDetails; // number of sound-detail records to follow (4 bytes)
    tr3_sound_details SoundDetails[NumSoundDetails]; // sound-detail list (NumSoundDetails * 8 bytes)
    uint32_t NumSampleIndices; // number of sample indices to follow (4 bytes)  +
    uint32_t SampleIndices[NumSampleIndices]; // sample indices (NumSampleIndices * 4 bytes)
}
uint32_t NumSamples; // number of sound samples (4 bytes)
tr4_sample Samples[NumSamples]; // sound samples (this is the last part, so you can simply read until EOF)

***/


/***
bitu32	"TR4",0
bitu16	Number of non bumped room tiles
bitu16	Number of tiles for objects
bitu16	Number of bumped room tiles
bitu32	Uncompressed size of texture 1
bitu32	Compressed size of texture 1
bitu8[]	(compressed) Texture 1 (32 bits texture)
bitu32	Uncompressed size of texture 2
bitu32	Compressed size of texture 2
bitu8[]	(compressed) Texture 2 (16 bits texture)
bitu32	Uncompressed size of texture 3
bitu32	Compressed size of texture 3
bitu8[]	(compressed) Texture 3 (32 bits texture)	-> a 256x512 texture with the font & sky
bitu32	Uncompressed size of geometry
bitu32	Compressed size of geometry
bitu8[]	(compressed) Geometry
bitu32	num_sounds
num_sounds * struct TR4_TrailingSound
{
bitu32	UncompSize;	// Uncompressed sound size
bitu32	CompSize;	// Compressed sound size -> compression is ADPCM (WAV format)
bitu8	sound_data[];	// data of the sound - size is CompSize bitu8
}


***/

uint8_t * ptr=nullptr;
uint8_t * buffer=nullptr;
tr4_level_load level;
#define MAX_MAP_SIZE (1024*1024*64)


unsigned int tryToFindFuckingTEX(uint8_t * src);
unsigned int tryToFindFuckingSPR(uint8_t * src);
void deleteBuffer(void);
int setTextures(tr4_level_load*);
void display_TR4_debugData(tr4_level_load *);

uint32_t readU32(void)
{
    uint32_t buf = *(uint32_t*)ptr;
    ptr+=4;
    return buf;
}
int32_t readS32(void)
{
    int32_t buf = *(int32_t*)ptr;
    ptr+=4;
    return buf;
}
uint16_t readU16(void)
{
    uint16_t buf = *(uint16_t*)ptr;
    ptr+=2;
    return buf;
}
int16_t readS16(void)
{
    int16_t buf = *(int16_t*)ptr;
    ptr+=2;
    return buf;
}
uint8_t readU8(void)
{
    uint8_t buf = *(uint8_t*)ptr;
    ptr+=1;
    return buf;
}
int8_t readS8(void)
{
    int8_t buf = *(int8_t*)ptr;
    ptr+=1;
    return buf;
}

int LOAD_TR4(string filename)
{
    cout << "******************************************\n";
    cout << "TR4 TO Z-TREME ENGINE MAP CONVERTER BY XL2\n";
    cout << "******************************************\n";
    cout << "\nREADING TR4 FILE " << filename << "\n------------------\n";

    ifstream file(filename.c_str(), ios::in | ios::binary);
    if (!file.is_open()) {cout << "\n!!!!\nOpening of file " << filename << " failed. Quitting the program...\n"; return 0;}
    cout <<  "File " << filename << " opened\n";
    buffer =	(uint8_t *)	malloc (MAX_MAP_SIZE);
    ZeroMemory(buffer,	MAX_MAP_SIZE);
    file.read((char*)buffer, MAX_MAP_SIZE);
    ptr = &buffer[0];

    if (*(uint32_t*)ptr != 0x00345254) {
            cout << "Wrong version number. It needs to be a TR4 map! \n";
            deleteBuffer();
            file.close();
            return 0;
    }

    level.Version=                readU32();
    level.NumRoomTextiles =       readU16();
    level.NumObjTextiles =        readU16();
    level.NumBumpTextiles =       readU16();
    level.Textile32_UncompSize =  readU32();
    level.Textile32_CompSize =    readU32();


    level.NumMiscTextiles=2;
    level.TotalTextile32 = level.NumBumpTextiles +level.NumMiscTextiles + level.NumObjTextiles + level.NumRoomTextiles;
    cout << "Total textile 32 : " << level.TotalTextile32 << "\n";


    uint8_t * uncomp_buffer = (uint8_t*) malloc(sizeof(uint8_t) * level.Textile32_UncompSize);
    ZeroMemory(uncomp_buffer,	level.Textile32_UncompSize);
    level.Textile32 = (tr4_textile32_t*)malloc(level.TotalTextile32*sizeof(tr4_textile32_t));

    uint8_t * comp_buffer = (uint8_t*) malloc(level.Textile32_CompSize);

    memcpy(comp_buffer, (void*)ptr, level.Textile32_CompSize);
    ptr=ptr+level.Textile32_CompSize;

    uLongf uncompSize = level.Textile32_UncompSize;
    if (uncompress(uncomp_buffer, &uncompSize, comp_buffer, level.Textile32_CompSize)!=Z_OK)
    {
        cout << "Failed to decompress the textures...\n\n";
    }
    cout << "Uncompressed size : " << uncompSize << ", sizeof uncomp_buffer : " << sizeof(uncomp_buffer) << "\n";

    cout << "Reading textures...\n";
    uint32_t * uncmpPtr = (uint32_t*)uncomp_buffer;
    for (unsigned int i = 0; i < (level.TotalTextile32 - level.NumMiscTextiles); i++)
    {
        ReadTextiles32(uncmpPtr, &level.Textile32[i]);
        uncmpPtr+=(256*256);
    }
    if (comp_buffer)        {delete [] comp_buffer; comp_buffer=nullptr;}
    if (uncomp_buffer)      {delete [] uncomp_buffer; uncomp_buffer=nullptr;}


    cout << "Skipping 16 bpp textures for now...\n";
    level.Textile16_UncompSize =    readU32();//*(uint32_t*)ptr; ptr+=4;
    level.Textile16_CompSize =      readU32();//*(uint32_t*)ptr; ptr+=4;

    ptr+=level.Textile16_CompSize;


    uint32_t text3_UncompSize = readU32();// *(uint32_t*)ptr; ptr+=4;
    uint32_t text3_CompSize   = readU32();//*(uint32_t*)ptr; ptr+=4;

    cout << "Sky compSize = " << text3_CompSize << ", uncompSize = " << text3_UncompSize << "\n";
    cout << "Skipping sky textures reading for now...\n";
    ptr+=text3_CompSize;

    readTR4geometry(&level);

    /*cout << "Alternate textures read using TGA...\n";
    extern int ReadTGAFile (string folder, string filename, unsigned int texture_id);
    ReadTGAFile("IN/MAP/", "TEST", 0);
    cout << "Setting textures...\n";*/


    int sizeT = setTextures(&level);
    cout << sizeT << "\n";
cout << level.object_textures_count << "\n";
    //while (1) {}


    display_TR4_debugData(&level);


    deleteBuffer();
    /****/
    ///TESTING
    if (comp_buffer)        delete [] comp_buffer;
    if (uncomp_buffer)      delete [] uncomp_buffer;
    //delete [] level.Textile32;
    /****/

    file.close();
    return 1;
}


unsigned int tryToFindFuckingSPR(uint8_t * src)
{
    unsigned int debugCounter=0;
    {

        while (1)
        {
            char SPR[3]= {*(int8_t*)(src+debugCounter),*(int8_t*)(src+debugCounter+1), *(int8_t*)(src+debugCounter+2)};
            if (SPR[0]=='S' && SPR[1]=='P' && SPR[2]=='R')
            {
                cout << "SPR reached at " << debugCounter << " bytes offset!\n";
                return debugCounter;
            }


            if (++debugCounter > 6553600) {cout << "Couldn't reach SPR...\n"; break;}
        }
    }
    return 0;
}

unsigned int tryToFindFuckingTEX(uint8_t * src)
{
    unsigned int debugCounter=0;
    {

        while (1)
        {
            char TEX[3]= {*(int8_t*)(src+debugCounter),*(int8_t*)(src+debugCounter+1), *(int8_t*)(src+debugCounter+2)};
            if (TEX[0]=='T' && TEX[1]=='E' && TEX[2]=='X')
            {
                cout << "TEX reached at " << debugCounter << " bytes offset!\n";
                return debugCounter;
            }


            if (++debugCounter > 6553600) {cout << "Couldn't reach TEX...\n"; break;}
        }
    }
    return 0;
}




int readTR4geometry(tr4_level_load * lvl)
{
    uint32_t geoUncompSize = readU32();// *(uint32_t*)ptr; ptr+=4;
    uint32_t geoCompSize = readU32();// *(uint32_t*)ptr; ptr+=4;

    cout << "Geometry uncomp size = " << geoUncompSize << ", comp size = " << geoCompSize << "\n";


    uint8_t * uncomp_buffer = new uint8_t[geoUncompSize];
    uint8_t * comp_buffer = new uint8_t[geoCompSize];

    memcpy(comp_buffer, (void*)ptr, geoCompSize);
    uLongf uncompSize = geoUncompSize;
    if (uncompress(uncomp_buffer, &uncompSize, comp_buffer, geoCompSize)!=Z_OK)
            cout << "Failed to decompress the textures...\n\n";

    cout << "Geometry sanity check (must be 0) : " << uncompSize-geoUncompSize << "\n";

    ptr+=geoCompSize;

    uint8_t * uncompPtr = uncomp_buffer+4;
    uint16_t numRoom =*(uint16_t*)(uncompPtr); uncompPtr+=2;
    lvl->numRooms = (uint32_t)numRoom;
    cout << "Number of rooms = " << numRoom << "\n";

    lvl->Rooms = new tr5_room_t[numRoom];
    uint8_t * newPtr = uncompPtr;
    cout << "New Ptr : " << *(uint64_t*)newPtr << "\n";
    for (uint16_t i = 0; i < numRoom; i++)
    {
        newPtr= readTR4room(newPtr, lvl->Rooms[i] );
        cout << "Ptr = " << *(uint64_t*)newPtr << "\n";
    }






    level.NumFloorData = *(uint32_t*)newPtr; newPtr+=4;
    level.FloorData = (uint16_t*)malloc(level.NumFloorData * sizeof(uint16_t));
    cout << "Number of floors : " << level.NumFloorData << "\n";
    for (unsigned int i=0; i<level.NumFloorData; i++)
    {
        level.FloorData[i]=*(uint16_t*)newPtr; newPtr+=2; //readU16();
    }

    //ALTnewPtr=read_mesh_data(&level, newPtr);
    level.meshes_count = *(uint32_t*)newPtr; newPtr+=4;
    cout << "Meshes count : " << level.meshes_count << "\n";
    //SKIP
    newPtr+=(sizeof(uint16_t)*level.meshes_count);
    level.mesh_indices_count = *(uint32_t*)newPtr; newPtr+=4;
    newPtr+=level.mesh_indices_count*sizeof(uint32_t);
    /**
       TR2fread(&NumMeshDataWords, sizeof(NumMeshDataWords), 1, fp);
   CONVERT_ENDIAN(&NumMeshDataWords, sizeof(NumMeshDataWords));
   RawMeshData = (bitu8 *)TR2alloc(sizeof(bitu16) * NumMeshDataWords);
   TR2fread(RawMeshData, sizeof(bitu16), NumMeshDataWords, fp);
   // Endian-conversion of this data occurs in ioExtractMeshes()

   // read mesh pointers
   TR2fread(&NumMeshPointers, sizeof(NumMeshPointers), 1, fp);
   CONVERT_ENDIAN(&NumMeshPointers, sizeof(NumMeshPointers));
   MeshPointerList = (bitu32 *)TR2alloc(sizeof(bitu32) * NumMeshPointers);
   TR2fread(MeshPointerList, sizeof(bitu32), NumMeshPointers, fp);

    **/

    level.animations_count = *(uint32_t*)newPtr; newPtr+=4;
    level.animations = (tr_animation_t*)malloc(level.animations_count * sizeof(tr_animation_t));
    cout << "Level animations count : " << level.animations_count << "\n";
    for (unsigned int i =0; i<level.animations_count; i++){
        newPtr += 40; //read_tr4_animations(newPtr, &level.animations[i]);
    }

    uint32_t state_change_count = *(uint32_t*)newPtr; newPtr+=4;
    cout << "State change count : " << state_change_count << "\n";
    newPtr += sizeof(tr_state_change_t)*state_change_count;

    uint32_t anim_dispatches_count  = *(uint32_t*)newPtr; newPtr+=4;
    cout << "anim dispatches count : " << anim_dispatches_count << "\n";
    newPtr += sizeof(tr_anim_dispatch_t)*anim_dispatches_count;

    uint32_t anim_commands_count = *(uint32_t*)newPtr; newPtr+=4;
    cout << "anim commands count : " << anim_commands_count << "\n";
    newPtr += sizeof(int16_t) * anim_commands_count;

    uint32_t mesh_tree_data_size = *(uint32_t*)newPtr; newPtr+=4;
    cout << "Mesh tree data size : " << mesh_tree_data_size << "\n";
    newPtr += sizeof(uint32_t)*mesh_tree_data_size;


    newPtr=read_frame_moveable_data(&level, newPtr);



    level.static_meshes_count = *(uint32_t*)newPtr; newPtr+=4;
    cout << "Static meshes count : " << level.static_meshes_count << "\n";
    /****Skipped***/
    //newPtr+=(sizeof(tr_staticmesh_t)*level.static_meshes_count);
    newPtr+=(sizeof(uint32_t)*level.static_meshes_count);

newPtr+=tryToFindFuckingSPR(newPtr); newPtr+=3;

    //newPtr+=3;

    level.sprite_textures_count = *(uint32_t*)newPtr; newPtr+=4;
    cout << "Sprite textures count : " << level.sprite_textures_count << "\n";
    /***SKIPPED!**/
    //newPtr+=sizeof(tr_sprite_texture_t)*level.sprite_textures_count;
newPtr+=16*level.sprite_textures_count;

    level.sprite_sequences_count =  *(uint32_t*)newPtr; newPtr+=4;
    cout << "Sprite sequences count : " << level.sprite_sequences_count << "\n";
    newPtr+=sizeof(tr_sprite_sequence_t)*level.sprite_sequences_count; //8 bytes

    level.cameras_count = *(uint32_t*)newPtr; newPtr+=4;
    cout << "Camera count : " << level.cameras_count << "\n";
    newPtr+=sizeof(tr_camera_t)*level.cameras_count; //16 bytes

    level.flyby_cameras_count =  *(uint32_t*)newPtr; newPtr+=4;
    cout << "Flyby camera count : " << level.flyby_cameras_count << "\n";
    newPtr+=sizeof(tr4_flyby_camera_t)*level.flyby_cameras_count;

    level.sound_sources_count = *(uint32_t*)newPtr; newPtr+=4;
    cout << "Sound sources count : " << level.sound_sources_count << "\n";
    newPtr+=sizeof(tr_sound_source_t)*level.sound_sources_count; //16

    level.boxes_count = *(uint32_t*)newPtr; newPtr+=4;
    cout << "Boxes count : " << level.boxes_count << "\n";
    newPtr+=8*level.boxes_count;

    level.overlaps_count = *(uint32_t*)newPtr; newPtr+=4;
    cout << "Overlaps count : " << level.overlaps_count << "\n";
    newPtr+=level.overlaps_count*sizeof(uint16_t);


    newPtr+=(10*2)*level.boxes_count;

    level.animated_textures_count = *(uint32_t*)newPtr; newPtr+=4;
    cout << "Animted textures count : " << level.animated_textures_count << "\n";
    newPtr += level.animated_textures_count*sizeof(uint16_t);

    level.animated_textures_uv_count = *(uint8_t*)newPtr; newPtr++;

newPtr+= tryToFindFuckingTEX(newPtr); newPtr+=3;

    level.object_textures_count = *(uint32_t*)newPtr; newPtr+=4;
    cout << "Object textures count : " << level.object_textures_count << "\n";
    level.object_textures = (tr4_object_texture_t*)malloc(level.object_textures_count * sizeof(tr4_object_texture_t));
    for (unsigned int i=0; i<level.object_textures_count; i++)
    {
       // cout << i << "         ";
        newPtr = read_tr4_object_texture(&level, newPtr, &level.object_textures[i] );
    }
    //newPtr += level.object_textures_count * sizeof(tr4_object_texture_t);

    /**Items**/
    level.items_count =  *(uint32_t*)newPtr; newPtr+=4;
    cout << "Level items count : " << level.items_count << "\n";
    level.items = (tr2_item_t*)malloc(level.items_count * sizeof(tr2_item_t));
    for (unsigned int i = 0; i < level.items_count; i++)
        newPtr=read_tr4_item(&level, newPtr, level.items[i]);

    level.ai_objects_count = *(uint32_t*)newPtr; newPtr+=4;
    cout << "Level ai objects count : " << level.ai_objects_count << "\n";
    level.ai_objects = (tr4_ai_object_t*)malloc(level.ai_objects_count*sizeof(tr4_ai_object_t));
    for (unsigned int i=0; i<level.ai_objects_count; i++)
    {
        level.ai_objects[i].object_id = *(uint16_t*)newPtr; newPtr+=2;
        level.ai_objects[i].room = *(uint16_t*)newPtr; newPtr+=2;

        level.ai_objects[i].x = *(int32_t*)newPtr; newPtr+=4;
        level.ai_objects[i].y = *(int32_t*)newPtr; newPtr+=4;
        level.ai_objects[i].z = *(int32_t*)newPtr; newPtr+=4;

        level.ai_objects[i].ocb = *(uint16_t*)newPtr; newPtr+=2;
        level.ai_objects[i].flags = *(uint16_t*)newPtr; newPtr+=2;
        level.ai_objects[i].angle = *(int32_t*)newPtr; newPtr+=4;
    }

    /*this->ai_objects_count = read_bitu32(newsrc);
    this->ai_objects = (tr4_ai_object_t*)malloc(this->ai_objects_count * sizeof(tr4_ai_object_t));
    for(i=0; i < this->ai_objects_count; i++)
    {
        this->ai_objects[i].object_id = read_bitu16(newsrc);
        this->ai_objects[i].room = read_bitu16(newsrc);                        // 4

        this->ai_objects[i].x = read_bit32(newsrc);
        this->ai_objects[i].y = read_bit32(newsrc);
        this->ai_objects[i].z = read_bit32(newsrc);                            // 16

        this->ai_objects[i].ocb = read_bitu16(newsrc);
        this->ai_objects[i].flags = read_bitu16(newsrc);                       // 20
        this->ai_objects[i].angle = read_bit32(newsrc);                        // 24
    }*/


    /***

    this->object_textures_count = read_bitu32(newsrc);
    this->object_textures = (tr4_object_texture_t*)malloc(this->object_textures_count * sizeof(tr4_object_texture_t));
    for (i = 0; i < this->object_textures_count; i++)
        read_tr4_object_texture(newsrc, this->object_textures[i]);
    ***/


return 1;

}



void display_TR4_debugData(tr4_level_load * lvl)
{
    cout << "\n****DEBUG*****\nVersion : " << lvl->Version << "\n";
    cout << "NumRoomTextiles : " << lvl->NumRoomTextiles << "\n";
    cout << "NumObjTextiles  : " << lvl->NumObjTextiles << "\n";
    cout << "NumBumpTextiles : " << lvl->NumBumpTextiles << "\n";
    cout << "Textile32_UncompSize : " << (uint32_t)lvl->Textile32_UncompSize << "\n";
    cout << "Textile32_CompSize : " << (uint32_t)lvl->Textile32_CompSize << "\n";
    cout << "Textile16_UncompSize : " << (uint32_t)lvl->Textile16_UncompSize << "\n";
    cout << "Textile16_CompSize : " << (uint32_t)lvl->Textile16_CompSize << "\n";
    /*cout << "Textile16_UncompSize : " << (uint32_t)lvl->Textile16_UncompSize << "\n";
    cout << "Textile16_CompSize : " << (uint32_t)lvl->Textile16_CompSize << "\n";*/
}

void deleteBuffer(void)
{
    if (buffer) delete [] buffer;
}


/***
void TR_Level::read_tr2_box(SDL_RWops * const src, tr_box_t & box)
{
    box.zmax =-1024 * read_bitu8(src);
    box.zmin =-1024 * read_bitu8(src);
    box.xmin = 1024 * read_bitu8(src);
    box.xmax = 1024 * read_bitu8(src);
    box.true_floor =-read_bit16(src);
    box.overlap_index = read_bitu16(src);
}
***/

/***IMPORTANT***
void TR_Level::read_tr2_zone(SDL_RWops * const src, tr2_zone_t & zone)
{
    zone.GroundZone1_Normal = read_bit16(src);
    zone.GroundZone2_Normal = read_bit16(src);
    zone.GroundZone3_Normal = read_bit16(src);
    zone.GroundZone4_Normal = read_bit16(src);
    zone.FlyZone_Normal = read_bit16(src);
    zone.GroundZone1_Alternate = read_bit16(src);
    zone.GroundZone2_Alternate = read_bit16(src);
    zone.GroundZone3_Alternate = read_bit16(src);
    zone.GroundZone4_Alternate = read_bit16(src);
    zone.FlyZone_Alternate = read_bit16(src);
}***/

