#ifndef WORLD
#define WORLD

#include "blocks/blocks.h"


extern int number_of_chunks;
extern int surronding_render;

typedef struct Chunk Chunk;


#ifndef MESH_DEF
#define MESH_DEF

typedef struct
{
    GLuint vbo;
    GLuint vao;
    int vertex_count;
} Mesh;
#endif

typedef Chunk **World;

World get_world();

void world_init();
void generate_chunks(vec2 position);

bool chunk_exists(vec2 position);
int get_chunk_index(int xPos, int yPos);

int get_render_distance();
Mesh get_rendering_mesh();

#endif