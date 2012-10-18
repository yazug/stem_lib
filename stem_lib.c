#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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

node * add(node * head, char * remainder)
{
    return head;
}

node * count(node * head, char * remainder)
{
    if(head && remainder)
    {
        int found = 0;
        node * cur = head;
        node * prev = cur;
        while(!found && cur)
        {
            if(cur->stem[0] < remainder[0])
            {
                prev = cur;
                cur = (node*)(cur->right);
                continue;
            }
            else if(cur->stem[0] > remainder[0])
            {
                prev = add(prev,remainder);
            }
            else
            {   // We have a match on this node
                // figure out if we need to split current node, move on to child, or count this node

            }

        }
    }
    return head;
}


int64_t find(node * head, char * remainder)
{
    int64_t count = 0;

    if(head && remainder)
    {
        
    
    }

    return count;
}

