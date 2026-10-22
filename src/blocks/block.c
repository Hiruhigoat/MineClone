#include "blocks/blocks.h"

#define STB_PERLIN_IMPLEMENTATION
#include "vendor/stb_perlin.h"


int base_terrian_height = 50;
Face facing_air[6];

static Vertex unit_block_verticies[36] = {
        // x  |   y   |  z         u    |    v
         0.0f,  0.0f,  0.0f,     0.0f,      0.0f,    /* Face 1 -Z*/
         1.0f,  0.0f,  0.0f,     0.015625f, 0.0f,
         1.0f,  1.0f,  0.0f,     0.015625f, 0.03125f,
         1.0f,  1.0f,  0.0f,     0.015625f, 0.03125f,
         0.0f,  1.0f,  0.0f,     0.0f,      0.03125f,
         0.0f,  0.0f,  0.0f,     0.0f,      0.0f,
         0.0f,  0.0f,  1.0f,     0.0f,      0.0f,    /* Face 2 +Z*/
         1.0f,  0.0f,  1.0f,     0.015625f, 0.0f,
         1.0f,  1.0f,  1.0f,     0.015625f, 0.03125f,
         1.0f,  1.0f,  1.0f,     0.015625f, 0.03125f,
         0.0f,  1.0f,  1.0f,     0.0f,      0.03125f,
         0.0f,  0.0f,  1.0f,     0.0f,      0.0f,
         0.0f,  0.0f,  1.0f,     0.0f,      0.0f,    /* Face 3 -X*/
         0.0f,  0.0f,  0.0f,     0.015625f, 0.0f,
         0.0f,  1.0f,  0.0f,     0.015625f, 0.03125f,
         0.0f,  1.0f,  0.0f,     0.015625f, 0.03125f,
         0.0f,  1.0f,  1.0f,     0.0f,      0.03125f,
         0.0f,  0.0f,  1.0f,     0.0f,      0.0f,
         1.0f,  0.0f,  1.0f,     0.0f,      0.0f,    /* Face 4 +X*/
         1.0f,  0.0f,  0.0f,     0.015625f, 0.0f,
         1.0f,  1.0f,  0.0f,     0.015625f, 0.03125f,
         1.0f,  1.0f,  0.0f,     0.015625f, 0.03125f,
         1.0f,  1.0f,  1.0f,     0.0f,      0.03125f,
         1.0f,  0.0f,  1.0f,     0.0f,      0.0f,
         0.0f,  0.0f,  0.0f,     0.0f,      0.0f,    /* Face 5 -Y*/
         1.0f,  0.0f,  0.0f,     0.015625f, 0.0f,
         1.0f,  0.0f,  1.0f,     0.015625f, 0.03125f,
         1.0f,  0.0f,  1.0f,     0.015625f, 0.03125f,
         0.0f,  0.0f,  1.0f,     0.0f,      0.03125f,
         0.0f,  0.0f,  0.0f,     0.0f,      0.0f,
         0.0f,  1.0f,  0.0f,     0.0f,      0.0f,    /* Face 6 +Y*/
         1.0f,  1.0f,  0.0f,     0.015625f, 0.0f,
         1.0f,  1.0f,  1.0f,     0.015625f, 0.03125f,
         1.0f,  1.0f,  1.0f,     0.015625f, 0.03125f,
         0.0f,  1.0f,  1.0f,     0.0f,      0.03125f,
         0.0f,  1.0f,  0.0f,     0.0f,      0.0f
};

World full_world;
World loaded_world;

void create_block(Block *b, vec2 texture)
{
    for (size_t i = 0; i < 36; i++)
    {
        b->vertices[i].coordinate = vec3_add(unit_block_verticies[i].coordinate, b->position);
        b->vertices[i].uv = vec2_add(unit_block_verticies[i].uv, vec2_mult(texture, (vec2){0.015625f, 0.03125f}));
    }
    int terrian_height = base_terrian_height + (stb_perlin_fbm_noise3(b->position.x * 0.01, 0.0f , b->position.z * 0.01, 2.0f, 0.5f, 6) * 10);
    b->type = (b->position.y > terrian_height) ? AIR_BLOCK : SOLID_BLOCK;
}

Chunk* create_chunk(vec2 position)
{
    Chunk *chunk = malloc(sizeof(Chunk));
    chunk->position = position;
    chunk->mesh.vao = 0;
    chunk->mesh.vbo = 0;
    chunk->mesh.vertex_count = 0;


    for (int j = 0; j < 256; j++)
    {
        for (int i = 0; i < 16; i++)
        {
            for (int k = 0; k < 16; k++)
            {
                
                chunk->blocks[i][j][k].position = 
                    vec3_add((vec3){16 * position.x, -64.0f, 16 * position.y}, (vec3){i, j, k});
                
                create_block(&chunk->blocks[i][j][k], (vec2){26,24});
                
            }
        }
    }

    return chunk;
}

Block get_block(vec3 position)
{
    full_world = get_world();
    vec2 chunk_position = block_to_chunk_pos(position);
    int chunk_index = get_chunk_index((int)chunk_position.x, (int)chunk_position.y);

    if ((full_world[chunk_index]->position.x != chunk_position.x) || (full_world[chunk_index]->position.y != chunk_position.y))
    {
        Block null_block = {
        .position = position,
        .type = AIR_BLOCK,
        .vertices = (float){0.0f}
        };
        return null_block;
    }
    
    
    vec3 block_chunk_position;
    block_chunk_position.x = position.x - (16 * chunk_position.x);
    block_chunk_position.y = (int) position.y + 64;
    block_chunk_position.z = position.z - (16 * chunk_position.y);

    if (block_chunk_position.y <= -1 || block_chunk_position.y >= 256)
    {
        Block null_block = {
            .position = vec3_add(block_chunk_position, (vec3){0,1,0}),
            .type = AIR_BLOCK,
            .vertices = (float){0.0f}
        };
        return null_block;
    }

    Block block = full_world[chunk_index]->blocks[(int)block_chunk_position.x][(int)block_chunk_position.y][(int)block_chunk_position.z];
    return block;
}

vec2 block_to_chunk_pos(vec3 block_position)
{
    vec2 block_to_chunk_pos = (vec2){(int)floorf((block_position.x / 16)), (int)floorf((block_position.z / 16))};
    return block_to_chunk_pos;
}

Block* get_adjacent_blocks(Block block)
{

}

Face* check_faces(Block block, int *num_of_faces)
{
    *num_of_faces = 0;
    if (block.type == AIR_BLOCK)
        return NULL;

    Block adjacent_blocks[6]= {
        get_block(vec3_add(block.position, (vec3){0,0,-1})),
        get_block(vec3_add(block.position, (vec3){0,0,1})),
        get_block(vec3_add(block.position, (vec3){-1,0,0})),
        get_block(vec3_add(block.position, (vec3){1,0,0})),
        get_block(vec3_add(block.position, (vec3){0,-1,0})),
        get_block(vec3_add(block.position, (vec3){0,1,0}))
    };
    
    for (size_t i = 0; i < 6; i++)
        if ((adjacent_blocks[i].type == AIR_BLOCK))
            (*num_of_faces)++;

    if (*num_of_faces == 0)
        return NULL;

    int index = 0;
    for (size_t i = 0; i < 6; i++)
    {
        if ((adjacent_blocks[i].type == AIR_BLOCK))
        {
            facing_air[index] = block.face[i];
            index++;
        }
    }

    return facing_air;
}



void create_chunk_mesh(Chunk* chunk)
{
    Face *faces;
    Vertex *verticies = malloc(255 * 16 * 16 * 6 * 6 * sizeof(Vertex));
    chunk->mesh.vertex_count = 0;

    int number_of_block_faces;

    for (int j = 30; j < 256; j++)
    {
        for (int i = 0; i < 16; i++)
        {
            for (int k = 0; k < 16; k++)
            {
                faces = check_faces(chunk->blocks[i][j][k], &number_of_block_faces);
                if (faces != NULL)
                {
                    for (size_t n = 0; n < number_of_block_faces; n++)
                    {
                        for (size_t f = 0; f < 6; f++)
                        {
                            verticies[chunk->mesh.vertex_count] = faces[n].verticies[f]; 
                            chunk->mesh.vertex_count++;
                        }
                    }
                }
            }
        }
    }

    int mesh_size = chunk->mesh.vertex_count * sizeof(Vertex);

    // Generate and bind the vertex array object.
    // glDeleteVertexArrays(1, &chunk->mesh.vao);
    glGenVertexArrays(1, &chunk->mesh.vao);
    glBindVertexArray(chunk->mesh.vao);
    
    // Generate and bind the vertex buffer object. Add the data from verticies  
    // glDeleteBuffers(1, &chunk->mesh.vbo);
    glGenBuffers(1, &chunk->mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, chunk->mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh_size, verticies, GL_DYNAMIC_DRAW);
    
    free(verticies);
    
    // Describe the vertex input and enable it
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*) (3 * (sizeof(GLfloat))));
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
