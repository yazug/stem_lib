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

		size_t node_count = prettyprintTree(head,"");
		printf("Got [%d] nodes using [%g] kb\n",node_count,(node_count*sizeof(node))/1024.0);
	}

	return 0;
}
