#ifndef WORLD
#define WORLD

#include "blocks/blocks.h"
extern int number_of_chunks;
extern int surronding_render;

typedef struct Chunk Chunk;

typedef Chunk **World;

World get_world();

void world_init();
void generate_chunks();

bool chunk_exists(vec2 position);
int get_chunk_index(int xPos, int yPos);


#endif