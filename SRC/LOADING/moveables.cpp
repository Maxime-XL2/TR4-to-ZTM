#include "../common.h"


/**
void TR_Level::read_tr_moveable(SDL_RWops * const src, tr_moveable_t & moveable)
{
    moveable.object_id = read_bitu32(src);
    moveable.num_meshes = read_bitu16(src);
    moveable.starting_mesh = read_bitu16(src);
    moveable.mesh_tree_index = read_bitu32(src);
    moveable.frame_offset = read_bitu32(src);
    moveable.animation_index = read_bitu16(src);

    // Disable unused skybox polygons.
    if((this->game_version == TR_III) && (moveable.object_id == 355))
    {
        this->meshes[(this->mesh_indices[moveable.starting_mesh])].num_coloured_triangles = 16;
    }
}
**/
uint8_t * read_frame_moveable_data(tr4_level_load * level, uint8_t * pointer)
{
 //   uint32_t i;
//    uint8_t *newsrc = nullptr;

    uint8_t * src = pointer;
//    uint32_t pos = 0;
//    uint32_t frame = 0;

    level->frame_data_size = *(uint32_t*)src; src+=4;  //read_bitu32(src);
    cout << "Frame data size : " << level->frame_data_size << "\n";
    level->frame_data = (uint16_t*)malloc(level->frame_data_size * sizeof(uint16_t));
    memccpy(level->frame_data, src, 1, level->frame_data_size );
    src+=level->frame_data_size*sizeof(uint16_t);

    level->moveables_count = *(uint32_t*)src; src+=4; //read_bitu32(src);

    cout << "Moveables count : " << level->moveables_count << "\n";
    level->moveables = (tr_moveable_t*)calloc(level->moveables_count, sizeof(tr_moveable_t));

    src+=level->moveables_count*(18);
    return src;
    /*for (i = 0; i < this->moveables_count; i++)
    {
        if (this->game_version < TR_V)
            read_tr_moveable(src, this->moveables[i]);
    }

    for (i = 0; i < this->moveables_count; i++)
    {
        uint32_t j;

        for (j = 0; j < this->moveables_count; j++)
            if (this->moveables[j].frame_offset == pos)
            {
                this->moveables[j].frame_index = frame;
                this->moveables[j].frame_offset = 0;
            }

        SDL_RWseek(newsrc, pos, RW_SEEK_SET);

        frame++;

        pos = 0;
        for (j = 0; j < this->moveables_count; j++)
            if (this->moveables[j].frame_offset > pos)
            {
                pos = this->moveables[j].frame_offset;
                break;
            }
    }

    SDL_RWclose(newsrc);
    newsrc = NULL;*/
}
