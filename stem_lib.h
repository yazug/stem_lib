

#include <stdint.h>
#include <stdio.h>


typedef struct nodeTAG
{
    int64_t count; // 8 bytes
    void * down; // 4 bytes
    void * right; // 4 bytes
    char stem[8]; // 8 bytes
}node;


node * split(node * head, const char * stem);
node * add(node * head, const char * stem, int64_t word_count);
node * count(node * head, const char * stem, int64_t word_count);
int64_t find_node(node * head, const char * stem);
void delete_nodes(node * head);
node * new_node(const char * stem, int64_t count);
size_t prettyprintEntries(node * head, const char * prefix);
size_t prettyprintTree(node * head, const char * prefix);

size_t writeoutEntries(node * head, const char * prefix, FILE* fp);
size_t writeoutRadixFiles(node * head, const char * folder_name);

void delete_all_storage(void);

size_t getNodesUsed(void);

