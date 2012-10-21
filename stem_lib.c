#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "stem_lib.h"

static size_t node_count = 0;

//then build up the tree using word stems
//if you have a new word that is part of another block split the block move the down entries with the new bock
//
//
//
node * split(node * head, const char * stem)
{
    if(head)
    {
        size_t i=0;
        size_t j=0;
        node * next_node = new_node("",0);

        while(head->stem[i] == stem[i] && stem[i] != '\0' && i < sizeof(head->stem))
            i++;

        while(head->stem[i] != '\0'  && i < sizeof(head->stem))
        {
            next_node->stem[j++] = head->stem[i];
            head->stem[i] = '\0';
            next_node->count = head->count;
            head->count = 0;
        }
        next_node->down = head->down;
        head->down = next_node;
    }
    return head;
}

node * add(node * head, const char * stem, int64_t word_count)
{
    if(head && stem)
    {
        node * next_node = new_node(stem,word_count);
        next_node->right = head;
        head = next_node;
    }
    return head;
}

node * count(node * head, const char * stem, int64_t word_count)
{
    if(NULL == head)
    {
        head = new_node(stem,word_count);
    }
    else if(head && stem)
    {
        node * cur = head;
        node * prev = head;

        while(cur)
        {
            if(cur->stem[0] < stem[0])
            {
                if(cur->right)
                {
                    prev = cur;
                    cur = (node*)(cur->right);
                }
                else
                {
                    cur->right = new_node(stem,word_count);
                    break;
                }
                continue;
            }
            else if(cur->stem[0] > stem[0])
            {
                if(cur == head)
                {
                    head = new_node(stem,word_count);
                    head->right = cur;
                }
                else
                {
                    node * next_node = new_node(stem,word_count);
                    prev->right = next_node;
                    next_node->right = cur;
                }
                break;
            }
            else
            {   // We have a match on this node
                // figure out if we need to split current node, move on to child, or count this node

                size_t i = 0;
                while(cur->stem[i] == stem[i] && stem[i] != '\0' && i < sizeof(cur->stem))
                    i++;

                if(stem[i] == '\0')
                {
                    cur->count += word_count;
                }
                else
                {
                    if(cur->stem[i] != '\0')
                    {
                        cur = split(cur,stem);
                    }
                    cur->down = count(cur->down,&stem[i],word_count);
                }

                break;
            }
        }
    }
    return head;
}


int64_t find_node(node * head, const char * stem)
{
    int64_t count = 0;

    if(head && stem)
    {
        node * cur = head;
        while(cur)
        {
            if(cur->stem[0] < stem[0])
            {
                cur = (node*)(cur->right);
                continue;
            }
            else if(cur->stem[0] > stem[0])
            {
                break;
            }
            else
            {
                size_t i = 0;
                while(cur->stem[i] == stem[i] && stem[i] != '\0' && i < sizeof(cur->stem))
                    i++;

                if(stem[i] == '\0')
                {
                    count = cur->count;
                }
                else
                {
                    count = find_node(cur->down, &stem[i]);
                }
                break;
            }
        }
    }

    return count;
}

struct memblock
{
    void * next_block;
    void * memory;
    size_t sizeof_memory;
};

struct memblock * block_list = NULL;
node * free_list = NULL;

/**
 * Cleanup the head and put all of the nodes in a free list
 */
void delete_nodes(node * head)
{
    if(head)
    {
        if(head->down)
        {
            delete_nodes(head->down);
            head->down = NULL;
        }
        if(head->right)
        {
            delete_nodes(head->right);
            head->right = NULL;
        }
        head->right = free_list;
        free_list = head;
        node_count--;
    }
}

void add_more_nodes_to_freelist(size_t num_nodes)
{
    if( num_nodes > 0)
    {
        struct memblock * block = (struct memblock*)malloc(sizeof(struct memblock));
        if(block)
        {
            size_t i;

            memset(block,0,sizeof(struct memblock));
            block->sizeof_memory = sizeof(node)*num_nodes;
            block->memory = malloc(block->sizeof_memory);
            memset(block->memory,0,block->sizeof_memory);
            node * next_node = (node*)block->memory;
            for(i = 0; i< num_nodes; i++)
            {
                next_node[i].right = free_list;
                free_list = &next_node[i];
            }
            block->next_block = block_list;
            block_list = block;
        }
    }
}

node * new_node(const char * stem, int64_t count)
{
    node * next_node = NULL;
    if(stem)
    {
        if(free_list == NULL)
            add_more_nodes_to_freelist(1000);

        // pop off one from the free list
        next_node = free_list;
        free_list = (next_node?next_node->right:NULL);

        node_count++;
        if(next_node)
        {
            memset(next_node,0,sizeof(node));
            snprintf(next_node->stem,sizeof(next_node->stem),"%s",stem);
            if(0 == strcmp(next_node->stem,stem))
            {
                next_node->count = count;
            }
            else if(strlen(stem) > strlen(next_node->stem))
            {
                next_node->down = new_node(&stem[strlen(next_node->stem)],count);
            }
        }
    }
    return next_node;
}

void delete_all_storage()
{
    while(block_list)
    {
        struct memblock * block = block_list;

        block_list = block_list->next_block;
        if(block)
        {
            free(block->memory);
            block->memory = NULL;
            block->sizeof_memory = 0;
            block->next_block = NULL;
            free(block);
        }
    }

    free_list = NULL;

}

size_t prettyprintTree(node * head, const char * prefix)
{
    size_t num_nodes = 0;
    if(head && prefix)
    {
        printf("%lld|%s %s|\n",head->count,prefix,head->stem);
	num_nodes ++;

        if(head->right)
        {
            num_nodes += prettyprintTree(head->right,prefix);
        }

        if(head->down)
        {
            char word[40] = {0};
            snprintf(word,sizeof(word),"%s%s",prefix,head->stem);
            num_nodes += prettyprintTree(head->down,word);
        }
    }
    return num_nodes;
}

size_t prettyprintEntries(node * head, const char * prefix)
{
    size_t num_nodes = 0;
    if(head && prefix)
    {
        if(head->count > 0)
            printf("%lld|%s%s|\n",head->count,prefix,head->stem);
        num_nodes++;

        if(head->down)
        {
            char word[1024] = {0};
            snprintf(word,sizeof(word),"%s%s",prefix,head->stem);
            num_nodes += prettyprintEntries(head->down,word);
        }

        if(head->right)
        {
            num_nodes += prettyprintEntries(head->right,prefix);
        }
    }
    return num_nodes;
}

size_t writeoutEntries(node * head, const char * prefix, FILE* fp)
{
    size_t entry_count = 0;
    if(head && prefix)
    {
        if(head->count > 0)
        {
            fprintf(fp,"%lld|%s%s|\n",head->count,prefix,head->stem);
            entry_count++;
        }

        if(head->down)
        {
            char word[1024] = {0};
            snprintf(word,sizeof(word),"%s%s",prefix,head->stem);
            entry_count += writeoutEntries(head->down,word,fp);
        }

        if(head->right)
        {
            entry_count += writeoutEntries(head->right,prefix,fp);
        }
    }
    return entry_count;
}


size_t getNodesUsed(void)
{
    return node_count;
}
