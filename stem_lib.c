#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "stem_lib.h"

//then build up the tree using word stems
//if you have a new word that is part of another block split the block move the down entries with the new bock
//
//
//
node * split(node * head, char * remainder)
{
    if(head)
    {
        size_t i=0;
        size_t j=0;
        node * new_node = (node*)malloc(sizeof(node));
        memset(new_node,0,sizeof(node));

        while(head->stem[i] == remainder[i] && remainder[i] != '\0' && i < sizeof(head->stem))
            i++;

        while(head->stem[i] != '\0'  && i < sizeof(head->stem))
        {
            new_node->stem[j++] = head->stem[i];
            head->stem[i] = '\0';
        }
        new_node->down = head->down;
        head->down = new_node;
    }
    return head;
}

node * add(node * head, char * stem)
{
    if(head && stem)
    {
        node * next_node = new_node(stem,1);
        next_node->right = head->right;
        head->right = next_node;
    }
    return head;
}

node * count(node * head, char * stem)
{
    if(NULL == head)
    {
        head = new_node(stem,1);
    }
    else if(head && stem)
    {
        int found = 0;
        node * cur = head;
        node * prev = cur;
        while(!found && cur)
        {
            if(cur->stem[0] < stem[0])
            {
                prev = cur;
                cur = (node*)(cur->right);
                continue;
            }
            else if(cur->stem[0] > stem[0])
            {
                prev = add(prev,stem);
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
                    cur->count ++;
                }

                break;
            }
        }
    }
    return head;
}


int64_t find_node(node * head, char * stem)
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
                break;
            }
        }
    }

    return count;
}

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
        free(head);
    }
}

node * new_node(char * stem, int64_t count)
{
    node * new_node = NULL;
    if(stem)
    {
        new_node = (node*)malloc(sizeof(node));

        if(new_node)
        {
            memset(new_node,0,sizeof(node));
            snprintf(new_node->stem,sizeof(new_node->stem),"%s",stem);
            if(0 == strcmp(new_node->stem,stem))
            {
                new_node->count = count;
            }
        }
    }
    return new_node;
}

