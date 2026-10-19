#include "world/world.h"

static World world;
World rendered_chunks;
float render_distance = 3.0f;
int chunk_index;
int number_of_chunks = 0;
int surronding_render;

World get_world(){return world;}

void world_init()
{
    surronding_render = (render_distance * 2) + 1;
    world = malloc(surronding_render * surronding_render * sizeof(Chunk*));
    

    for (int j = 0; j < surronding_render; j++)
    {
        for (int i = 0; i < surronding_render; i++)
        {
            chunk_index = get_chunk_index(i,j);
            world[chunk_index] = create_chunk((vec2) {i - render_distance + get_camera_chunk_position().x , j - render_distance + get_camera_chunk_position().y});
            number_of_chunks++;
            create_chunk_mesh(world[chunk_index]);
        }
    }
} 

void generate_chunks()
{

    if (!chunk_exists((vec2) {render_distance + get_camera_chunk_position().x , get_camera_chunk_position().y}))
    {
        int world_size_pre = sizeof(world);
        world = realloc(world, world_size_pre + (surronding_render * sizeof(Chunk*)));
        for (size_t i = 0; i < surronding_render; i++)
        {
            chunk_index += surronding_render;
            world[chunk_index] = create_chunk((vec2) {render_distance + get_camera_chunk_position().x , get_camera_chunk_position().y + i});
            number_of_chunks++;
            create_chunk_mesh(world[chunk_index]);
        }
    }

    if (!chunk_exists((vec2) { get_camera_chunk_position().x , render_distance + get_camera_chunk_position().y}))
    {
        int world_size_pre = sizeof(world);
        world = realloc(world, world_size_pre + (surronding_render * sizeof(Chunk*)));
        for (size_t i = 0; i < surronding_render; i++)
        {
            chunk_index += surronding_render;
            world[chunk_index] = create_chunk((vec2) {get_camera_chunk_position().x + i, render_distance + get_camera_chunk_position().y});
            number_of_chunks++;
            create_chunk_mesh(world[chunk_index]);
        }
    }

    if (!chunk_exists((vec2) {get_camera_chunk_position().x - render_distance, get_camera_chunk_position().y}))
    {
        int world_size_pre = sizeof(world);
        world = realloc(world, world_size_pre + (surronding_render * sizeof(Chunk*)));
        for (size_t i = 0; i < surronding_render; i++)
        {
            chunk_index += surronding_render;
            world[chunk_index] = create_chunk((vec2) {get_camera_chunk_position().x - render_distance , get_camera_chunk_position().y - i});
            number_of_chunks++;
            create_chunk_mesh(world[chunk_index]);
        }
    }

    if (!chunk_exists((vec2) {get_camera_chunk_position().x, get_camera_chunk_position().y - render_distance,}))
    {
        int world_size_pre = sizeof(world);
        world = realloc(world, world_size_pre + (surronding_render * sizeof(Chunk*)));
        for (size_t i = 0; i < surronding_render; i++)
        {
            chunk_index += surronding_render;
            world[chunk_index] = create_chunk((vec2) {get_camera_chunk_position().x - i , get_camera_chunk_position().y - render_distance});
            number_of_chunks++;
            create_chunk_mesh(world[chunk_index]);
        }
    }

}

void load_chunks()
{

}

bool chunk_exists(vec2 position)
{
    for (size_t i = 0; i < number_of_chunks; i++)
    {
        if ((position.x == world[i * surronding_render]->position.x) && (position.y  == world[i * surronding_render]->position.y))
            return true;
        
    }
    return false;
}

int get_chunk_index(int xPos, int yPos)
{
    return (xPos * surronding_render) + yPos;
}
