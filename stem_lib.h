

#include <stdint.h>


typedef struct nodeTAG
{
    int64_t count; // 8 bytes
    void * down; // 4 bytes
    void * right; // 4 bytes
    char stem[8]; // 8 bytes
}node;


node * split(node * head, char * remainder);
node * add(node * head, char * remainder);
node * count(node * head, char * remainder);
int64_t find_node(node * head, char * remainder);
void delete_nodes(node * head);
node * new_node(char * stem, int64_t count);

