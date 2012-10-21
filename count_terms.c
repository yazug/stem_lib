#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stem_lib.h"


int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        printf("%s <filename_to_process> <folder_to_process_to>\n",argv[0]);
    }

    if(argc == 3)
    {
        node * head = NULL;
        size_t i;
        size_t rows = 0;

        FILE * fp = fopen(argv[1],"r");
        if(fp)
        {
            for(i=0; i<100000; i++)
            {
                char buf[1024] = {0};
                if(NULL != fgets(buf,sizeof(buf),fp))
                {
                    size_t data_len = strlen(buf);
                    if(data_len > 0 && buf[data_len-1] == '\n')
                        buf[data_len-1] = '\0';
                    //printf("Got word [%s]\n",buf);
                    head = count(head,buf,1);
                    rows++;
                }
                else
                {
                    break;
                }
                if(getNodesUsed()*sizeof(node) > 1024*1024*8 )
                {
                    printf("Got to [%d] row and have used [%d] nodes",i,getNodesUsed());
                    break;
                }
            }
            fclose(fp);
            fp = NULL;
        }

        {
            FILE * fp = stdout;

            size_t entry_count = writeoutEntries(head,"", fp);
            size_t node_count = getNodesUsed();
            printf("Got [%d] entries [%d] nodes for [%d] rows using [%g] kb [%0.1f] bytes per entry\n",entry_count, node_count, rows, (node_count*sizeof(node))/1024.0, (node_count*sizeof(node))/(double)entry_count);
        }
    }

    return 0;
}
