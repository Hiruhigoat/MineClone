#ifndef BLOCKS
#define BLOCKS

#include "math/vmath.h"
#include "renderer/renderer.h"
#include "world/world.h"

#define AIR_BLOCK 0
#define SOLID_BLOCK 1

#define TOP 5
#define BOTTOM 4
#define RIGHT 3
#define LEFT 2
#define FRONT 1
#define BACK 0

#ifndef MESH_DEF
#define MESH_DEF

typedef struct
{
    GLuint vbo;
    GLuint vao;
    int vertex_count;
} Mesh;
#endif

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

typedef struct Chunk
{
    vec2 position;
    Block blocks[16][256][16];
    Mesh mesh;
    bool edited;
} Chunk;

typedef struct Chunk Chunk;
typedef Chunk **World;

void create_block(Block *b, vec2 texture);
Chunk* create_chunk(vec2 position);

void create_chunk_mesh(Chunk* chunk);


Block get_block(vec3 position);
vec2 block_to_chunk_pos(vec3 block_position);
Face* check_faces(Block block, int *num_of_faces);

#endif