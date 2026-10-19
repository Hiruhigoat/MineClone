#ifndef BLOCKS
#define BLOCKS

#include "math/vmath.h"
#include "renderer/renderer.h"

#define AIR_BLOCK 0
#define SOLID_BLOCK 1

#define TOP 5
#define BOTTOM 4
#define RIGHT 3
#define LEFT 2
#define FRONT 1
#define BACK 0

typedef struct
{
    GLuint vbo;
    GLuint vao;
    int vertex_count;
} Mesh;

typedef union
{
    struct {vec3 coordinate; vec2 uv;};
    float data[5];
} Vertex;

typedef struct
{
    Vertex verticies[6];
} Face;

typedef union 
{
    struct {vec3 position; int type; Vertex vertices[36];};
    struct {float block_data[4]; Face face[6];};
    float data[4 + (36 * 9)];
} Block;

typedef struct
{
    vec3 position;
    Block blocks[16][256][16];
    Mesh mesh;
} Chunk;

void create_block(Block *b, vec2 texture);
void create_mesh(Block *vertices, int vertex_count);
Chunk* create_chunk(vec3 position);

bool create_chunk_mesh(Chunk *chunk);

#endif