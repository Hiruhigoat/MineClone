#include "blocks/blocks.h"

#define STB_PERLIN_IMPLEMENTATION
#include "vendor/stb_perlin.h"


int base_terrian_height = 50;
Face facing_air[6];

static Face unit_block_faces[6] = {
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

static const Block_Data BLOCK_PROPERTIES[BLOCK_COUNT] =
{
    [NULL_BLOCK] = {
        .name = "Null",
        .uv_top = (vec2) {0,0},
        .uv_bottom = (vec2) {0,0},
        .uv_side = (vec2) {0,0},
        .is_solid = false
    },
    [AIR_BLOCK] = {
        .name = "Air",
        .uv_top = (vec2) {0,0},
        .uv_bottom = (vec2) {0,0},
        .uv_side = (vec2) {0,0},
        .is_solid = false
    },
    [GRASS_BLOCK] = {
        .name = "Grass",
        .uv_top = (vec2) {8,14},
        .uv_bottom = (vec2) {30,13},
        .uv_side = (vec2) {25,26},
        .is_solid = true
    },
    [STONE_BLOCK] = {
        .name = "Stone",
        .uv_top = (vec2) {8,6},
        .uv_bottom = (vec2) {8,6},
        .uv_side = (vec2) {8,6},
        .is_solid = true
    },
    [SAND_BLOCK] = {
        .name = "Sand",
        .uv_top = (vec2) {4,8},
        .uv_bottom = (vec2) {4,8},
        .uv_side = (vec2) {4,8},
        .is_solid = true
    },

};

World full_world;
World loaded_world;

vec2 atlas = {0.015625f, 0.03125f};


Chunk* create_chunk(vec2 position)
{
    Chunk *chunk = malloc(sizeof(Chunk));
    chunk->edited = false;
    chunk->position = position;
    chunk->mesh.vao = 0;
    chunk->mesh.vbo = 0;
    chunk->mesh.vertex_count = 0;


    for (int i = 0; i < 16; i++)
    {
        for (int k = 0; k < 16; k++)
        {
            int terrian_height = base_terrian_height + (stb_perlin_fbm_noise3(((position.x * 16) + i) * 0.02, 0.0f , ((position.y * 16) + k) * 0.02, 2.0f, 0.5f, 6) * 20);
            for (int j = 0; j < 256; j++)
            {
                if (j > terrian_height)
                {
                    chunk->blocks[i][j][k] = AIR_BLOCK;
                } else if (j >= terrian_height - 7)
                {
                    chunk->blocks[i][j][k] = GRASS_BLOCK;
                } else
                {
                    chunk->blocks[i][j][k] = STONE_BLOCK;
                }
            }
        }
    }
    return chunk;
}


Block get_block(vec3 position)
{
    full_world = get_world();
    vec2 chunk_position = block_to_chunk_pos(position);
    int chunk_index = get_chunk_index((int) chunk_position.x, (int) chunk_position.y);


    if ((abs(chunk_position.x - get_camera_chunk_position().x) > get_render_distance()) || (abs(chunk_position.y - get_camera_chunk_position().y) > get_render_distance()) || (chunk_index >= surronding_render * surronding_render))
        return NULL_BLOCK;
    
    
    vec3 block_chunk_position;
    block_chunk_position.x = position.x - (16 * chunk_position.x);
    block_chunk_position.y = (int) position.y + 64;
    block_chunk_position.z = position.z - (16 * chunk_position.y);

    if (block_chunk_position.y <= -1 || block_chunk_position.y >= 256)
        return NULL_BLOCK;

    Block blocked = full_world[chunk_index]->blocks[(int)block_chunk_position.x][(int)block_chunk_position.y][(int)block_chunk_position.z];
    return blocked;
}

vec2 block_to_chunk_pos(vec3 block_position)
{
    vec2 block_to_chunk_pos = (vec2){(int)floorf((block_position.x / 16)), (int)floorf((block_position.z / 16))};
    return block_to_chunk_pos;
}


Face* check_faces(Chunk* chunk, vec3 block_chunk_position, int *num_of_faces)
{
    *num_of_faces = 0;

    vec2 chunk_position = chunk->position;
    vec3 block_position = vec3_add((vec3){16 * chunk_position.x, -64.0f, 16 * chunk_position.y}, block_chunk_position);
    Block block_type = chunk->blocks[(int) block_chunk_position.x][(int) block_chunk_position.y][(int) block_chunk_position.z];

    if (!BLOCK_PROPERTIES[block_type].is_solid)
        return NULL;

    Block adjacent_blocks[6]= {
        get_block(vec3_add(block_position, (vec3){0,0,-1})),
        get_block(vec3_add(block_position, (vec3){0,0,1})),
        get_block(vec3_add(block_position, (vec3){-1,0,0})),
        get_block(vec3_add(block_position, (vec3){1,0,0})),
        get_block(vec3_add(block_position, (vec3){0,-1,0})),
        get_block(vec3_add(block_position, (vec3){0,1,0}))
    };
    
    for (size_t i = 0; i < 6; i++)
        if (!BLOCK_PROPERTIES[adjacent_blocks[i]].is_solid)
            (*num_of_faces)++;

    if (*num_of_faces == 0)
        return NULL;

    int index = 0;
    for (size_t i = 0; i < 6; i++)
    {
        if (!BLOCK_PROPERTIES[adjacent_blocks[i]].is_solid)
        {
            Face face =
            {

                
                .verticies = {
                    vec3_add(unit_block_faces[i].verticies[0].coordinate, block_position),
                    vec2_add(unit_block_faces[i].verticies[0].uv, vec2_mult(BLOCK_PROPERTIES[block_type].uv_top, atlas)),
                    vec3_add(unit_block_faces[i].verticies[1].coordinate, block_position),
                    vec2_add(unit_block_faces[i].verticies[1].uv, vec2_mult(BLOCK_PROPERTIES[block_type].uv_top, atlas)),
                    vec3_add(unit_block_faces[i].verticies[2].coordinate, block_position),
                    vec2_add(unit_block_faces[i].verticies[2].uv, vec2_mult(BLOCK_PROPERTIES[block_type].uv_top, atlas)),
                    vec3_add(unit_block_faces[i].verticies[3].coordinate, block_position),
                    vec2_add(unit_block_faces[i].verticies[3].uv, vec2_mult(BLOCK_PROPERTIES[block_type].uv_top, atlas)),
                    vec3_add(unit_block_faces[i].verticies[4].coordinate, block_position),
                    vec2_add(unit_block_faces[i].verticies[4].uv, vec2_mult(BLOCK_PROPERTIES[block_type].uv_top, atlas)),
                    vec3_add(unit_block_faces[i].verticies[5].coordinate, block_position),
                    vec2_add(unit_block_faces[i].verticies[5].uv, vec2_mult(BLOCK_PROPERTIES[block_type].uv_top, atlas)),
                }
            };
 
            facing_air[index] = face;          
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

    for (int j = 0; j < 256; j++)
    {
        for (int i = 0; i < 16; i++)
        {
            for (int k = 0; k < 16; k++)
            {
                
                faces = check_faces(chunk, (vec3) {i, j, k}, &number_of_block_faces);
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
    glDeleteVertexArrays(1, &chunk->mesh.vao);
    glGenVertexArrays(1, &chunk->mesh.vao);
    glBindVertexArray(chunk->mesh.vao);
    
    // Generate and bind the vertex buffer object. Add the data from verticies  
    glDeleteBuffers(1, &chunk->mesh.vbo);
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
