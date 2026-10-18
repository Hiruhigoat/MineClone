#include "blocks/blocks.h"

static GLfloat block[] = {
        // x  |   y   |  z         u    |    v
         0.0f,  0.0f,  0.0f,     0.0f,      0.0f,    /* Face 1*/
         1.0f,  0.0f,  0.0f,     0.015625f, 0.0f,
         1.0f,  1.0f,  0.0f,     0.015625f, 0.03125f,
         1.0f,  1.0f,  0.0f,     0.015625f, 0.03125f,
         0.0f,  1.0f,  0.0f,     0.0f,      0.03125f,
         0.0f,  0.0f,  0.0f,     0.0f,      0.0f,
         0.0f,  0.0f,  1.0f,     0.0f,      0.0f,    /* Face 2*/
         1.0f,  0.0f,  1.0f,     0.015625f, 0.0f,
         1.0f,  1.0f,  1.0f,     0.015625f, 0.03125f,
         1.0f,  1.0f,  1.0f,     0.015625f, 0.03125f,
         0.0f,  1.0f,  1.0f,     0.0f,      0.03125f,
         0.0f,  0.0f,  1.0f,     0.0f,      0.0f,
         0.0f,  0.0f,  1.0f,     0.0f,      0.0f,    /* Face 3*/
         0.0f,  0.0f,  0.0f,     0.015625f, 0.0f,
         0.0f,  1.0f,  0.0f,     0.015625f, 0.03125f,
         0.0f,  1.0f,  0.0f,     0.015625f, 0.03125f,
         0.0f,  1.0f,  1.0f,     0.0f,      0.03125f,
         0.0f,  0.0f,  1.0f,     0.0f,      0.0f,
         1.0f,  0.0f,  1.0f,     0.0f,      0.0f,    /* Face 4*/
         1.0f,  0.0f,  0.0f,     0.015625f, 0.0f,
         1.0f,  1.0f,  0.0f,     0.015625f, 0.03125f,
         1.0f,  1.0f,  0.0f,     0.015625f, 0.03125f,
         1.0f,  1.0f,  1.0f,     0.0f,      0.03125f,
         1.0f,  0.0f,  1.0f,     0.0f,      0.0f,
         0.0f,  0.0f,  0.0f,     0.0f,      0.0f,    /* Face 5*/
         1.0f,  0.0f,  0.0f,     0.015625f, 0.0f,
         1.0f,  0.0f,  1.0f,     0.015625f, 0.03125f,
         1.0f,  0.0f,  1.0f,     0.015625f, 0.03125f,
         0.0f,  0.0f,  1.0f,     0.0f,      0.03125f,
         0.0f,  0.0f,  0.0f,     0.0f,      0.0f,
         0.0f,  1.0f,  0.0f,     0.0f,      0.0f,    /* Face 6*/
         1.0f,  1.0f,  0.0f,     0.015625f, 0.0f,
         1.0f,  1.0f,  1.0f,     0.015625f, 0.03125f,
         1.0f,  1.0f,  1.0f,     0.015625f, 0.03125f,
         0.0f,  1.0f,  1.0f,     0.0f,      0.03125f,
         0.0f,  1.0f,  0.0f,     0.0f,      0.0f,
};

void create_block(Block *b)
{
    for (size_t i = 0; i < 36; i++)
    {
        for (size_t k = 0; k < 5; k++)
        {
            if (k < 3)
            {
                b->vertices[i].data[k] = block[(i * 5) + k] + b->position.data[k];
            } else
            {
                b->vertices[i].data[k] = block[(i * 5) + k];
            }
        }
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
                create_block(&chunk->blocks[i][j][k]);
            
            }
        }
    }
    return chunk;
}

bool create_chunk_mesh(Chunk *chunk)
{
    float *verticies;
    float max_number_of_blocks = 16 * 16 * 256;
    float vertex_array_size = 36 * 5;
    float number_of_blocks = 0.0f;
    verticies = malloc(max_number_of_blocks * vertex_array_size * sizeof(float));

    int index = 0;
    for (int j = 0; j < 256; j++)
    {
        for (int i = 0; i < 16; i++)
        {
            for (int k = 0; k < 16; k++)
            {
                memcpy(&verticies[index], chunk->blocks[i][j][k].vertices, vertex_array_size * sizeof(float));    
                index += vertex_array_size;
                number_of_blocks++;
            }
        }
    }

    chunk->mesh.vertex_count = number_of_blocks * 36;

    // Generate and bind the vertex array object.
    glGenVertexArrays(1, &chunk->mesh.vao);
    glBindVertexArray(chunk->mesh.vao);

    // Generate and bind the vertex buffer object. Add the data from verticies  
    glGenBuffers(1, &chunk->mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, chunk->mesh.vbo);
    
    size_t total_size = chunk->mesh.vertex_count * 5 * sizeof(GLfloat);
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