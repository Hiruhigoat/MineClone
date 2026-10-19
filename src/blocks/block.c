#include "blocks/blocks.h"

const int terrian_height = 60;

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
World world;

Block null_block = {
    .position = (vec3){0, 256 + terrian_height, 0},
    .type = AIR_BLOCK,
    .vertices = (float){0,0,0,0,0}
};
void create_block(Block *b, vec2 texture)
{
    for (size_t i = 0; i < 36; i++)
    {
        b->vertices[i].coordinate = vec3_add(unit_block_verticies[i].coordinate, b->position);
        b->vertices[i].uv = vec2_add(unit_block_verticies[i].uv, vec2_mult(texture, (vec2){0.015625f, 0.03125f}));
    }
}

Chunk* create_chunk(vec2 position)
{
    Chunk *chunk = malloc(sizeof(Chunk));
    chunk->position = position;
    
    for (int j = 0; j < 256; j++)
    {
        for (int i = 0; i < 16; i++)
        {
            for (int k = 0; k < 16; k++)
            {
                chunk->blocks[i][j][k].position = 
                    vec3_add((vec3){16 * position.x, -64.0f, 16 * position.y}, (vec3){i, j, k});
                
                    create_block(&chunk->blocks[i][j][k], (vec2){26,24});
                chunk->blocks[i][j][k].type = (j > terrian_height) ? AIR_BLOCK : SOLID_BLOCK;
            }
        }
    }
    return chunk;
}



Face* faces_air(Chunk *chunk, Block block, int *num_of_faces)
{
    world = get_world();
    Block adjacent_chunk_block;
    Block adjacent_chunk_2_block;
    Face *facing_air;
    int chunk_index = get_chunk_index(chunk->position.x, chunk->position.y);
    int adjacent_chunk_index;
    int adjacent_chunk_2_index;
    int x = block.position.x;
    int y = block.position.y;
    int z = block.position.z;


    if ((y == 0) || (y == 255) || block.type == AIR_BLOCK)
        return NULL;

    switch (x)
    {
        case 0:
        {
            switch (z)
            {
                case 0:
                {
                    adjacent_chunk_index = get_chunk_index(chunk->position.x - 1, chunk->position.y);
                    adjacent_chunk_2_index = get_chunk_index(chunk->position.x, chunk->position.y - 1);
                    adjacent_chunk_block = world[adjacent_chunk_index]->blocks[15][y][0];
                    adjacent_chunk_2_block = world[adjacent_chunk_2_index]->blocks[0][y][15];
                    if (((world[adjacent_chunk_index] == NULL) || (adjacent_chunk_block.type == AIR_BLOCK)) && ((world[adjacent_chunk_2_index] == NULL) || (adjacent_chunk_2_block.type == AIR_BLOCK)))
                    {
                        *num_of_faces = (chunk->blocks[x][y + 1][z].type == AIR_BLOCK) ? 3 : 2;
                        facing_air = malloc(*num_of_faces * sizeof(Face));
                        memcpy(&facing_air[0], &block.face[BACK], sizeof(Face));
                        memcpy(&facing_air[1], &block.face[LEFT], sizeof(Face));
                        if (*num_of_faces == 3)
                            memcpy(&facing_air[2], &block.face[TOP], sizeof(Face));
                        return facing_air;
                    } else if (((world[adjacent_chunk_index] == NULL) || (adjacent_chunk_block.type == AIR_BLOCK)) || ((world[adjacent_chunk_2_index] == NULL) || (adjacent_chunk_2_block.type == AIR_BLOCK)))
                    {
                        *num_of_faces = (chunk->blocks[x][y + 1][z].type == AIR_BLOCK) ? 2 : 1;
                        facing_air = malloc(*num_of_faces * sizeof(Face));
                        int direction = (world[adjacent_chunk_index] == NULL) ? LEFT : BACK;
                        memcpy(&facing_air[0], &block.face[direction], sizeof(Face));
                        if (*num_of_faces == 2)
                            memcpy(&facing_air[1], &block.face[TOP], sizeof(Face));
                        return facing_air;
                    }
                    return NULL;
                }
                case 15:
                {
                    adjacent_chunk_index = get_chunk_index(chunk->position.x - 1, chunk->position.y);
                    adjacent_chunk_2_index = get_chunk_index(chunk->position.x, chunk->position.y + 1);
                    adjacent_chunk_block = world[adjacent_chunk_index]->blocks[15][y][15];
                    adjacent_chunk_2_block = world[adjacent_chunk_2_index]->blocks[0][y][0];
                    if (((world[adjacent_chunk_index] == NULL) || (adjacent_chunk_block.type == AIR_BLOCK)) && ((world[adjacent_chunk_2_index] == NULL) || (adjacent_chunk_2_block.type == AIR_BLOCK)))
                    {
                        *num_of_faces = (chunk->blocks[x][y + 1][z].type == AIR_BLOCK) ? 3 : 2;
                        facing_air = malloc(*num_of_faces * sizeof(Face));
                        memcpy(&facing_air[0], &block.face[FRONT], sizeof(Face));
                        memcpy(&facing_air[1], &block.face[LEFT], sizeof(Face));
                        if (*num_of_faces == 3)
                            memcpy(&facing_air[2], &block.face[TOP], sizeof(Face));
                        return facing_air;
                    } else if (((world[adjacent_chunk_index] == NULL) || (adjacent_chunk_block.type == AIR_BLOCK)) || ((world[adjacent_chunk_2_index] == NULL) || (adjacent_chunk_2_block.type == AIR_BLOCK)))
                    {
                        *num_of_faces = (chunk->blocks[x][y + 1][z].type == AIR_BLOCK) ? 2 : 1;
                        facing_air = malloc(*num_of_faces * sizeof(Face));
                        int direction = (world[adjacent_chunk_index] == NULL) ? LEFT : FRONT;
                        memcpy(&facing_air[0], &block.face[direction], sizeof(Face));
                        if (*num_of_faces == 2)
                            memcpy(&facing_air[1], &block.face[TOP], sizeof(Face));
                        return facing_air;
                    }
                    return NULL;
                }
                default:
                    adjacent_chunk_index = get_chunk_index(chunk->position.x - 1, chunk->position.y);
                    adjacent_chunk_block = world[adjacent_chunk_index]->blocks[15][y][z];
                    if ((world[adjacent_chunk_index] == NULL) || (adjacent_chunk_block.type == AIR_BLOCK))
                    {
                        *num_of_faces = (chunk->blocks[x][y + 1][z].type == AIR_BLOCK) ? 2 : 1;
                        facing_air = malloc(*num_of_faces * sizeof(Face));
                        memcpy(&facing_air[0], &block.face[LEFT], sizeof(Face));
                        if (*num_of_faces == 1)
                            memcpy(&facing_air[1], &block.face[TOP], sizeof(Face));
                        return facing_air;
                    }
                    return NULL;
            }
        }
        case 15:
        {
            switch (z)
            {
                case 0:
                {
                    adjacent_chunk_index = get_chunk_index(chunk->position.x + 1, chunk->position.y);
                    adjacent_chunk_2_index = get_chunk_index(chunk->position.x, chunk->position.y - 1);
                    adjacent_chunk_block = world[adjacent_chunk_index]->blocks[0][y][0];
                    adjacent_chunk_2_block = world[adjacent_chunk_2_index]->blocks[15][y][15];
                    if (((world[adjacent_chunk_index] == NULL) || (adjacent_chunk_block.type == AIR_BLOCK)) && ((world[adjacent_chunk_2_index] == NULL) || (adjacent_chunk_2_block.type == AIR_BLOCK)))
                    {
                        *num_of_faces = (chunk->blocks[x][y + 1][z].type == AIR_BLOCK) ? 3 : 2;
                        facing_air = malloc(*num_of_faces * sizeof(Face));
                        memcpy(&facing_air[0], &block.face[BACK], sizeof(Face));
                        memcpy(&facing_air[1], &block.face[RIGHT], sizeof(Face));
                        if (*num_of_faces == 3)
                            memcpy(&facing_air[2], &block.face[TOP], sizeof(Face));
                        return facing_air;
                    } else if (((world[adjacent_chunk_index] == NULL) || (adjacent_chunk_block.type == AIR_BLOCK)) || ((world[adjacent_chunk_2_index] == NULL) || (adjacent_chunk_2_block.type == AIR_BLOCK)))
                    {
                        *num_of_faces = (chunk->blocks[x][y + 1][z].type == AIR_BLOCK) ? 2 : 1;
                        facing_air = malloc(*num_of_faces * sizeof(Face));
                        int direction = (world[adjacent_chunk_index] == NULL) ? RIGHT : BACK;
                        memcpy(&facing_air[0], &block.face[direction], sizeof(Face));
                        if (*num_of_faces == 2)
                            memcpy(&facing_air[1], &block.face[TOP], sizeof(Face));
                        return facing_air;
                    }
                    return NULL;
                }
                case 15:
                {
                    adjacent_chunk_index = get_chunk_index(chunk->position.x + 1, chunk->position.y);
                    adjacent_chunk_2_index = get_chunk_index(chunk->position.x, chunk->position.y + 1);
                    adjacent_chunk_block = world[adjacent_chunk_index]->blocks[0][y][15];
                    adjacent_chunk_2_block = world[adjacent_chunk_2_index]->blocks[15][y][0];
                    if (((world[adjacent_chunk_index] == NULL) || (adjacent_chunk_block.type == AIR_BLOCK)) && ((world[adjacent_chunk_2_index] == NULL) || (adjacent_chunk_2_block.type == AIR_BLOCK)))
                    {
                        *num_of_faces = (chunk->blocks[x][y + 1][z].type == AIR_BLOCK) ? 3 : 2;
                        facing_air = malloc(*num_of_faces * sizeof(Face));
                        memcpy(&facing_air[0], &block.face[FRONT], sizeof(Face));
                        memcpy(&facing_air[1], &block.face[RIGHT], sizeof(Face));
                        if (*num_of_faces == 3)
                            memcpy(&facing_air[2], &block.face[TOP], sizeof(Face));
                        return facing_air;
                    } else if (((world[adjacent_chunk_index] == NULL) || (adjacent_chunk_block.type == AIR_BLOCK)) || ((world[adjacent_chunk_2_index] == NULL) || (adjacent_chunk_2_block.type == AIR_BLOCK)))
                    {
                        *num_of_faces = (chunk->blocks[x][y + 1][z].type == AIR_BLOCK) ? 2 : 1;
                        facing_air = malloc(*num_of_faces * sizeof(Face));
                        int direction = (world[adjacent_chunk_index] == NULL) ? RIGHT : FRONT;
                        memcpy(&facing_air[0], &block.face[direction], sizeof(Face));
                        if (*num_of_faces == 2)
                            memcpy(&facing_air[1], &block.face[TOP], sizeof(Face));
                        return facing_air;
                    }
                    return NULL;
                }
                default:
                    adjacent_chunk_index = get_chunk_index(chunk->position.x + 1, chunk->position.y);
                    adjacent_chunk_block = world[adjacent_chunk_index]->blocks[0][y][z];
                    if ((world[adjacent_chunk_index] == NULL) || (adjacent_chunk_block.type == AIR_BLOCK))
                    {
                        *num_of_faces = (chunk->blocks[x][y + 1][z].type == AIR_BLOCK) ? 2 : 1;
                        facing_air = malloc(*num_of_faces * sizeof(Face));
                        memcpy(&facing_air[0], &block.face[RIGHT], sizeof(Face));
                        if (*num_of_faces == 1)
                            memcpy(&facing_air[1], &block.face[TOP], sizeof(Face));
                        return facing_air;
                    }
                    return NULL;
            }
            
        }
        default:
            switch (z)
            {
                case 0:
                {
                    adjacent_chunk_index = get_chunk_index(chunk->position.x, chunk->position.y - 1);
                    adjacent_chunk_block = world[adjacent_chunk_index]->blocks[x][y][15];
                    if ((world[adjacent_chunk_index] == NULL) || (adjacent_chunk_block.type == AIR_BLOCK))
                    {
                        *num_of_faces = (chunk->blocks[x][y + 1][z].type == AIR_BLOCK) ? 2 : 1;
                        facing_air = malloc(*num_of_faces * sizeof(Face));
                        memcpy(&facing_air[0], &block.face[BACK], sizeof(Face));
                        if (*num_of_faces == 1)
                            memcpy(&facing_air[1], &block.face[TOP], sizeof(Face));
                        return facing_air;
                    }
                    return NULL;
                }
                case 15:
                {
                    adjacent_chunk_index = get_chunk_index(chunk->position.x, chunk->position.y + 1);
                    adjacent_chunk_block = world[adjacent_chunk_index]->blocks[x][y][0];
                    if ((world[adjacent_chunk_index] == NULL) || (adjacent_chunk_block.type == AIR_BLOCK))
                    {
                        *num_of_faces = (chunk->blocks[x][y + 1][z].type == AIR_BLOCK) ? 2 : 1;
                        facing_air = malloc(*num_of_faces * sizeof(Face));
                        memcpy(&facing_air[0], &block.face[FRONT], sizeof(Face));
                        if (*num_of_faces == 1)
                            memcpy(&facing_air[1], &block.face[TOP], sizeof(Face));
                        return facing_air;
                    }
                    return NULL;
                }
                default:
                    break;
            }
    }
    

}

Block get_block(vec3 position)
{
    world = get_world();
    vec2 chunk_position = block_to_chunk_pos(position);
    if (!chunk_exists(chunk_position))
        return null_block;
    int chunk_index = get_chunk_index(chunk_position.x, chunk_position.y);
    vec3 block_chunk_position;
    block_chunk_position.x = position.x - (16 * chunk_position.x);
    block_chunk_position.y = (int) position.y;
    block_chunk_position.z = position.z - (16 * chunk_position.y);

    Block block = world[chunk_index]->blocks[(int)block_chunk_position.x][(int)block_chunk_position.y][(int)block_chunk_position.z];
    return block;
}

vec2 block_to_chunk_pos(vec3 block_position)
{
    return (vec2){(int)floorf((block_position.x / 16)), (int)floorf((block_position.z / 16))};
}

Face* check_faces(Block block, int *num_of_faces)
{
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

    Face *facing_air = malloc(*num_of_faces * sizeof(Face));

    int index = 0;
    for (size_t i = 0; i < 6; i++)
    {
        if ((adjacent_blocks[i].type == AIR_BLOCK))
        {
            memcpy(&facing_air[index], &block.face[i], sizeof(Face));
            index++;
        }
    }

    return facing_air;
}


bool create_chunk_mesh(World world)
{
    world = get_world();
    Vertex *verticies;
    Face *faces;
    int number_of_block_faces;
    
    int count_faces;
    int chunk_index;

    for (int cY = 0; cY < surronding_render; cY++)
    {
        for (int cX = 0; cX < surronding_render; cX++)
        {
            faces = NULL;
            chunk_index = get_chunk_index(cX, cY);
            count_faces = 0;
            for (int j = 0; j < 256; j++)
            {
                for (int i = 0; i < 16; i++)
                {
                    for (int k = 0; k < 16; k++)
                    {
                        // faces = faces_air(world[chunk_index], world[chunk_index]->blocks[i][j][k], &number_of_block_faces);
                        faces = check_faces(world[chunk_index]->blocks[i][j][k], &number_of_block_faces);
                        if (faces != NULL && world[chunk_index]->blocks[i][j][k].type != AIR_BLOCK)
                        {
                            verticies = realloc(verticies, (count_faces + number_of_block_faces) * sizeof(Face));
                            memcpy(&verticies[count_faces * 6], faces, number_of_block_faces * sizeof(Face));    
                            count_faces += number_of_block_faces;
                        }
                        free(faces);
                    }
                }
            }
            world[chunk_index]->mesh.vertex_count = count_faces * 6;

            // Generate and bind the vertex array object.
            glGenVertexArrays(1, &world[chunk_index]->mesh.vao);
            glBindVertexArray(world[chunk_index]->mesh.vao);

            // Generate and bind the vertex buffer object. Add the data from verticies  
            glGenBuffers(1, &world[chunk_index]->mesh.vbo);
            glBindBuffer(GL_ARRAY_BUFFER, world[chunk_index]->mesh.vbo);
            
            size_t total_size = count_faces * sizeof(Face);
            glBufferData(GL_ARRAY_BUFFER, total_size, verticies, GL_STATIC_DRAW);
            free(verticies);

            // Describe the vertex input and enable it
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*) (3 * (sizeof(GLfloat))));

            glBindVertexArray(0);
        }
    }

    return true;
}
