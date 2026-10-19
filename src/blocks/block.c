#include "blocks/blocks.h"

int terrian_height = 200;

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

void create_block(Block *b, vec2 texture)
{
    for (size_t i = 0; i < 36; i++)
    {
        b->vertices[i].coordinate = vec3_add(unit_block_verticies[i].coordinate, b->position);
        b->vertices[i].uv = vec2_add(unit_block_verticies[i].uv, vec2_mult(texture, (vec2){0.015625f, 0.03125f}));
    }
}

Chunk* create_chunk(vec3 position)
{
    Chunk *chunk = malloc(sizeof(Chunk));
    chunk->position = position;
    
    for (int j = 0; j < 256; j++)
    {
        for (int i = 0; i < 16; i++)
        {
            for (int k = 0; k < 16; k++)
            {
                chunk->blocks[i][j][k].position = vec3_add(vec3_scale(position, (vec3){16, 1, 16}),
                                                                            (vec3){i, j, k});
                create_block(&chunk->blocks[i][j][k], (vec2){26,24});
                chunk->blocks[i][j][k].type = (j > terrian_height) ? AIR_BLOCK : SOLID_BLOCK;
            }
        }
    }
    return chunk;
}

Face* faces_air(Chunk *chunk, int x, int y, int z, int *num_of_faces)
{
    Block block = chunk->blocks[x][y][z];
    
    if ((x == 0 || x == 15) || (y == 0 || y == 255) || (z == 0 || z == 15))
        return NULL;
    
    int block_top    = chunk->blocks[x][y + 1][z].type == AIR_BLOCK ? 1 : 0;
    int block_bottom = chunk->blocks[x][y - 1][z].type == AIR_BLOCK ? 1 : 0;
    int block_right  = chunk->blocks[x + 1][y][z].type == AIR_BLOCK ? 1 : 0;
    int block_left   = chunk->blocks[x - 1][y][z].type == AIR_BLOCK ? 1 : 0;
    int block_front  = chunk->blocks[x][y][z + 1].type == AIR_BLOCK ? 1 : 0;
    int block_back   = chunk->blocks[x][y][z - 1].type == AIR_BLOCK ? 1 : 0;
    
    *num_of_faces = block_top + block_bottom + block_right + block_left + block_front + block_back;
    
    if (*num_of_faces == 0)
        return NULL;

    Face *facing_air = malloc(*num_of_faces * sizeof(Face));
    int index = 0;
    if (block_top)
    {
        memcpy(&facing_air[index], &block.face[TOP], sizeof(Face));
        index++;
    }
    if (block_bottom)
    {
        memcpy(&facing_air[index], &block.face[BOTTOM], sizeof(Face));
        index++;
    }
    if (block_right)
    {
        memcpy(&facing_air[index], &block.face[RIGHT], sizeof(Face));
        index++;
    }
    if (block_left)
    {
        memcpy(&facing_air[index], &block.face[LEFT], sizeof(Face));
        index++;
    }
    if (block_front)
    {
        memcpy(&facing_air[index], &block.face[BACK], sizeof(Face));
        index++;
    }
    if (block_back)
    {
        memcpy(&facing_air[index], &block.face[FRONT], sizeof(Face));
        index++;
    }
    
    return facing_air;
}

bool create_chunk_mesh(Chunk *chunk)
{
    Vertex *verticies = NULL;
    Face *faces = NULL;
    int number_of_block_faces;
    
    int count_faces = 0;

    for (int j = 0; j < 256; j++)
    {
        for (int i = 0; i < 16; i++)
        {
            for (int k = 0; k < 16; k++)
            {
                faces = faces_air(chunk, i, j, k, &number_of_block_faces);
                if (faces != NULL && chunk->blocks[i][j][k].type != AIR_BLOCK)
                {
                    verticies = realloc(verticies, (count_faces + number_of_block_faces) * sizeof(Face));
                    memcpy(&verticies[count_faces * 6], faces, number_of_block_faces * sizeof(Face));    
                    count_faces += number_of_block_faces;
                }
                free(faces);
            }
        }
    }

    chunk->mesh.vertex_count = count_faces * 6;

    // Generate and bind the vertex array object.
    glGenVertexArrays(1, &chunk->mesh.vao);
    glBindVertexArray(chunk->mesh.vao);

    // Generate and bind the vertex buffer object. Add the data from verticies  
    glGenBuffers(1, &chunk->mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, chunk->mesh.vbo);
    
    size_t total_size = count_faces * sizeof(Face);
    glBufferData(GL_ARRAY_BUFFER, total_size, verticies, GL_STATIC_DRAW);
    free(verticies);

    // Describe the vertex input and enable it
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*) (3 * (sizeof(GLfloat))));

    glBindVertexArray(0);

    if (&chunk->mesh == NULL)
    {
        return false;
    }

    return true;
}