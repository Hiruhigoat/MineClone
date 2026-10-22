#include "world/world.h"

static World world;
int render_distance = 2;
int chunk_index;
int number_of_chunks;
int surronding_render;

int get_render_distance()
{
    return render_distance;
}

World get_world(){return world;}

void world_init()
{
    surronding_render = (render_distance * 2) + 1;
    number_of_chunks = surronding_render * surronding_render;
    world = malloc(number_of_chunks * sizeof(Chunk*));
    
    for (int j = 0; j < surronding_render; j++)
    {
        for (int i = 0; i < surronding_render; i++)
        {
            chunk_index = get_chunk_index(i - get_render_distance(), j - get_render_distance());
            world[chunk_index] = create_chunk((vec2) {i - get_render_distance(), j - get_render_distance()});
        }
    }

    for (size_t i = 0; i < surronding_render * surronding_render; i++)      
    {
        create_chunk_mesh(world[i]);
    }
} 

void generate_chunks(vec2 position)
{
    int unloaded_index;
    vec2 unloaded_chunk_position;
    
    if (position.x != 0)
    {
        unloaded_chunk_position = (vec2) {get_camera_chunk_position().x + (-position.x * (render_distance + 1)), get_camera_chunk_position().y - render_distance};
        for (int n = 0; n < surronding_render; n++)
        {
            unloaded_index = get_chunk_index(unloaded_chunk_position.x, unloaded_chunk_position.y + n);
            free(world[unloaded_index]);
            world[unloaded_index] = create_chunk((vec2) {unloaded_chunk_position.x + (position.x * surronding_render), unloaded_chunk_position.y});
        }
        for (size_t i = 0; i < surronding_render; i++)  
        {
            unloaded_index = get_chunk_index(unloaded_chunk_position.x, unloaded_chunk_position.y + i);
            create_chunk_mesh(world[unloaded_index]); 
        }
    }
    
    if (position.y != 0)
    {
        unloaded_chunk_position = (vec2) {get_camera_chunk_position().x - render_distance, get_camera_chunk_position().y + (-position.y * (render_distance + 1))};
        for (int n = 0; n < surronding_render; n++)
        {
            unloaded_index = get_chunk_index(unloaded_chunk_position.x + n, unloaded_chunk_position.y);
            free(world[unloaded_index]);
            world[unloaded_index] = create_chunk((vec2) {unloaded_chunk_position.x, unloaded_chunk_position.y + (position.y * surronding_render)});
        }
        for (size_t i = 0; i < surronding_render; i++)  
        {
            unloaded_index = get_chunk_index(unloaded_chunk_position.x + i, unloaded_chunk_position.y);
            create_chunk_mesh(world[unloaded_index]); 
        }
    }
}

int get_chunk_index(int xPos, int yPos)
{
    
    int x_index = ((xPos % surronding_render) + surronding_render) % surronding_render;
    int y_index = ((yPos % surronding_render) + surronding_render) % surronding_render;    

    int flat_index = ((x_index * surronding_render) + y_index);

    return flat_index;
}
