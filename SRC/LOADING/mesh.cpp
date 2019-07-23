#include "../common.h"


uint8_t * read_tr4_mesh(tr4_mesh_t & mesh, uint8_t * src)
{
    int i;
    uint8_t * newSrc = src;
    newSrc=read_tr_vertex16(newSrc, mesh.centre);
    mesh.collision_size = *(int16_t*)newSrc; newSrc+=2;
    mesh.flags = *(uint8_t*)newSrc; newSrc+=1;
    mesh.dummy = *(uint8_t*)newSrc; newSrc+=1;

    mesh.num_vertices = *(int16_t*)newSrc; newSrc+=2;
    mesh.vertices = (tr5_vertex_t*)malloc(mesh.num_vertices * sizeof(tr5_vertex_t));
    for (i = 0; i < mesh.num_vertices; i++)
        newSrc=read_tr_vertex16(newSrc, mesh.vertices[i]);

    mesh.num_normals = *(int16_t*)newSrc; newSrc+=2;
    if (mesh.num_normals >= 0)
    {
        mesh.num_lights = 0;
        mesh.normals = (tr5_vertex_t*)malloc(mesh.num_normals * sizeof(tr5_vertex_t));
        for (i = 0; i < mesh.num_normals; i++)
            newSrc=read_tr_vertex16(newSrc, mesh.normals[i]);
    }
    else
    {
        mesh.num_lights = -mesh.num_normals;
        mesh.num_normals = 0;
        mesh.lights = (int16_t*)malloc(mesh.num_lights * sizeof(int16_t));
        for (i = 0; i < mesh.num_lights; i++)
        {
            mesh.lights[i] = *(int16_t*)newSrc; newSrc+=2;
        }
    }

    mesh.num_textured_rectangles = *(int16_t*)newSrc; newSrc+=2;
    mesh.textured_rectangles = (tr4_face4_t*)malloc(mesh.num_textured_rectangles * sizeof(tr4_face4_t));
    for (i = 0; i < mesh.num_textured_rectangles; i++)
        newSrc=read_tr4_face4(newSrc, mesh.textured_rectangles[i]);

    mesh.num_textured_triangles = *(int16_t*)newSrc; newSrc+=2;
    mesh.textured_triangles = (tr4_face3_t*)malloc(mesh.num_textured_triangles * sizeof(tr4_face3_t));
    for (i = 0; i < mesh.num_textured_triangles; i++)
        newSrc=read_tr4_face3(newSrc, mesh.textured_triangles[i]);

    mesh.num_coloured_rectangles = 0;
    mesh.num_coloured_triangles = 0;

    return newSrc;
}

/******/
uint8_t * read_mesh_data(tr4_level_load * level, uint8_t * srcPtr)
{
    /**
       TR2fread(&Level->NumMeshTrees, sizeof(Level->NumMeshTrees), 1, fp);
   CONVERT_ENDIAN(&Level->NumMeshTrees, sizeof(Level->NumMeshTrees));
   if (Level->NumMeshTrees > 0) {
      Level->MeshTrees = (struct tr2_meshtree_struct *)TR2alloc(Level->NumMeshTrees * sizeof(bitu32));
      TR2fread(Level->MeshTrees, sizeof(bit32), Level->NumMeshTrees, fp);
      }

    uint32_t NumMeshData; // number of uint16_t's of mesh data to follow (=Meshes[]) (4 bytes)
    tr4_mesh Meshes[NumMeshPointers]; // note that NumMeshPointers comes AFTER Meshes[]
    uint32_t NumMeshPointers; // number of mesh pointers to follow (4 bytes)
    uint32_t MeshPointers[NumMeshPointers]; // mesh pointer list (NumMeshPointers * 4 bytes)
    **/
    uint8_t * src = srcPtr;
    uint8_t *buffer;
//    uint8_t *newsrc = nullptr;
    uint32_t size;
//    uint32_t pos = 0;
//    int mesh = 0;
    uint32_t i;
    uint32_t num_mesh_data;

    num_mesh_data = *(uint32_t*)src; src+=4; //read_bitu32(src);
    cout << "Number of mesh data : " << num_mesh_data << "\n";
    size = num_mesh_data * 2;
    buffer = new uint8_t[size];

    memccpy(&buffer[0], src, 1, size);
    src+=size;

 //   newsrc = &buffer[0];


    level->mesh_indices_count = *(uint32_t*)src; src+=4; //read_bitu32(src);
    cout << "Mesh indices count : " << level->mesh_indices_count << "\n";
    level->mesh_indices = (uint32_t*)malloc(level->mesh_indices_count * sizeof(uint32_t));
    for (i = 0; i < level->mesh_indices_count; i++)
       {
           level->mesh_indices[i] = *(uint32_t*)src; src+=4;
       }

    return src;
   /*** level->meshes_count = level->mesh_indices_count;
    level->meshes = (tr4_mesh_t*)calloc(level->meshes_count, sizeof(tr4_mesh_t));

    for (i = 0; i < level->mesh_indices_count; i++)
    {
        uint32_t j;

        for (j = 0; j < level->mesh_indices_count; j++)
            if (level->mesh_indices[j] == pos)
                level->mesh_indices[j] = mesh;

        newsrc=&buffer[pos];
        //SDL_RWseek(newsrc, pos, RW_SEEK_SET);

        if (this->game_version >= TR_IV)
            read_tr4_mesh(newsrc, this->meshes[mesh]);
        else
            read_tr_mesh(newsrc, this->meshes[mesh]);

        mesh++;

        for (j = 0; j < this->mesh_indices_count; j++)
            if (this->mesh_indices[j] > pos)
            {
                pos = this->mesh_indices[j];
                break;
            }
    }
    SDL_RWclose(newsrc);
    newsrc = NULL;
    delete [] buffer;*/
} /*************************/


